#ifndef INTERFAZ_H
#define INTERFAZ_H

#include "Juego.h"

struct VentanaJuego;

struct Interfaz {
	VentanaJuego *ventana;
};

Interfaz crearInterfaz(VentanaJuego *ventana);
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
