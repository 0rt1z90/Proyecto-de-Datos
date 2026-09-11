#include "PilaHold.h"

PilaHold crearPilaHold() {
    PilaHold pila;
    pila.ocupada = false;
    return pila;
}

void apilarHold(PilaHold &pila, Pieza pieza) {

    pila.piezaGuardada = pieza;
    pila.ocupada = true;

}

Pieza desapilarHold(PilaHold &pila) {
    Pieza pieza;
    Pieza auxPieza;

    if(pila.ocupada == false){
        return pieza;
    }else{
        auxPieza = pila.piezaGuardada;
        pila.ocupada = false;
        return auxPieza;
    }

}

bool pilaHoldVacia(PilaHold pila) {
    if(pila.ocupada == false){
        return true;
    }else{
        return false;
    }
}
