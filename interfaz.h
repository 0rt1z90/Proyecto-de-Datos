#ifndef INTERFAZ_H
#define INTERFAZ_H

#include "Juego.h"

//Declaracion adelantada: MainWindow es quien realmente dibuja con Qt
//(ver mainwindow.h/.cpp). Interfaz.h no puede incluir mainwindow.h porque
//mainwindow.h necesita incluir Juego.h/Interfaz.h, y se generaria un ciclo.
class MainWindow;

//Encargada de dibujar todos los elementos graficos del juego.
//Es solo un puente: guarda un puntero a la ventana Qt real y delega ahi.
struct Interfaz {
    MainWindow *ventana;
};

Interfaz crearInterfaz(MainWindow *ventana);
void dibujarTablero(Interfaz &interfaz, Tablero tablero);
void dibujarPiezaActual(Interfaz &interfaz, Pieza pieza);
void dibujarSiguientesPiezas(Interfaz &interfaz, ColaPiezas colaPiezas);
void dibujarHold(Interfaz &interfaz, PilaHold pilaHold);
void dibujarPuntaje(Interfaz &interfaz, int puntaje);
void dibujarPantallaInicio(Interfaz &interfaz);
void dibujarPantallaPausa(Interfaz &interfaz);
void dibujarPantallaFin(Interfaz &interfaz);
void dibujarControlesReplay(Interfaz &interfaz);

#endif