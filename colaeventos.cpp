#include "ColaEventos.h"

ColaEventos crearColaEventos() {
	ColaEventos cola;
	cola.frente = nullptr;
	cola.cantidad = 0;
	return cola;
}

void insertarEventoOrdenado(ColaEventos &cola, EventoJuego evento) {
	NodoEvento *nodoNuevo = new NodoEvento();
	nodoNuevo->dato = evento;
	
	//Si va de primero, antes de la cabeza actual
	if(cola.frente == nullptr || evento.tiempoDisparo < cola.frente->dato.tiempoDisparo){
		nodoNuevo->siguiente = cola.frente;
		cola.frente = nodoNuevo;
	}else{
		NodoEvento *auxActual = cola.frente;
		
		while(auxActual->siguiente != nullptr && auxActual->siguiente->dato.tiempoDisparo <= evento.tiempoDisparo){
			auxActual = auxActual->siguiente;
		}
		
		nodoNuevo->siguiente = auxActual->siguiente;
		auxActual->siguiente = nodoNuevo;
	}
	
	cola.cantidad++;
}

EventoJuego extraerEventoFrente(ColaEventos &cola) {
	EventoJuego evento;
	
	if(cola.frente == nullptr){
		return evento;
	}
	
	NodoEvento *auxNodo = cola.frente;
	evento = auxNodo->dato;
	cola.frente = cola.frente->siguiente;
	delete auxNodo;
	cola.cantidad--;
	
	return evento;
}

EventoJuego verEventoFrente(ColaEventos cola) {
	EventoJuego evento;
	
	if(cola.frente == nullptr){
		return evento;
	}
	
	return cola.frente->dato;
}

bool colaEventosVacia(ColaEventos cola) {
	
	if(cola.frente == nullptr){
		return true;
	}else{
		return false;
	}
}
