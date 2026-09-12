#ifndef LISTAHISTORIAL_H
#define LISTAHISTORIAL_H

#include "Movimiento.h"

//Nodo de la lista doblemente enlazada de movimientos
struct NodoHistorial {
    Movimiento dato;
    NodoHistorial* anterior;
    NodoHistorial* siguiente;
};

struct ListaHistorial {
    NodoHistorial* primero;
    NodoHistorial* ultimo;
    NodoHistorial* actual;
    int cantidad;
};

ListaHistorial crearListaHistorial();
void agregarMovimiento(ListaHistorial &lista, Movimiento movimiento);
bool deshacerMovimiento(ListaHistorial &lista);
bool rehacerMovimiento(ListaHistorial &lista);
void irAlPrimerMovimiento(ListaHistorial &lista);
NodoHistorial* obtenerMovimientoActual(ListaHistorial lista);

#endif
