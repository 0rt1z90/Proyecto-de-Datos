#include "TableroWidget.h"
#include <QPainter>

TableroWidget::TableroWidget(QWidget *padre) : QWidget(padre) {
    tableroValido = false;
    tienePieza = false;
    filaAnimadaActual = 0.0f;
    columnaAnimadaActual = 3.0f;
    opacidadDestello = 0.0;

    animacionCaida.setDuration(90);
    animacionCaida.setEasingCurve(QEasingCurve::OutQuad);
    connect(&animacionCaida, &QVariantAnimation::valueChanged, this, [this](const QVariant &valor){
        QPointF punto = valor.toPointF();
        filaAnimadaActual = (float)punto.x();
        columnaAnimadaActual = (float)punto.y();
        update();
    });

    animacionDestello.setStartValue(0.75);
    animacionDestello.setEndValue(0.0);
    animacionDestello.setDuration(350);
    connect(&animacionDestello, &QVariantAnimation::valueChanged, this, [this](const QVariant &valor){
        opacidadDestello = valor.toReal();
        update();
    });

    setMinimumSize(COLUMNAS_TABLERO * TAM_CELDA, FILAS_TABLERO * TAM_CELDA);
}

QSize TableroWidget::sizeHint() const {
    return QSize(COLUMNAS_TABLERO * TAM_CELDA, FILAS_TABLERO * TAM_CELDA);
}

void TableroWidget::establecerTablero(const Tablero &tablero) {
    tableroActual = tablero;
    tableroValido = true;
    update();
}

//Cambia que tan rapido se ve la animacion de caida en pantalla
void TableroWidget::establecerDuracionCaida(int milisegundos) {
    animacionCaida.setDuration(milisegundos);
}

void TableroWidget::establecerPiezaActual(const Pieza &pieza, bool hayPieza) {

    if(hayPieza == false){
        tienePieza = false;
        update();
        return;
    }

    //Si es una pieza distinta a la que se estaba mostrando (pieza nueva
    //recien generada, o venia de hold) no se anima el salto, se coloca directo
    bool esPiezaNueva = (tienePieza == false) || (piezaMostrada.tipo != pieza.tipo && piezaMostrada.filaPivote == 0);

    float filaOrigen = filaAnimadaActual;
    float columnaOrigen = columnaAnimadaActual;

    if(esPiezaNueva){
        filaOrigen = (float)pieza.filaPivote;
        columnaOrigen = (float)pieza.columnaPivote;
    }

    piezaMostrada = pieza;
    tienePieza = true;

    animacionCaida.stop();
    animacionCaida.setStartValue(QPointF(filaOrigen, columnaOrigen));
    animacionCaida.setEndValue(QPointF((float)pieza.filaPivote, (float)pieza.columnaPivote));
    animacionCaida.start();
}

void TableroWidget::destacarLimpieza(int cantidadFilas) {
    if(cantidadFilas <= 0){
        return;
    }
    animacionDestello.stop();
    animacionDestello.start();
}

QColor TableroWidget::colorPorTipo(int tipo) const {
    switch(tipo){
    case PIEZA_I: return QColor(0, 188, 212);
    case PIEZA_O: return QColor(255, 193, 7);
    case PIEZA_T: return QColor(156, 39, 176);
    case PIEZA_S: return QColor(76, 175, 80);
    case PIEZA_Z: return QColor(244, 67, 54);
    case PIEZA_J: return QColor(33, 150, 243);
    case PIEZA_L: return QColor(255, 152, 0);
    default: return QColor(60, 60, 70);
    }
}

void TableroWidget::paintEvent(QPaintEvent *evento) {
    Q_UNUSED(evento);
    QPainter pintor(this);
    pintor.setRenderHint(QPainter::Antialiasing);

    pintor.fillRect(rect(), QColor(20, 20, 28));

    //Cuadricula
    pintor.setPen(QColor(45, 45, 55));
    for(int columna = 0; columna <= COLUMNAS_TABLERO; columna++){
        pintor.drawLine(columna * TAM_CELDA, 0, columna * TAM_CELDA, FILAS_TABLERO * TAM_CELDA);
    }
    for(int fila = 0; fila <= FILAS_TABLERO; fila++){
        pintor.drawLine(0, fila * TAM_CELDA, COLUMNAS_TABLERO * TAM_CELDA, fila * TAM_CELDA);
    }

    //Celdas fijas del tablero
    if(tableroValido == true){
        for(int fila = 0; fila < FILAS_TABLERO; fila++){
            NodoFila *nodo = obtenerNodoFila(tableroActual, fila);

            if(nodo == nullptr){
                continue;
            }

            for(int columna = 0; columna < COLUMNAS_TABLERO; columna++){
                if(nodo->dato.celdas[columna].ocupada == true){
                    QRect celda(columna * TAM_CELDA + 1, fila * TAM_CELDA + 1, TAM_CELDA - 2, TAM_CELDA - 2);
                    pintor.fillRect(celda, colorPorTipo(nodo->dato.celdas[columna].colorPieza));
                }
            }
        }
    }

    //Pieza actual: se dibuja en su posicion animada (deslizandose), no en la logica directa
    if(tienePieza == true){
        Bloque bloques[4];
        obtenerBloquesPieza(piezaMostrada, bloques);

        for(int i = 0; i < 4; i++){
            int desplazFila = bloques[i].fila - piezaMostrada.filaPivote;
            int desplazColumna = bloques[i].columna - piezaMostrada.columnaPivote;

            int filaDibujo = (int)(filaAnimadaActual + desplazFila + 0.5f);
            int columnaDibujo = (int)(columnaAnimadaActual + desplazColumna + 0.5f);

            if(filaDibujo >= 0 && filaDibujo < FILAS_TABLERO){
                QRect celda(columnaDibujo * TAM_CELDA + 1, filaDibujo * TAM_CELDA + 1, TAM_CELDA - 2, TAM_CELDA - 2);
                pintor.fillRect(celda, colorPorTipo(piezaMostrada.tipo));
            }
        }
    }

    //Destello de limpieza de filas
    if(opacidadDestello > 0.0){
        QColor blanco(255, 255, 255);
        blanco.setAlphaF((float)opacidadDestello);
        pintor.fillRect(rect(), blanco);
    }
}