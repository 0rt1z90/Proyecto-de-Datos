#ifndef GRAFICO_H
#define GRAFICO_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#include "Tablero.h"
#include "Pieza.h"

const int TAM_CELDA = 28;
const int TAM_CELDA_PREVIEW = 18;

struct AnimacionTablero {
	Pieza piezaMostrada;
	bool tienePieza;
	
	float filaAnimada;
	float columnaAnimada;
	float filaOrigen;
	float columnaOrigen;
	float filaObjetivo;
	float columnaObjetivo;
	float tiempoTranscurridoAnim;
	float duracionAnimacion;   //segundos 
	bool animando;
	
	float opacidadDestello;    
	float tiempoDestello;
	bool destellando;
};

AnimacionTablero crearAnimacionTablero();

void establecerPiezaActualAnim(AnimacionTablero &anim, const Pieza &pieza, bool hayPieza);
void establecerDuracionCaidaAnim(AnimacionTablero &anim, float segundosDuracion);
void destacarLimpiezaAnim(AnimacionTablero &anim, int cantidadFilas);

void actualizarAnimacionTablero(AnimacionTablero &anim, float deltaTiempo);

ALLEGRO_COLOR colorPorTipo(int tipo);

void dibujarTableroCompleto(const Tablero &tablero, const AnimacionTablero &anim, int xOrigen, int yOrigen);

void dibujarPreviewPieza(TipoPieza tipo, bool vacio, int x, int y);

#endif
