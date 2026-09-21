#ifndef COLAPIEZAS_H
#define COLAPIEZAS_H

#include "Pieza.h"

//Nodo de la cola enlazada de piezas futuras
struct NodoPieza {
	Pieza dato;
	NodoPieza* siguiente;
};

struct ColaPiezas {
	NodoPieza* frente;
	NodoPieza* final;
	int cantidad;
};

ColaPiezas crearColaPiezas();
void encolarPieza(ColaPiezas &cola, Pieza pieza);
Pieza desencolarPieza(ColaPiezas &cola);
Pieza verFrentePieza(ColaPiezas cola);
bool colaPiezasVacia(ColaPiezas cola);
void generarNuevaBolsa(ColaPiezas &cola);

#endif
