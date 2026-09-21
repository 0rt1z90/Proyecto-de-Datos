#ifndef JUEGO_H
#define JUEGO_H

#include "ColaPiezas.h"
#include "PilaHold.h"
#include "ListaHistorial.h"
#include "ColaEventos.h"
#include "Tablero.h"
#include "TablaPuntajes.h"
#include "Pieza.h"

enum EstadoJuego { ESTADO_INICIO, ESTADO_JUGANDO, ESTADO_PAUSA, ESTADO_FIN, ESTADO_REPLAY };

enum TeclaJuego { TECLA_IZQUIERDA, TECLA_DERECHA, TECLA_ROTAR, TECLA_BAJAR, TECLA_CAIDA_RAPIDA, TECLA_HOLD };

//Coordina todas las estructuras y la logica general de la partida
struct Juego {
	Tablero tablero;
	ColaPiezas colaPiezas;
	PilaHold pilaHold;
	ListaHistorial historial;
	ColaEventos eventos;
	TablaPuntajes tablaPuntajes;
	
	Pieza piezaActual;
	EstadoJuego estadoActual;
	int puntajeActual;
	float tiempoTranscurrido;
	float intervaloCaida;
};

Juego crearJuego();
void inicializarJuego(Juego &juego);
void actualizarJuego(Juego &juego, float deltaTiempo);
void manejarEntradaJuego(Juego &juego, int tecla);
void generarNuevaPiezaActual(Juego &juego);
void enviarPiezaAHold(Juego &juego);
void procesarEventosProgramados(Juego &juego);
void registrarMovimientoActual(Juego &juego, TipoMovimiento tipo, Pieza piezaAntes, Fila tableroAntes[FILAS_TABLERO]);
void finalizarPartida(Juego &juego);
void iniciarReplay(Juego &juego);
bool avanzarReplay(Juego &juego);
void retrocederReplay(Juego &juego);
bool deshacerJuego(Juego &juego);
bool rehacerJuego(Juego &juego);

#endif
