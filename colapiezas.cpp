#include "ColaPiezas.h"
#include <cstdlib>
#include <ctime>

ColaPiezas crearColaPiezas() {
    ColaPiezas cola;
    cola.frente = nullptr;
    cola.final = nullptr;
    cola.cantidad = 0;
    return cola;
}

void encolarPieza(ColaPiezas &cola, Pieza pieza) {
    NodoPieza *nodoNuevo = new NodoPieza();
    nodoNuevo->dato = pieza;
    nodoNuevo->siguiente = nullptr;


    if(cola.frente == nullptr){
        cola.frente = nodoNuevo;
        cola.final = nodoNuevo;
    }else{
        cola.final->siguiente = nodoNuevo;
        cola.final = nodoNuevo;
    }
    cola.cantidad += 1;
}

Pieza desencolarPieza(ColaPiezas &cola) {
    Pieza pieza;
    if(cola.frente == nullptr){
        return pieza;
    }else{
        Pieza auxPieza = cola.frente->dato;
        NodoPieza *auxNodo = cola.frente;
        cola.frente = cola.frente->siguiente;
        if(cola.frente == nullptr){
            cola.final = nullptr;
        }
        delete auxNodo;
        pieza = auxPieza;
    }
    return pieza;
}

Pieza verFrentePieza(ColaPiezas cola) {
    Pieza pieza;
    if(cola.frente == nullptr){
        return pieza;
    }else{
        return cola.frente->dato;
    }

}

bool colaPiezasVacia(ColaPiezas cola) {

    if(cola.frente == nullptr){
        return true;
    }else{
        return false;
    }
}

void generarNuevaBolsa(ColaPiezas &cola) {
    TipoPieza tiposTemp[7] = {PIEZA_I, PIEZA_O, PIEZA_T, PIEZA_S, PIEZA_Z, PIEZA_J, PIEZA_L };

    for(int i = 0; i < 7; i++){
        int pos = rand() % 7;
        TipoPieza aux = tiposTemp[i];
        tiposTemp[i] = tiposTemp[pos];
        tiposTemp[pos] = aux;
    }

    for(int i = 0; i < 7; i++){
        encolarPieza(cola,crearPieza(tiposTemp[i]));
    }

}
