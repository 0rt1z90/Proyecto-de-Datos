#include "Interfaz.h"
#include "mainwindow.h"

Interfaz crearInterfaz(MainWindow *ventana) {
    Interfaz interfaz;
    interfaz.ventana = ventana;
    return interfaz;
}

void dibujarTablero(Interfaz &interfaz, Tablero tablero) {
    interfaz.ventana->actualizarTablero(tablero);
}

void dibujarPiezaActual(Interfaz &interfaz, Pieza pieza) {
    interfaz.ventana->actualizarPiezaActual(pieza);
}

void dibujarSiguientesPiezas(Interfaz &interfaz, ColaPiezas colaPiezas) {
    interfaz.ventana->actualizarSiguientesPiezas(colaPiezas);
}

void dibujarHold(Interfaz &interfaz, PilaHold pilaHold) {
    interfaz.ventana->actualizarHold(pilaHold);
}

void dibujarPuntaje(Interfaz &interfaz, int puntaje) {
    interfaz.ventana->actualizarPuntaje(puntaje);
}

void dibujarPantallaInicio(Interfaz &interfaz) {
    interfaz.ventana->mostrarPantallaInicio();
}

void dibujarPantallaPausa(Interfaz &interfaz) {
    interfaz.ventana->mostrarPantallaPausa();
}

void dibujarPantallaFin(Interfaz &interfaz) {
    interfaz.ventana->mostrarPantallaFin();
}

void dibujarControlesReplay(Interfaz &interfaz) {
    interfaz.ventana->mostrarControlesReplay();
}