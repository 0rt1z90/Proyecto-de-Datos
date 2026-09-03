#ifndef FILA_H
#define FILA_H

#include "Celda.h"

const int COLUMNAS_TABLERO = 10;

struct Fila {
    Celda celdas[COLUMNAS_TABLERO];
};

Fila crearFilaVacia();
bool filaEstaCompleta(Fila fila);

#endif
