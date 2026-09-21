#ifndef MOVIMIENTO_H
#define MOVIMIENTO_H

#include "Pieza.h"
#include "Tablero.h"

//Tipos de accion registrable en el historial de la partida
enum TipoMovimiento { MOV_IZQUIERDA, MOV_DERECHA, MOV_ROTAR, MOV_BAJAR, MOV_COLOCAR };

//Guarda una foto completa del estado antes y despues del movimiento
struct Movimiento {
	TipoMovimiento tipo;
	Pieza piezaAntes;
	Pieza piezaDespues;
	Fila tableroAntes[FILAS_TABLERO];
	Fila tableroDespues[FILAS_TABLERO];
};

#endif
