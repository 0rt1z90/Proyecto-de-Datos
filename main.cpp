#include <cstdlib>
#include <ctime>
#include <cstdio>

#include "VentanaJuego.h"

int main() {
	srand((unsigned int)time(nullptr));
	
	VentanaJuego ventana = crearVentanaJuego();
	
	if(inicializarVentana(ventana) == false){
		fprintf(stderr, "No se pudo inicializar Allegro.\n");
		return 1;
	}
	
	ejecutarVentana(ventana);
	destruirVentana(ventana);
	
	return 0;
}
