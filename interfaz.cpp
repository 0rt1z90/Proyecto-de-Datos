#include "Interfaz.h"
#include "VentanaJuego.h"

Interfaz crearInterfaz(VentanaJuego *ventana) {
	Interfaz interfaz;
	interfaz.ventana = ventana;
	return interfaz;
}

void dibujarTablero(Interfaz &interfaz, Tablero tablero) {
	actualizarTablero(*interfaz.ventana, tablero);
}

void dibujarPiezaActual(Interfaz &interfaz, Pieza pieza) {
	actualizarPiezaActual(*interfaz.ventana, pieza);
}

void dibujarSiguientesPiezas(Interfaz &interfaz, ColaPiezas colaPiezas) {
	actualizarSiguientesPiezas(*interfaz.ventana, colaPiezas);
}

void dibujarHold(Interfaz &interfaz, PilaHold pilaHold) {
	actualizarHold(*interfaz.ventana, pilaHold);
}

void dibujarPuntaje(Interfaz &interfaz, int puntaje) {
	actualizarPuntaje(*interfaz.ventana, puntaje);
}

void dibujarPantallaInicio(Interfaz &interfaz) {
	mostrarPantallaInicio(*interfaz.ventana);
}

void dibujarPantallaPausa(Interfaz &interfaz) {
	mostrarPantallaPausa(*interfaz.ventana);
}

void dibujarPantallaFin(Interfaz &interfaz) {
	mostrarPantallaFin(*interfaz.ventana);
}

void dibujarControlesReplay(Interfaz &interfaz) {
	mostrarControlesReplay(*interfaz.ventana);
}
