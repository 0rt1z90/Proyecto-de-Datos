#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QTableWidgetItem>
#include <QAbstractItemView>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *padre) : QMainWindow(padre) {
    juego = crearJuego();
    interfaz = crearInterfaz(this);
    tableroReplay = crearTablero();

    cargarPuntajesDesdeArchivo(juego.tablaPuntajes, "puntajes.txt");

    setWindowTitle("TetrisQT");
    resize(760, 680);

    pilaDePantallas = new QStackedWidget(this);
    setCentralWidget(pilaDePantallas);

    construirPantallaInicio();
    construirPantallaJuego();
    construirPantallaPausa();
    construirPantallaFin();
    construirPantallaReplay();

    pilaDePantallas->addWidget(pantallaInicio);
    pilaDePantallas->addWidget(pantallaJuego);
    pilaDePantallas->addWidget(pantallaPausa);
    pilaDePantallas->addWidget(pantallaFin);
    pilaDePantallas->addWidget(pantallaReplay);

    pilaDePantallas->setCurrentWidget(pantallaInicio);

    connect(&timerJuego, &QTimer::timeout, this, &MainWindow::alTickJuego);
    timerJuego.start(16);
}

// Construccion de pantallas


void MainWindow::construirPantallaInicio() {
    pantallaInicio = new QWidget();
    pantallaInicio->setStyleSheet("background-color: #14141c;");

    QVBoxLayout *diseno = new QVBoxLayout(pantallaInicio);
    diseno->setAlignment(Qt::AlignCenter);

    QLabel *titulo = new QLabel("TETRIS");
    titulo->setAlignment(Qt::AlignCenter);
    titulo->setStyleSheet("font-size: 48px; font-weight: bold; color: white;");

    QLabel *ayuda = new QLabel("Flechas: mover / rotar   Espacio: caida rapida   C: hold   Z: deshacer   X: rehacer   ESC: pausa");
    ayuda->setAlignment(Qt::AlignCenter);
    ayuda->setStyleSheet("color: #aaaaaa;");

    botonJugar = new QPushButton("Jugar");
    botonJugar->setFixedWidth(160);
    botonJugar->setStyleSheet("font-size: 18px; padding: 10px;");

    diseno->addWidget(titulo);
    diseno->addSpacing(10);
    diseno->addWidget(ayuda);
    diseno->addSpacing(30);
    diseno->addWidget(botonJugar, 0, Qt::AlignCenter);

    connect(botonJugar, &QPushButton::clicked, this, &MainWindow::alPresionarJugar);
}

void MainWindow::construirPantallaJuego() {
    pantallaJuego = new QWidget();
    pantallaJuego->setStyleSheet("background-color: #14141c;");

    QHBoxLayout *diseno = new QHBoxLayout(pantallaJuego);

    widgetTablero = new TableroWidget();
    diseno->addWidget(widgetTablero);

    QVBoxLayout *panelLateral = new QVBoxLayout();

    QLabel *etiquetaSiguientes = new QLabel("SIGUIENTES");
    etiquetaSiguientes->setStyleSheet("color: white; font-weight: bold;");
    panelLateral->addWidget(etiquetaSiguientes);

    for(int i = 0; i < 4; i++){
        PiezaPreviewWidget *previa = new PiezaPreviewWidget();
        widgetsSiguientes.push_back(previa);
        panelLateral->addWidget(previa);
    }

    panelLateral->addSpacing(20);

    QLabel *etiquetaHold = new QLabel("HOLD (tecla C)");
    etiquetaHold->setStyleSheet("color: white; font-weight: bold;");
    widgetHold = new PiezaPreviewWidget();
    panelLateral->addWidget(etiquetaHold);
    panelLateral->addWidget(widgetHold);

    panelLateral->addSpacing(20);

    etiquetaPuntaje = new QLabel("Puntaje: 0");
    etiquetaPuntaje->setStyleSheet("color: white; font-size: 16px;");
    panelLateral->addWidget(etiquetaPuntaje);


    //Aviso que sale unos segundos antes de que dispare un evento programado
    etiquetaAvisoEvento = new QLabel("");
    etiquetaAvisoEvento->setStyleSheet("color: #ffcc00; font-size: 13px; font-weight: bold;");
    etiquetaAvisoEvento->setWordWrap(true);
    panelLateral->addWidget(etiquetaAvisoEvento);

    panelLateral->addStretch();

    diseno->addLayout(panelLateral);
}

void MainWindow::construirPantallaPausa() {
    pantallaPausa = new QWidget();
    pantallaPausa->setStyleSheet("background-color: #14141c;");

    QVBoxLayout *diseno = new QVBoxLayout(pantallaPausa);
    diseno->setAlignment(Qt::AlignCenter);

    QLabel *titulo = new QLabel("PAUSA");
    titulo->setAlignment(Qt::AlignCenter);
    titulo->setStyleSheet("font-size: 36px; font-weight: bold; color: white;");

    QPushButton *botonContinuar = new QPushButton("Continuar (ESC)");
    botonContinuar->setFixedWidth(200);

    diseno->addWidget(titulo);
    diseno->addSpacing(20);
    diseno->addWidget(botonContinuar, 0, Qt::AlignCenter);

    connect(botonContinuar, &QPushButton::clicked, this, &MainWindow::alPresionarContinuar);
}

void MainWindow::construirPantallaFin() {
    pantallaFin = new QWidget();
    pantallaFin->setStyleSheet("background-color: #14141c;");

    QVBoxLayout *diseno = new QVBoxLayout(pantallaFin);
    diseno->setAlignment(Qt::AlignCenter);

    QLabel *titulo = new QLabel("FIN DEL JUEGO");
    titulo->setAlignment(Qt::AlignCenter);
    titulo->setStyleSheet("font-size: 32px; font-weight: bold; color: white;");

    etiquetaPuntajeFinal = new QLabel("Puntaje: 0");
    etiquetaPuntajeFinal->setAlignment(Qt::AlignCenter);
    etiquetaPuntajeFinal->setStyleSheet("font-size: 18px; color: white;");

    tablaMejoresPuntajes = new QTableWidget(0, 2);
    tablaMejoresPuntajes->setHorizontalHeaderLabels(QStringList() << "Jugador" << "Puntos");
    tablaMejoresPuntajes->setFixedSize(260, 220);
    tablaMejoresPuntajes->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QHBoxLayout *filaBotonesOrden = new QHBoxLayout();
    botonOrdenInsercion = new QPushButton("Ordenar (Insercion)");
    botonOrdenQuicksort = new QPushButton("Ordenar (Quicksort)");
    filaBotonesOrden->addWidget(botonOrdenInsercion);
    filaBotonesOrden->addWidget(botonOrdenQuicksort);

    botonVerReplay = new QPushButton("Ver Replay de la partida");
    botonVerReplay->setFixedWidth(220);

    botonReiniciar = new QPushButton("Jugar de nuevo");
    botonReiniciar->setFixedWidth(180);

    diseno->addWidget(titulo);
    diseno->addWidget(etiquetaPuntajeFinal);
    diseno->addSpacing(10);
    diseno->addWidget(tablaMejoresPuntajes, 0, Qt::AlignCenter);
    diseno->addLayout(filaBotonesOrden);
    diseno->addSpacing(20);
    diseno->addWidget(botonVerReplay, 0, Qt::AlignCenter);
    diseno->addSpacing(10);
    diseno->addWidget(botonReiniciar, 0, Qt::AlignCenter);

    connect(botonVerReplay, &QPushButton::clicked, this, &MainWindow::alPresionarVerReplay);
    connect(botonReiniciar, &QPushButton::clicked, this, &MainWindow::alPresionarReiniciar);
    connect(botonOrdenInsercion, &QPushButton::clicked, this, &MainWindow::alPresionarOrdenInsercion);
    connect(botonOrdenQuicksort, &QPushButton::clicked, this, &MainWindow::alPresionarOrdenQuicksort);
}

void MainWindow::construirPantallaReplay() {
    pantallaReplay = new QWidget();
    pantallaReplay->setStyleSheet("background-color: #14141c;");

    QVBoxLayout *diseno = new QVBoxLayout(pantallaReplay);
    diseno->setAlignment(Qt::AlignCenter);

    QLabel *titulo = new QLabel("REPLAY DE LA PARTIDA");
    titulo->setAlignment(Qt::AlignCenter);
    titulo->setStyleSheet("font-size: 28px; font-weight: bold; color: white;");

    widgetTableroReplay = new TableroWidget();

    etiquetaInfoReplay = new QLabel("(antes del primer movimiento)");
    etiquetaInfoReplay->setAlignment(Qt::AlignCenter);
    etiquetaInfoReplay->setStyleSheet("color: #cccccc;");

    QHBoxLayout *filaBotonesReplay = new QHBoxLayout();
    botonReplayAtras = new QPushButton("< Atras");
    botonReplayReproducir = new QPushButton("Reproducir");
    botonReplayPausar = new QPushButton("Pausar");
    botonReplayAdelante = new QPushButton("Adelante >");
    filaBotonesReplay->addWidget(botonReplayAtras);
    filaBotonesReplay->addWidget(botonReplayReproducir);
    filaBotonesReplay->addWidget(botonReplayPausar);
    filaBotonesReplay->addWidget(botonReplayAdelante);

    botonReplayVolver = new QPushButton("Volver");
    botonReplayVolver->setFixedWidth(160);

    diseno->addWidget(titulo);
    diseno->addSpacing(10);
    diseno->addWidget(widgetTableroReplay, 0, Qt::AlignCenter);
    diseno->addWidget(etiquetaInfoReplay);
    diseno->addLayout(filaBotonesReplay);
    diseno->addSpacing(20);
    diseno->addWidget(botonReplayVolver, 0, Qt::AlignCenter);

    connect(botonReplayAtras, &QPushButton::clicked, this, &MainWindow::alPresionarReplayAtras);
    connect(botonReplayAdelante, &QPushButton::clicked, this, &MainWindow::alPresionarReplayAdelante);
    connect(botonReplayReproducir, &QPushButton::clicked, this, &MainWindow::alPresionarReplayReproducir);
    connect(botonReplayPausar, &QPushButton::clicked, this, &MainWindow::alPresionarReplayPausar);
    connect(botonReplayVolver, &QPushButton::clicked, this, &MainWindow::alPresionarReplayVolver);

    //Este timer es el que hace avanzar el replay solo, un paso cada vez que suena
    connect(&timerReplay, &QTimer::timeout, this, &MainWindow::alTickReplay);
}

// Puente Interfaz.cpp -> widgets Qt


void MainWindow::actualizarTablero(const Tablero &tablero) {
    widgetTablero->establecerTablero(tablero);
}

void MainWindow::actualizarPiezaActual(const Pieza &pieza) {
    widgetTablero->establecerPiezaActual(pieza, juego.estadoActual == ESTADO_JUGANDO);
}

void MainWindow::actualizarSiguientesPiezas(const ColaPiezas &colaPiezas) {
    NodoPieza *nodo = colaPiezas.frente;

    for(size_t i = 0; i < widgetsSiguientes.size(); i++){
        if(nodo != nullptr){
            widgetsSiguientes[i]->establecerPieza(nodo->dato.tipo, false);
            nodo = nodo->siguiente;
        }else{
            widgetsSiguientes[i]->establecerPieza(PIEZA_I, true);
        }
    }
}

void MainWindow::actualizarHold(const PilaHold &pilaHold) {
    if(pilaHold.ocupada == true){
        widgetHold->establecerPieza(pilaHold.piezaGuardada.tipo, false);
    }else{
        widgetHold->establecerPieza(PIEZA_I, true);
    }
}

void MainWindow::actualizarPuntaje(int puntaje) {
    etiquetaPuntaje->setText(QString("Puntaje: %1").arg(puntaje));
}

void MainWindow::mostrarPantallaInicio() {
    pilaDePantallas->setCurrentWidget(pantallaInicio);
}

void MainWindow::mostrarPantallaPausa() {
    pilaDePantallas->setCurrentWidget(pantallaPausa);
}

void MainWindow::mostrarPantallaFin() {
    etiquetaPuntajeFinal->setText(QString("Puntaje: %1").arg(juego.puntajeActual));
    guardarPuntajesEnArchivo(juego.tablaPuntajes, "puntajes.txt");
    refrescarTablaMejoresPuntajes();
    pilaDePantallas->setCurrentWidget(pantallaFin);
}

void MainWindow::mostrarControlesReplay() {
    //Los controles de replay ya estan siempre visibles en la pantalla de fin;
    //esta funcion queda disponible por si se agrega una pantalla dedicada.
}

// Ciclo de juego

void MainWindow::refrescarPantallaJuego() {
    dibujarTablero(interfaz, juego.tablero);
    dibujarPiezaActual(interfaz, juego.piezaActual);
    dibujarSiguientesPiezas(interfaz, juego.colaPiezas);
    dibujarHold(interfaz, juego.pilaHold);
    dibujarPuntaje(interfaz, juego.puntajeActual);
    actualizarAvisoEvento();

    //La animacion se ajusta a la misma velocidad que trae el evento de velocidad
    int duracionAnimacion = (int)(juego.intervaloCaida * 90.0f);

    if(duracionAnimacion < 30){
        duracionAnimacion = 30;
    }

    widgetTablero->establecerDuracionCaida(duracionAnimacion);
}

//Revisa el proximo evento programado y avisa si falta poco para que dispare
//Revisa el proximo evento programado y avisa si falta poco para que dispare
void MainWindow::actualizarAvisoEvento() {

    QString mensaje = "";

    if(colaEventosVacia(juego.eventos) == false){
        EventoJuego proximoEvento = verEventoFrente(juego.eventos);
        float tiempoRestante = proximoEvento.tiempoDisparo - juego.tiempoTranscurrido;

        if(tiempoRestante <= 8.0f && tiempoRestante > 0.0f){

            if(proximoEvento.tipo == EVENTO_AUMENTAR_VELOCIDAD){
                mensaje = "Aviso: la velocidad va a subir pronto";
            }

            if(proximoEvento.tipo == EVENTO_PIEZA_ESPECIAL){
                mensaje = "Aviso: vienen puntos de bono";
            }

            if(proximoEvento.tipo == EVENTO_LIMPIAR_FILA){
                mensaje = "Aviso: se va a eliminar la fila de abajo";
            }
        }
    }

    //Solo se actualiza el label si el mensaje en verdad cambio
    //asi se evita forzar un recalculo del layout en cada tick
    if(etiquetaAvisoEvento->text() != mensaje){
        etiquetaAvisoEvento->setText(mensaje);
    }
}

void MainWindow::alTickJuego() {
    if(juego.estadoActual != ESTADO_JUGANDO){
        return;
    }

    int puntajeAntes = juego.puntajeActual;
    actualizarJuego(juego, 0.016f);
    int puntosGanados = juego.puntajeActual - puntajeAntes;

    if(puntosGanados > 0){
        widgetTablero->destacarLimpieza(puntosGanados / 100);
    }

    refrescarPantallaJuego();

    if(juego.estadoActual == ESTADO_FIN){
        QString nombreIngresado = QInputDialog::getText(this, "Nuevo puntaje", "Ingresa tu nombre:");

        if(nombreIngresado.isEmpty()){
            nombreIngresado = "Jugador";
        }

        actualizarNombreUltimoRegistro(juego.tablaPuntajes, nombreIngresado.toStdString(), juego.puntajeActual);

        dibujarPantallaFin(interfaz);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *evento) {

    if(pilaDePantallas->currentWidget() == pantallaJuego && juego.estadoActual == ESTADO_JUGANDO){
        switch(evento->key()){
        case Qt::Key_Left: manejarEntradaJuego(juego, TECLA_IZQUIERDA); break;
        case Qt::Key_Right: manejarEntradaJuego(juego, TECLA_DERECHA); break;
        case Qt::Key_Up: manejarEntradaJuego(juego, TECLA_ROTAR); break;
        case Qt::Key_Down: manejarEntradaJuego(juego, TECLA_BAJAR); break;
        case Qt::Key_Space: manejarEntradaJuego(juego, TECLA_CAIDA_RAPIDA); break;
        case Qt::Key_C: manejarEntradaJuego(juego, TECLA_HOLD); break;
        case Qt::Key_Z: deshacerJuego(juego); break;
        case Qt::Key_X: rehacerJuego(juego); break;
        case Qt::Key_Escape:
            juego.estadoActual = ESTADO_PAUSA;
            dibujarPantallaPausa(interfaz);
            break;
        default: break;
        }
        refrescarPantallaJuego();
    }

    QMainWindow::keyPressEvent(evento);
}

// Slots de botones

void MainWindow::alPresionarJugar() {
    juego = crearJuego();
    inicializarJuego(juego);
    cargarPuntajesDesdeArchivo(juego.tablaPuntajes, "puntajes.txt");
    pilaDePantallas->setCurrentWidget(pantallaJuego);
    refrescarPantallaJuego();
    this->setFocus();
}

void MainWindow::alPresionarContinuar() {
    juego.estadoActual = ESTADO_JUGANDO;
    pilaDePantallas->setCurrentWidget(pantallaJuego);
    this->setFocus();
}

void MainWindow::alPresionarReiniciar() {
    juego = crearJuego();
    inicializarJuego(juego);
    cargarPuntajesDesdeArchivo(juego.tablaPuntajes, "puntajes.txt");
    pilaDePantallas->setCurrentWidget(pantallaJuego);
    refrescarPantallaJuego();
    this->setFocus();
}

void MainWindow::refrescarTablaMejoresPuntajes() {
    tablaMejoresPuntajes->setRowCount(juego.tablaPuntajes.cantidad);

    for(int i = 0; i < juego.tablaPuntajes.cantidad; i++){
        QString nombre = QString::fromStdString(juego.tablaPuntajes.registros[i].nombreJugador);
        tablaMejoresPuntajes->setItem(i, 0, new QTableWidgetItem(nombre));
        tablaMejoresPuntajes->setItem(i, 1, new QTableWidgetItem(QString::number(juego.tablaPuntajes.registros[i].puntos)));
    }
}

void MainWindow::mostrarInfoMovimientoReplay(NodoHistorial *nodo) {
    if(nodo == nullptr){
        etiquetaInfoReplay->setText("(antes del primer movimiento)");
        vaciarTablero(tableroReplay);
        widgetTableroReplay->establecerTablero(tableroReplay);
        widgetTableroReplay->establecerPiezaActual(juego.piezaActual, false);
        return;
    }

    QString textoTipo;
    switch(nodo->dato.tipo){
    case MOV_IZQUIERDA: textoTipo = "Izquierda"; break;
    case MOV_DERECHA: textoTipo = "Derecha"; break;
    case MOV_ROTAR: textoTipo = "Rotar"; break;
    case MOV_BAJAR: textoTipo = "Bajar"; break;
    case MOV_COLOCAR: textoTipo = "Colocar"; break;
    }

    etiquetaInfoReplay->setText(QString("%1  ->  fila %2, columna %3, rotacion %4")
                                    .arg(textoTipo)
                                    .arg(nodo->dato.piezaDespues.filaPivote)
                                    .arg(nodo->dato.piezaDespues.columnaPivote)
                                    .arg(nodo->dato.piezaDespues.rotacionActual));

    restaurarTableroDesdeArreglo(tableroReplay, nodo->dato.tableroDespues);
    widgetTableroReplay->establecerTablero(tableroReplay);
    widgetTableroReplay->establecerPiezaActual(nodo->dato.piezaDespues, true);
}

void MainWindow::alPresionarVerReplay() {
    timerReplay.stop();
    iniciarReplay(juego);
    pilaDePantallas->setCurrentWidget(pantallaReplay);
    mostrarInfoMovimientoReplay(obtenerMovimientoActual(juego.historial));
}

void MainWindow::alPresionarReplayAtras() {
    //Se detiene la reproduccion automatica antes de mover manualmente
    timerReplay.stop();
    retrocederReplay(juego);
    mostrarInfoMovimientoReplay(obtenerMovimientoActual(juego.historial));
}

void MainWindow::alPresionarReplayAdelante() {
    timerReplay.stop();
    avanzarReplay(juego);
    mostrarInfoMovimientoReplay(obtenerMovimientoActual(juego.historial));
}

void MainWindow::alPresionarReplayReproducir() {
    //Cada 500 milisegundos avanza un movimiento solo
    timerReplay.start(500);
}

void MainWindow::alPresionarReplayPausar() {
    timerReplay.stop();
}

void MainWindow::alPresionarReplayVolver() {
    timerReplay.stop();
    pilaDePantallas->setCurrentWidget(pantallaFin);
}

void MainWindow::alTickReplay() {
    bool huboAvance = avanzarReplay(juego);

    if(huboAvance == false){
        timerReplay.stop();
        return;
    }

    mostrarInfoMovimientoReplay(obtenerMovimientoActual(juego.historial));
}


void MainWindow::alPresionarOrdenInsercion() {
    ordenarTabla(juego.tablaPuntajes, ALGORITMO_INSERCION);
    refrescarTablaMejoresPuntajes();
}

void MainWindow::alPresionarOrdenQuicksort() {
    ordenarTabla(juego.tablaPuntajes, ALGORITMO_QUICKSORT);
    refrescarTablaMejoresPuntajes();
}