#ifndef PILAHOLD_H
#define PILAHOLD_H

#include "Pieza.h"

//Pila propia de capacidad 1 para la pieza en espera
struct PilaHold {
	Pieza piezaGuardada;
	bool ocupada;
};

PilaHold crearPilaHold();
void apilarHold(PilaHold &pila, Pieza pieza);
Pieza desapilarHold(PilaHold &pila);
bool pilaHoldVacia(PilaHold pila);

#endif
