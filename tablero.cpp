#include "Tablero.h"

Tablero crearTablero() {
	Tablero tablero;
	tablero.primeraFila = nullptr;
	tablero.cantidadFilas = 0;
	
	for(int i = 0; i < FILAS_TABLERO; i++){
		insertarFilaVaciaAlInicio(tablero);
	}
	
	return tablero;
}

bool hayColision(Tablero tablero, Pieza pieza) {
	Bloque bloques[4];
	obtenerBloquesPieza(pieza, bloques);
	
	for(int i = 0; i < 4; i++){
		if(bloques[i].columna < 0 || bloques[i].columna >= COLUMNAS_TABLERO){
			return true;
		}
		if(bloques[i].fila >= FILAS_TABLERO){
			return true;
		}
		if(bloques[i].fila >= 0){
			NodoFila *nodoFila = obtenerNodoFila(tablero, bloques[i].fila);
			
			if(nodoFila->dato.celdas[bloques[i].columna].ocupada == true){
				return true;
			}
		}
	}
	return false;
}

void fijarPiezaEnTablero(Tablero &tablero, Pieza pieza) {
	Bloque bloques[4];
	obtenerBloquesPieza(pieza, bloques);
	
	for(int i = 0; i < 4; i++){
		
		NodoFila *nodoFila = obtenerNodoFila(tablero, bloques[i].fila);
		
		nodoFila->dato.celdas[bloques[i].columna].ocupada = true;
		nodoFila->dato.celdas[bloques[i].columna].colorPieza = pieza.tipo;
	}
}

int eliminarFilasCompletas(Tablero &tablero) {
	int cantidadEliminadas = 0;
	NodoFila *auxActual = tablero.primeraFila;
	NodoFila *auxAnterior = nullptr;
	
	while (auxActual != nullptr) {
		if (filaEstaCompleta(auxActual->dato)) {
			
			//Si es la cabeza
			if (auxAnterior == nullptr) {
				tablero.primeraFila = auxActual->siguiente;
				delete auxActual;
				auxActual = tablero.primeraFila;
				cantidadEliminadas++;
			}
			
			//Si es medio o final
			else {
				auxAnterior->siguiente = auxActual->siguiente;
				delete auxActual;
				auxActual = auxAnterior->siguiente;
				cantidadEliminadas++;
			}
			
		}else{
			auxAnterior = auxActual;
			auxActual = auxActual->siguiente;
		}
	}
	
	tablero.cantidadFilas -= cantidadEliminadas;
	
	for(int i = 0; i < cantidadEliminadas; i++){
		insertarFilaVaciaAlInicio(tablero);
	}
	
	return cantidadEliminadas;
}

//Elimina la fila de mas abajo sin importar si esta completa o no
//Se usa para el evento EVENTO_LIMPIAR_FILA
void eliminarFilaInferior(Tablero &tablero) {
	NodoFila *penultimo = nullptr;
	NodoFila *ultimo = tablero.primeraFila;
	
	while(ultimo->siguiente != nullptr){
		penultimo = ultimo;
		ultimo = ultimo->siguiente;
	}
	
	if(penultimo == nullptr){
		tablero.primeraFila = nullptr;
	}else{
		penultimo->siguiente = nullptr;
	}
	
	delete ultimo;
	tablero.cantidadFilas--;
	
	insertarFilaVaciaAlInicio(tablero);
}

void insertarFilaVaciaAlInicio(Tablero &tablero) {
	NodoFila *nuevoNodoFila = new NodoFila;
	nuevoNodoFila->dato = crearFilaVacia();
	nuevoNodoFila->siguiente = tablero.primeraFila;
	tablero.primeraFila = nuevoNodoFila;
	tablero.cantidadFilas = tablero.cantidadFilas + 1;
}

NodoFila* obtenerNodoFila(Tablero tablero, int indice) {
	if(indice < 0 || indice >= tablero.cantidadFilas){
		return nullptr;
	}
	
	NodoFila *auxNodoFila = tablero.primeraFila;
	while(indice > 0){
		auxNodoFila = auxNodoFila->siguiente;
		indice--;
		if(auxNodoFila == nullptr){
			return nullptr;
		}
	}
	
	return auxNodoFila;
}

//Se usa para guardar la foto del tablero dentro de un Movimiento
void copiarTableroAArreglo(Tablero tablero, Fila arreglo[FILAS_TABLERO]) {
	NodoFila *nodo = tablero.primeraFila;
	
	for(int i = 0; i < FILAS_TABLERO; i++){
		arreglo[i] = nodo->dato;
		nodo = nodo->siguiente;
	}
}

void restaurarTableroDesdeArreglo(Tablero &tablero, Fila arreglo[FILAS_TABLERO]) {
	NodoFila *nodo = tablero.primeraFila;
	
	for(int i = 0; i < FILAS_TABLERO; i++){
		nodo->dato = arreglo[i];
		nodo = nodo->siguiente;
	}
}

void vaciarTablero(Tablero &tablero) {
	NodoFila *nodo = tablero.primeraFila;
	
	while(nodo != nullptr){
		nodo->dato = crearFilaVacia();
		nodo = nodo->siguiente;
	}
}
