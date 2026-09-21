#include "Fila.h"

Fila crearFilaVacia() {
	Fila fila;
	
	for(int i = 0; i < COLUMNAS_TABLERO; i++){
		fila.celdas[i].ocupada = false;
		fila.celdas[i].colorPieza = -1;
	}
	
	return fila;
}

bool filaEstaCompleta(Fila fila) {
	
	for(int i = 0; i < COLUMNAS_TABLERO; i++){
		if(fila.celdas[i].ocupada == false){
			return false;
		}
	}
	
	return true;
}
