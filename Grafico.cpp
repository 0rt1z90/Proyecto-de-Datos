#include "Grafico.h"
#include <allegro5/allegro_primitives.h>

AnimacionTablero crearAnimacionTablero() {
	AnimacionTablero anim;
	anim.tienePieza = false;
	anim.filaAnimada = 0.0f;
	anim.columnaAnimada = 3.0f;
	anim.filaOrigen = 0.0f;
	anim.columnaOrigen = 3.0f;
	anim.filaObjetivo = 0.0f;
	anim.columnaObjetivo = 3.0f;
	anim.tiempoTranscurridoAnim = 0.0f;
	anim.duracionAnimacion = 0.09f;
	anim.animando = false;
	anim.opacidadDestello = 0.0f;
	anim.tiempoDestello = 0.0f;
	anim.destellando = false;
	return anim;
}

void establecerPiezaActualAnim(AnimacionTablero &anim, const Pieza &pieza, bool hayPieza) {
	
	if(hayPieza == false){
		anim.tienePieza = false;
		return;
	}
	
	bool esPiezaNueva = (anim.tienePieza == false) || (anim.piezaMostrada.tipo != pieza.tipo && anim.piezaMostrada.filaPivote == 0);
	
	float filaOrigen = anim.filaAnimada;
	float columnaOrigen = anim.columnaAnimada;
	
	if(esPiezaNueva){
		filaOrigen = (float)pieza.filaPivote;
		columnaOrigen = (float)pieza.columnaPivote;
	}
	
	anim.piezaMostrada = pieza;
	anim.tienePieza = true;
	
	anim.filaOrigen = filaOrigen;
	anim.columnaOrigen = columnaOrigen;
	anim.filaObjetivo = (float)pieza.filaPivote;
	anim.columnaObjetivo = (float)pieza.columnaPivote;
	anim.tiempoTranscurridoAnim = 0.0f;
	anim.animando = true;
	
	if(esPiezaNueva){
		anim.filaAnimada = filaOrigen;
		anim.columnaAnimada = columnaOrigen;
	}
}

void establecerDuracionCaidaAnim(AnimacionTablero &anim, float segundosDuracion) {
	anim.duracionAnimacion = segundosDuracion;
}

void destacarLimpiezaAnim(AnimacionTablero &anim, int cantidadFilas) {
	if(cantidadFilas <= 0){
		return;
	}
	anim.destellando = true;
	anim.tiempoDestello = 0.0f;
	anim.opacidadDestello = 0.75f;
}

void actualizarAnimacionTablero(AnimacionTablero &anim, float deltaTiempo) {
	
	if(anim.animando == true){
		anim.tiempoTranscurridoAnim = anim.tiempoTranscurridoAnim + deltaTiempo;
		
		float progreso = 1.0f;
		if(anim.duracionAnimacion > 0.0f){
			progreso = anim.tiempoTranscurridoAnim / anim.duracionAnimacion;
		}
		if(progreso >= 1.0f){
			progreso = 1.0f;
			anim.animando = false;
		}
		
		float progresoSuave = 1.0f - (1.0f - progreso) * (1.0f - progreso);
		
		anim.filaAnimada = anim.filaOrigen + (anim.filaObjetivo - anim.filaOrigen) * progresoSuave;
		anim.columnaAnimada = anim.columnaOrigen + (anim.columnaObjetivo - anim.columnaOrigen) * progresoSuave;
	}
	
	if(anim.destellando == true){
		anim.tiempoDestello = anim.tiempoDestello + deltaTiempo;
		
		float progresoDestello = anim.tiempoDestello / 0.35f;
		if(progresoDestello >= 1.0f){
			progresoDestello = 1.0f;
			anim.destellando = false;
		}
		
		anim.opacidadDestello = 0.75f * (1.0f - progresoDestello);
	}
}

ALLEGRO_COLOR colorPorTipo(int tipo) {
	switch(tipo){
	case PIEZA_I: return al_map_rgb(0, 188, 212);
	case PIEZA_O: return al_map_rgb(255, 193, 7);
	case PIEZA_T: return al_map_rgb(156, 39, 176);
	case PIEZA_S: return al_map_rgb(76, 175, 80);
	case PIEZA_Z: return al_map_rgb(244, 67, 54);
	case PIEZA_J: return al_map_rgb(33, 150, 243);
	case PIEZA_L: return al_map_rgb(255, 152, 0);
	default: return al_map_rgb(60, 60, 70);
	}
}

void dibujarTableroCompleto(const Tablero &tablero, const AnimacionTablero &anim, int xOrigen, int yOrigen) {
	
	//Fondo
	al_draw_filled_rectangle(xOrigen, yOrigen, xOrigen + COLUMNAS_TABLERO * TAM_CELDA, yOrigen + FILAS_TABLERO * TAM_CELDA, al_map_rgb(20, 20, 28));
	
	//Cuadricula
	ALLEGRO_COLOR colorGrilla = al_map_rgb(45, 45, 55);
	for(int columna = 0; columna <= COLUMNAS_TABLERO; columna++){
		al_draw_line(xOrigen + columna * TAM_CELDA, yOrigen, xOrigen + columna * TAM_CELDA, yOrigen + FILAS_TABLERO * TAM_CELDA, colorGrilla, 1.0f);
	}
	for(int fila = 0; fila <= FILAS_TABLERO; fila++){
		al_draw_line(xOrigen, yOrigen + fila * TAM_CELDA, xOrigen + COLUMNAS_TABLERO * TAM_CELDA, yOrigen + fila * TAM_CELDA, colorGrilla, 1.0f);
	}
	
	//Celdas fijas del tablero
	for(int fila = 0; fila < FILAS_TABLERO; fila++){
		NodoFila *nodo = obtenerNodoFila(tablero, fila);
		
		if(nodo == nullptr){
			continue;
		}
		
		for(int columna = 0; columna < COLUMNAS_TABLERO; columna++){
			if(nodo->dato.celdas[columna].ocupada == true){
				float x1 = xOrigen + columna * TAM_CELDA + 1;
				float y1 = yOrigen + fila * TAM_CELDA + 1;
				al_draw_filled_rectangle(x1, y1, x1 + TAM_CELDA - 2, y1 + TAM_CELDA - 2, colorPorTipo(nodo->dato.celdas[columna].colorPieza));
			}
		}
	}
	
	//Pieza actual
	if(anim.tienePieza == true){
		Bloque bloques[4];
		obtenerBloquesPieza(anim.piezaMostrada, bloques);
		
		for(int i = 0; i < 4; i++){
			int desplazFila = bloques[i].fila - anim.piezaMostrada.filaPivote;
			int desplazColumna = bloques[i].columna - anim.piezaMostrada.columnaPivote;
			
			int filaDibujo = (int)(anim.filaAnimada + desplazFila + 0.5f);
			int columnaDibujo = (int)(anim.columnaAnimada + desplazColumna + 0.5f);
			
			if(filaDibujo >= 0 && filaDibujo < FILAS_TABLERO){
				float x1 = xOrigen + columnaDibujo * TAM_CELDA + 1;
				float y1 = yOrigen + filaDibujo * TAM_CELDA + 1;
				al_draw_filled_rectangle(x1, y1, x1 + TAM_CELDA - 2, y1 + TAM_CELDA - 2, colorPorTipo(anim.piezaMostrada.tipo));
			}
		}
	}
	
	if(anim.opacidadDestello > 0.0f){
		ALLEGRO_COLOR blanco = al_map_rgba_f(anim.opacidadDestello, anim.opacidadDestello, anim.opacidadDestello, anim.opacidadDestello);
		al_draw_filled_rectangle(xOrigen, yOrigen, xOrigen + COLUMNAS_TABLERO * TAM_CELDA, yOrigen + FILAS_TABLERO * TAM_CELDA, blanco);
	}
}

void dibujarPreviewPieza(TipoPieza tipo, bool vacio, int x, int y) {
	const int lado = 4 * TAM_CELDA_PREVIEW;
	
	al_draw_filled_rectangle(x, y, x + lado, y + lado, al_map_rgb(30, 30, 40));
	al_draw_rectangle(x + 0.5f, y + 0.5f, x + lado - 0.5f, y + lado - 0.5f, al_map_rgb(55, 55, 65), 1.0f);
	
	if(vacio == true){
		return;
	}
	
	Pieza piezaTemporal = crearPieza(tipo);
	piezaTemporal.filaPivote = 0;
	piezaTemporal.columnaPivote = 0;
	
	Bloque bloques[4];
	obtenerBloquesPieza(piezaTemporal, bloques);
	
	int filaMinima = bloques[0].fila;
	int columnaMinima = bloques[0].columna;
	int filaMaxima = bloques[0].fila;
	int columnaMaxima = bloques[0].columna;
	
	for(int i = 1; i < 4; i++){
		if(bloques[i].fila < filaMinima){ filaMinima = bloques[i].fila; }
		if(bloques[i].fila > filaMaxima){ filaMaxima = bloques[i].fila; }
		if(bloques[i].columna < columnaMinima){ columnaMinima = bloques[i].columna; }
		if(bloques[i].columna > columnaMaxima){ columnaMaxima = bloques[i].columna; }
	}
	
	int anchoFigura = (columnaMaxima - columnaMinima + 1) * TAM_CELDA_PREVIEW;
	int altoFigura = (filaMaxima - filaMinima + 1) * TAM_CELDA_PREVIEW;
	int desplazX = x + (lado - anchoFigura) / 2;
	int desplazY = y + (lado - altoFigura) / 2;
	
	for(int i = 0; i < 4; i++){
		int bx = desplazX + (bloques[i].columna - columnaMinima) * TAM_CELDA_PREVIEW;
		int by = desplazY + (bloques[i].fila - filaMinima) * TAM_CELDA_PREVIEW;
		al_draw_filled_rectangle(bx + 1, by + 1, bx + TAM_CELDA_PREVIEW - 1, by + TAM_CELDA_PREVIEW - 1, colorPorTipo(tipo));
	}
}
