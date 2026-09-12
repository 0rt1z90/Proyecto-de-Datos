#include "PiezaPreviewWidget.h"
#include <QPainter>

const int TAM_CELDA_PREVIEW = 18;

PiezaPreviewWidget::PiezaPreviewWidget(QWidget *padre) : QWidget(padre) {
    estaVacio = true;
    tipoMostrado = PIEZA_I;
    setMinimumSize(4 * TAM_CELDA_PREVIEW, 4 * TAM_CELDA_PREVIEW);
}

QSize PiezaPreviewWidget::sizeHint() const {
    return QSize(4 * TAM_CELDA_PREVIEW, 4 * TAM_CELDA_PREVIEW);
}

void PiezaPreviewWidget::establecerPieza(TipoPieza tipo, bool vacio) {
    tipoMostrado = tipo;
    estaVacio = vacio;
    update();
}

QColor PiezaPreviewWidget::colorPorTipo(int tipo) const {
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

void PiezaPreviewWidget::paintEvent(QPaintEvent *evento) {
    Q_UNUSED(evento);
    QPainter pintor(this);
    pintor.setRenderHint(QPainter::Antialiasing);

    pintor.fillRect(rect(), QColor(30, 30, 40));
    pintor.setPen(QColor(55, 55, 65));
    pintor.drawRect(rect().adjusted(0, 0, -1, -1));

    if(estaVacio == true){
        return;
    }

    Pieza piezaTemporal = crearPieza(tipoMostrado);
    piezaTemporal.filaPivote = 0;
    piezaTemporal.columnaPivote = 0;

    Bloque bloques[4];
    obtenerBloquesPieza(piezaTemporal, bloques);

    //Se centra la figura dentro de la caja de 4x4 celdas
    int filaMinima = bloques[0].fila;
    int columnaMinima = bloques[0].columna;
    int filaMaxima = bloques[0].fila;
    int columnaMaxima = bloques[0].columna;

    for(int i = 1; i < 4; i++){
        if(bloques[i].fila < filaMinima){ filaMinima = bloques[i].fila; }
        if(bloques[i].fila > filaMaxima){ filaMaxima = bloques[i].fila; }
        if(bloques[i].columna < columnaMinima){ columnaMinima = bloques[i].columna; }
        if(bloques[i].columna > columnaMaxima){ columnaMaxima = bloques[i].columna; }
    }

    int anchoFigura = (columnaMaxima - columnaMinima + 1) * TAM_CELDA_PREVIEW;
    int altoFigura = (filaMaxima - filaMinima + 1) * TAM_CELDA_PREVIEW;
    int desplazX = (width() - anchoFigura) / 2;
    int desplazY = (height() - altoFigura) / 2;

    for(int i = 0; i < 4; i++){
        int x = desplazX + (bloques[i].columna - columnaMinima) * TAM_CELDA_PREVIEW;
        int y = desplazY + (bloques[i].fila - filaMinima) * TAM_CELDA_PREVIEW;
        QRect celda(x + 1, y + 1, TAM_CELDA_PREVIEW - 2, TAM_CELDA_PREVIEW - 2);
        pintor.fillRect(celda, colorPorTipo(tipoMostrado));
    }
}