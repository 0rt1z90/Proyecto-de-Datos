#ifndef PIEZA_H
#define PIEZA_H

//Tipos de pieza del tetris clasico
enum TipoPieza { PIEZA_I, PIEZA_O, PIEZA_T, PIEZA_S, PIEZA_Z, PIEZA_J, PIEZA_L };

struct Bloque {
	int fila;
	int columna;
};

struct Pieza {
	TipoPieza tipo;
	int rotacionActual;
	int filaPivote;
	int columnaPivote;
};

Pieza crearPieza(TipoPieza tipo);
void obtenerBloquesPieza(Pieza pieza, Bloque bloques[4]);
void rotarPieza(Pieza &pieza);
void moverPiezaIzquierda(Pieza &pieza);
void moverPiezaDerecha(Pieza &pieza);
void bajarPieza(Pieza &pieza);

#endif
