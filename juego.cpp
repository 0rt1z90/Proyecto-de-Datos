#include "Juego.h"
#include <QDebug>

Juego crearJuego() {
    Juego juego;
    return juego;
}

void inicializarJuego(Juego &juego) {
    juego.tablero = crearTablero();
    juego.colaPiezas = crearColaPiezas();
    juego.pilaHold = crearPilaHold();
    juego.historial = crearListaHistorial();
    juego.eventos = crearColaEventos();
    juego.tablaPuntajes = crearTablaPuntajes();

    generarNuevaBolsa(juego.colaPiezas);

    juego.estadoActual = ESTADO_JUGANDO;
    juego.puntajeActual = 0;
    juego.tiempoTranscurrido = 0.0f;
    juego.intervaloCaida = 1.0f;

    //Los 3 eventos
    EventoJuego eventoVelocidad;
    eventoVelocidad.tipo = EVENTO_AUMENTAR_VELOCIDAD;
    eventoVelocidad.tiempoDisparo = 15.0f;
    insertarEventoOrdenado(juego.eventos, eventoVelocidad);

    EventoJuego eventoPiezaEspecial;
    eventoPiezaEspecial.tipo = EVENTO_PIEZA_ESPECIAL;
    eventoPiezaEspecial.tiempoDisparo = 25.0f;
    insertarEventoOrdenado(juego.eventos, eventoPiezaEspecial);

    EventoJuego eventoLimpiarFila;
    eventoLimpiarFila.tipo = EVENTO_LIMPIAR_FILA;
    eventoLimpiarFila.tiempoDisparo = 40.0f;
    insertarEventoOrdenado(juego.eventos, eventoLimpiarFila);

    generarNuevaPiezaActual(juego);
}

void actualizarJuego(Juego &juego, float deltaTiempo) {
    static float tiempoAcumuladoCaida = 0.0f;

    if(juego.estadoActual != ESTADO_JUGANDO){
        return;
    }

    juego.tiempoTranscurrido = juego.tiempoTranscurrido + deltaTiempo;
    tiempoAcumuladoCaida = tiempoAcumuladoCaida + deltaTiempo;

    procesarEventosProgramados(juego);

    if(tiempoAcumuladoCaida >= juego.intervaloCaida){
        tiempoAcumuladoCaida = 0.0f;
        manejarEntradaJuego(juego, TECLA_BAJAR);
    }
}

void manejarEntradaJuego(Juego &juego, int tecla) {

    if(juego.estadoActual != ESTADO_JUGANDO){
        return;
    }

    if(tecla == TECLA_IZQUIERDA){
        Pieza piezaNueva = juego.piezaActual;
        moverPiezaIzquierda(piezaNueva);

        if(hayColision(juego.tablero, piezaNueva) == false){
            Pieza piezaAntes = juego.piezaActual;
            Fila tableroAntes[FILAS_TABLERO];
            copiarTableroAArreglo(juego.tablero, tableroAntes);

            juego.piezaActual = piezaNueva;
            registrarMovimientoActual(juego, MOV_IZQUIERDA, piezaAntes, tableroAntes);
        }
    }

    if(tecla == TECLA_DERECHA){
        Pieza piezaNueva = juego.piezaActual;
        moverPiezaDerecha(piezaNueva);

        if(hayColision(juego.tablero, piezaNueva) == false){
            Pieza piezaAntes = juego.piezaActual;
            Fila tableroAntes[FILAS_TABLERO];
            copiarTableroAArreglo(juego.tablero, tableroAntes);

            juego.piezaActual = piezaNueva;
            registrarMovimientoActual(juego, MOV_DERECHA, piezaAntes, tableroAntes);
        }
    }

    if(tecla == TECLA_ROTAR){
        Pieza piezaNueva = juego.piezaActual;
        rotarPieza(piezaNueva);

        if(hayColision(juego.tablero, piezaNueva) == false){
            Pieza piezaAntes = juego.piezaActual;
            Fila tableroAntes[FILAS_TABLERO];
            copiarTableroAArreglo(juego.tablero, tableroAntes);

            juego.piezaActual = piezaNueva;
            registrarMovimientoActual(juego, MOV_ROTAR, piezaAntes, tableroAntes);
        }
    }

    if(tecla == TECLA_BAJAR){
        Pieza piezaNueva = juego.piezaActual;
        bajarPieza(piezaNueva);

        if(hayColision(juego.tablero, piezaNueva) == false){
            Pieza piezaAntes = juego.piezaActual;
            Fila tableroAntes[FILAS_TABLERO];
            copiarTableroAArreglo(juego.tablero, tableroAntes);

            juego.piezaActual = piezaNueva;
            registrarMovimientoActual(juego, MOV_BAJAR, piezaAntes, tableroAntes);
        }else{
            Pieza piezaAntes = juego.piezaActual;
            Fila tableroAntes[FILAS_TABLERO];
            copiarTableroAArreglo(juego.tablero, tableroAntes);

            fijarPiezaEnTablero(juego.tablero, juego.piezaActual);
            registrarMovimientoActual(juego, MOV_COLOCAR, piezaAntes, tableroAntes);

            int filasEliminadas = eliminarFilasCompletas(juego.tablero);
            juego.puntajeActual = juego.puntajeActual + (filasEliminadas * 100);

            generarNuevaPiezaActual(juego);
        }
    }

    if(tecla == TECLA_CAIDA_RAPIDA){
        Pieza piezaAntes = juego.piezaActual;
        Fila tableroAntes[FILAS_TABLERO];
        copiarTableroAArreglo(juego.tablero, tableroAntes);

        Pieza piezaNueva = juego.piezaActual;
        bajarPieza(piezaNueva);

        while(hayColision(juego.tablero, piezaNueva) == false){
            juego.piezaActual = piezaNueva;
            bajarPieza(piezaNueva);
        }

        fijarPiezaEnTablero(juego.tablero, juego.piezaActual);
        registrarMovimientoActual(juego, MOV_COLOCAR, piezaAntes, tableroAntes);

        int filasEliminadas = eliminarFilasCompletas(juego.tablero);
        juego.puntajeActual = juego.puntajeActual + (filasEliminadas * 100);

        generarNuevaPiezaActual(juego);
    }

    if(tecla == TECLA_HOLD){
        enviarPiezaAHold(juego);
    }
}

void generarNuevaPiezaActual(Juego &juego) {

    if(colaPiezasVacia(juego.colaPiezas) == true){
        generarNuevaBolsa(juego.colaPiezas);
    }

    juego.piezaActual = desencolarPieza(juego.colaPiezas);

    if(hayColision(juego.tablero, juego.piezaActual) == true){
        finalizarPartida(juego);
    }
}

void enviarPiezaAHold(Juego &juego) {

    if(pilaHoldVacia(juego.pilaHold) == true){
        apilarHold(juego.pilaHold, juego.piezaActual);
        generarNuevaPiezaActual(juego);
    }else{
        Pieza piezaTemporal = desapilarHold(juego.pilaHold);
        apilarHold(juego.pilaHold, juego.piezaActual);

        juego.piezaActual = crearPieza(piezaTemporal.tipo);
    }
}

void procesarEventosProgramados(Juego &juego) {

    while(colaEventosVacia(juego.eventos) == false && verEventoFrente(juego.eventos).tiempoDisparo <= juego.tiempoTranscurrido){
        EventoJuego evento = extraerEventoFrente(juego.eventos);

        if(evento.tipo == EVENTO_AUMENTAR_VELOCIDAD){
            juego.intervaloCaida = juego.intervaloCaida - 0.15f;

            if(juego.intervaloCaida < 0.1f){
                juego.intervaloCaida = 0.1f;
            }

            qDebug() << "Nuevo intervalo de caida:" << juego.intervaloCaida;

            //Se vuelve a programar para que siga subiendo la velocidad cada rato
            EventoJuego eventoNuevo;
            eventoNuevo.tipo = EVENTO_AUMENTAR_VELOCIDAD;
            eventoNuevo.tiempoDisparo = juego.tiempoTranscurrido + 15.0f;
            insertarEventoOrdenado(juego.eventos, eventoNuevo);
        }

        if(evento.tipo == EVENTO_PIEZA_ESPECIAL){
            //Otorga puntos extra de bono cada cierto tiempo
            juego.puntajeActual = juego.puntajeActual + 200;

            EventoJuego eventoNuevo;
            eventoNuevo.tipo = EVENTO_PIEZA_ESPECIAL;
            eventoNuevo.tiempoDisparo = juego.tiempoTranscurrido + 25.0f;
            insertarEventoOrdenado(juego.eventos, eventoNuevo);
        }

        if(evento.tipo == EVENTO_LIMPIAR_FILA){
            //Elimina la fila inferior para dar espacio al jugador
            eliminarFilaInferior(juego.tablero);

            EventoJuego eventoNuevo;
            eventoNuevo.tipo = EVENTO_LIMPIAR_FILA;
            eventoNuevo.tiempoDisparo = juego.tiempoTranscurrido + 40.0f;
            insertarEventoOrdenado(juego.eventos, eventoNuevo);
        }
    }
}

void registrarMovimientoActual(Juego &juego, TipoMovimiento tipo, Pieza piezaAntes, Fila tableroAntes[FILAS_TABLERO]) {
    Movimiento movimiento;
    movimiento.tipo = tipo;
    movimiento.piezaAntes = piezaAntes;
    movimiento.piezaDespues = juego.piezaActual;

    for(int i = 0; i < FILAS_TABLERO; i++){
        movimiento.tableroAntes[i] = tableroAntes[i];
    }

    copiarTableroAArreglo(juego.tablero, movimiento.tableroDespues);

    agregarMovimiento(juego.historial, movimiento);
}

void finalizarPartida(Juego &juego) {
    juego.estadoActual = ESTADO_FIN;

    if(calificaParaTabla(juego.tablaPuntajes, juego.puntajeActual) == true){
        RegistroPuntaje registro;
        registro.nombreJugador = "Jugador";
        registro.puntos = juego.puntajeActual;
        agregarRegistroPuntaje(juego.tablaPuntajes, registro);
    }
}

void iniciarReplay(Juego &juego) {
    juego.estadoActual = ESTADO_REPLAY;
    irAlPrimerMovimiento(juego.historial);
}

bool avanzarReplay(Juego &juego) {
    //Se devuelve el resultado para saber si ya se llego al final del historial
    bool huboAvance = rehacerMovimiento(juego.historial);
    return huboAvance;
}

void retrocederReplay(Juego &juego) {
    deshacerMovimiento(juego.historial);
}

//Deshacer/rehacer durante la partida en curso, usa la foto guardada en el nodo
bool deshacerJuego(Juego &juego) {

    if(juego.estadoActual != ESTADO_JUGANDO){
        return false;
    }

    NodoHistorial *nodoActual = obtenerMovimientoActual(juego.historial);

    if(nodoActual == nullptr){
        return false;
    }

    Movimiento movimiento = nodoActual->dato;
    juego.piezaActual = movimiento.piezaAntes;
    restaurarTableroDesdeArreglo(juego.tablero, movimiento.tableroAntes);

    deshacerMovimiento(juego.historial);

    return true;
}

bool rehacerJuego(Juego &juego) {

    if(juego.estadoActual != ESTADO_JUGANDO){
        return false;
    }

    bool huboRehacer = rehacerMovimiento(juego.historial);

    if(huboRehacer == false){
        return false;
    }

    NodoHistorial *nodoActual = obtenerMovimientoActual(juego.historial);
    Movimiento movimiento = nodoActual->dato;
    juego.piezaActual = movimiento.piezaDespues;
    restaurarTableroDesdeArreglo(juego.tablero, movimiento.tableroDespues);

    return true;
}