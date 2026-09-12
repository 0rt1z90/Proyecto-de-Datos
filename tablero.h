#ifndef TABLERO_H
#define TABLERO_H

#include "Fila.h"
#include "Pieza.h"

const int FILAS_TABLERO = 20;

//Nodo de la lista enlazada que representa una fila del tablero
struct NodoFila {
    Fila dato;
    NodoFila* siguiente;
};

//Tablero representado como lista enlazada de filas
struct Tablero {
    NodoFila* primeraFila;
    int cantidadFilas;
};

Tablero crearTablero();
bool hayColision(Tablero tablero, Pieza pieza);
void fijarPiezaEnTablero(Tablero &tablero, Pieza pieza);
int eliminarFilasCompletas(Tablero &tablero);
void eliminarFilaInferior(Tablero &tablero);
void insertarFilaVaciaAlInicio(Tablero &tablero);
NodoFila* obtenerNodoFila(Tablero tablero, int indice);
void copiarTableroAArreglo(Tablero tablero, Fila arreglo[FILAS_TABLERO]);
void restaurarTableroDesdeArreglo(Tablero &tablero, Fila arreglo[FILAS_TABLERO]);
void vaciarTablero(Tablero &tablero);

#endif