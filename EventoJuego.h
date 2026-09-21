#ifndef EVENTOJUEGO_H
#define EVENTOJUEGO_H

//Tipos de evento programado dentro de la partida
enum TipoEvento { EVENTO_AUMENTAR_VELOCIDAD, EVENTO_PIEZA_ESPECIAL, EVENTO_LIMPIAR_FILA };

struct EventoJuego {
	TipoEvento tipo;
	float tiempoDisparo;
};

#endif
