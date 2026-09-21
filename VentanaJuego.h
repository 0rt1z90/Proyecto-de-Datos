#ifndef VENTANAJUEGO_H
#define VENTANAJUEGO_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <vector>
#include <string>

#include "Juego.h"
#include "Interfaz.h"
#include "Grafico.h"

const int ANCHO_VENTANA = 760;
const int ALTO_VENTANA = 680;

enum PantallaActual {
	PANTALLA_INICIO,
	PANTALLA_JUEGO,
	PANTALLA_PAUSA,
	PANTALLA_FIN,
	PANTALLA_REPLAY,
	PANTALLA_INGRESO_NOMBRE   
};

struct Boton {
	float x, y, ancho, alto;
	std::string texto;
};

struct VentanaJuego {
	ALLEGRO_DISPLAY *display;
	ALLEGRO_FONT *fuente;
	ALLEGRO_FONT *fuenteTitulo;
	ALLEGRO_EVENT_QUEUE *colaEventosAllegro;
	ALLEGRO_TIMER *timer;
	
	Juego juego;
	Interfaz interfaz;
	std::string rutaArchivoPuntajes;

	Tablero tableroReplay;
	
	PantallaActual pantalla;
	bool corriendo;
	
	AnimacionTablero animPieza;
	AnimacionTablero animPiezaReplay;
	
	bool replayReproduciendo;
	float tiempoAcumuladoReplay;
	std::string textoInfoReplay;
	
	std::string nombreIngresado;
	
	std::string mensajeAvisoEvento;
	
	Boton botonJugar;
	Boton botonContinuar;
	Boton botonOrdenInsercion, botonOrdenQuicksort, botonVerReplay, botonReiniciar;
	Boton botonReplayAtras, botonReplayAdelante, botonReplayReproducir, botonReplayPausar, botonReplayVolver;
};

VentanaJuego crearVentanaJuego();
bool inicializarVentana(VentanaJuego &ventana);
void ejecutarVentana(VentanaJuego &ventana);
void destruirVentana(VentanaJuego &ventana);

//Puente Interfaz -> Allegro 
void actualizarTablero(VentanaJuego &ventana, const Tablero &tablero);
void actualizarPiezaActual(VentanaJuego &ventana, const Pieza &pieza);
void actualizarSiguientesPiezas(VentanaJuego &ventana, const ColaPiezas &colaPiezas);
void actualizarHold(VentanaJuego &ventana, const PilaHold &pilaHold);
void actualizarPuntaje(VentanaJuego &ventana, int puntaje);
void mostrarPantallaInicio(VentanaJuego &ventana);
void mostrarPantallaPausa(VentanaJuego &ventana);
void mostrarPantallaFin(VentanaJuego &ventana);
void mostrarControlesReplay(VentanaJuego &ventana);

#endif
