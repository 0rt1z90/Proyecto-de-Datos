#include "ListaHistorial.h"

ListaHistorial crearListaHistorial() {
    ListaHistorial lista;
    lista.primero = nullptr;
    lista.ultimo = nullptr;
    lista.actual = nullptr;
    lista.cantidad = 0;
    return lista;
}

void agregarMovimiento(ListaHistorial &lista, Movimiento movimiento) {

    //Si el actual no es el ultimo, se borra todo lo que hay despues
    if(lista.actual != lista.ultimo){
        NodoHistorial *auxActual = lista.ultimo;

        while(auxActual != lista.actual){
            NodoHistorial *auxAnterior = auxActual->anterior;
            delete auxActual;
            auxActual = auxAnterior;
            lista.cantidad--;
        }

        if(lista.actual == nullptr){
            lista.primero = nullptr;
        }else{
            lista.actual->siguiente = nullptr;
        }

        lista.ultimo = lista.actual;
    }

    NodoHistorial *nodoNuevo = new NodoHistorial();
    nodoNuevo->dato = movimiento;
    nodoNuevo->siguiente = nullptr;
    nodoNuevo->anterior = lista.ultimo;

    if(lista.primero == nullptr){
        lista.primero = nodoNuevo;
    }else{
        lista.ultimo->siguiente = nodoNuevo;
    }

    lista.ultimo = nodoNuevo;
    lista.actual = nodoNuevo;
    lista.cantidad++;
}

bool deshacerMovimiento(ListaHistorial &lista) {

    if(lista.actual == nullptr){
        return false;
    }

    lista.actual = lista.actual->anterior;
    return true;
}

bool rehacerMovimiento(ListaHistorial &lista) {

    if(lista.actual == lista.ultimo){
        return false;
    }

    if(lista.actual == nullptr){
        lista.actual = lista.primero;
    }else{
        lista.actual = lista.actual->siguiente;
    }

    return true;
}

void irAlPrimerMovimiento(ListaHistorial &lista) {
    lista.actual = lista.primero;
}

NodoHistorial* obtenerMovimientoActual(ListaHistorial lista) {
    return lista.actual;
}
