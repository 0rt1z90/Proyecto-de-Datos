#ifndef COLAEVENTOS_H
#define COLAEVENTOS_H

#include "EventoJuego.h"

//Nodo de la cola de eventos ordenada por tiempo de disparo
struct NodoEvento {
    EventoJuego dato;
    NodoEvento* siguiente;
};

struct ColaEventos {
    NodoEvento* frente;
    int cantidad;
};

ColaEventos crearColaEventos();
void insertarEventoOrdenado(ColaEventos &cola, EventoJuego evento);
EventoJuego extraerEventoFrente(ColaEventos &cola);
EventoJuego verEventoFrente(ColaEventos cola);
bool colaEventosVacia(ColaEventos cola);

#endif
