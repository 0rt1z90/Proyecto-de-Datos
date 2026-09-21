#include "VentanaJuego.h"
#include <allegro5/allegro_primitives.h>
#include <cstdio>

static const float DELTA_TICK = 1.0f / 60.0f;  

static bool botonContienePunto(const Boton &boton, float px, float py) {
	return px >= boton.x && px <= boton.x + boton.ancho && py >= boton.y && py <= boton.y + boton.alto;
}

static void dibujarBoton(VentanaJuego &ventana, const Boton &boton) {
	al_draw_filled_rectangle(boton.x, boton.y, boton.x + boton.ancho, boton.y + boton.alto, al_map_rgb(50, 50, 65));
	al_draw_rectangle(boton.x, boton.y, boton.x + boton.ancho, boton.y + boton.alto, al_map_rgb(90, 90, 110), 1.5f);
	
	int anchoTexto = al_get_text_width(ventana.fuente, boton.texto.c_str());
	float tx = boton.x + (boton.ancho - anchoTexto) / 2.0f;
	float ty = boton.y + boton.alto / 2.0f - 6;
	al_draw_text(ventana.fuente, al_map_rgb(255, 255, 255), tx, ty, 0, boton.texto.c_str());
}

//Revisa el proximo evento programado y arma el aviso
static void actualizarAvisoEvento(VentanaJuego &ventana) {
	std::string mensaje = "";
	
	if(colaEventosVacia(ventana.juego.eventos) == false){
		EventoJuego proximoEvento = verEventoFrente(ventana.juego.eventos);
		float tiempoRestante = proximoEvento.tiempoDisparo - ventana.juego.tiempoTranscurrido;
		
		if(tiempoRestante <= 8.0f && tiempoRestante > 0.0f){
			if(proximoEvento.tipo == EVENTO_AUMENTAR_VELOCIDAD){
				mensaje = "Aviso: la velocidad va a subir pronto";
			}
			if(proximoEvento.tipo == EVENTO_PIEZA_ESPECIAL){
				mensaje = "Aviso: vienen puntos de bono";
			}
			if(proximoEvento.tipo == EVENTO_LIMPIAR_FILA){
				mensaje = "Aviso: se va a eliminar la fila de abajo";
			}
		}
	}
	
	ventana.mensajeAvisoEvento = mensaje;
}

static void actualizarTrasCambioJuego(VentanaJuego &ventana) {
	dibujarPiezaActual(ventana.interfaz, ventana.juego.piezaActual);
	
	float duracionAnimacion = ventana.juego.intervaloCaida * 0.09f;
	if(duracionAnimacion < 0.03f){
		duracionAnimacion = 0.03f;
	}
	establecerDuracionCaidaAnim(ventana.animPieza, duracionAnimacion);
	
	actualizarAvisoEvento(ventana);
}

static void mostrarInfoMovimientoReplay(VentanaJuego &ventana, NodoHistorial *nodo) {
	if(nodo == nullptr){
		ventana.textoInfoReplay = "(antes del primer movimiento)";
		vaciarTablero(ventana.tableroReplay);
		establecerPiezaActualAnim(ventana.animPiezaReplay, ventana.juego.piezaActual, false);
		return;
	}
	
	std::string textoTipo;
	switch(nodo->dato.tipo){
	case MOV_IZQUIERDA: textoTipo = "Izquierda"; break;
	case MOV_DERECHA: textoTipo = "Derecha"; break;
	case MOV_ROTAR: textoTipo = "Rotar"; break;
	case MOV_BAJAR: textoTipo = "Bajar"; break;
	case MOV_COLOCAR: textoTipo = "Colocar"; break;
	}
	
	char buffer[160];
	snprintf(buffer, sizeof(buffer), "%s  ->  fila %d, columna %d, rotacion %d",
			 textoTipo.c_str(), nodo->dato.piezaDespues.filaPivote,
			 nodo->dato.piezaDespues.columnaPivote, nodo->dato.piezaDespues.rotacionActual);
	ventana.textoInfoReplay = buffer;
	
	restaurarTableroDesdeArreglo(ventana.tableroReplay, nodo->dato.tableroDespues);
	establecerPiezaActualAnim(ventana.animPiezaReplay, nodo->dato.piezaDespues, true);
}

static void iniciarNuevaPartida(VentanaJuego &ventana) {
	ventana.juego = crearJuego();
	inicializarJuego(ventana.juego);
	cargarPuntajesDesdeArchivo(ventana.juego.tablaPuntajes, ventana.rutaArchivoPuntajes);
	
	ventana.animPieza = crearAnimacionTablero();
	ventana.pantalla = PANTALLA_JUEGO;
	
	dibujarTablero(ventana.interfaz, ventana.juego.tablero);
	actualizarTrasCambioJuego(ventana);
}

static void entrarReplay(VentanaJuego &ventana) {
	ventana.replayReproduciendo = false;
	ventana.tiempoAcumuladoReplay = 0.0f;
	ventana.animPiezaReplay = crearAnimacionTablero();
	iniciarReplay(ventana.juego);
	ventana.pantalla = PANTALLA_REPLAY;
	mostrarInfoMovimientoReplay(ventana, obtenerMovimientoActual(ventana.juego.historial));
}

static void dibujarPantallaInicioFrame(VentanaJuego &ventana) {
	al_draw_text(ventana.fuenteTitulo, al_map_rgb(255, 255, 255), ANCHO_VENTANA / 2, 140, ALLEGRO_ALIGN_CENTRE, "TETRIS");
	al_draw_text(ventana.fuente, al_map_rgb(170, 170, 170), ANCHO_VENTANA / 2, 210, ALLEGRO_ALIGN_CENTRE,
				 "Flechas: mover / rotar     Espacio: caida rapida     C: hold");
	al_draw_text(ventana.fuente, al_map_rgb(170, 170, 170), ANCHO_VENTANA / 2, 230, ALLEGRO_ALIGN_CENTRE,
				 "Z: deshacer     X: rehacer     ESC: pausa");
	dibujarBoton(ventana, ventana.botonJugar);
}

static void dibujarPantallaJuegoFrame(VentanaJuego &ventana) {
	const int xTablero = 20;
	const int yTablero = 20;
	
	dibujarTableroCompleto(ventana.juego.tablero, ventana.animPieza, xTablero, yTablero);
	
	int xPanel = xTablero + COLUMNAS_TABLERO * TAM_CELDA + 30;
	int y = 20;
	
	al_draw_text(ventana.fuente, al_map_rgb(255, 255, 255), xPanel, y, 0, "SIGUIENTES");
	y += 24;
	
	NodoPieza *nodo = ventana.juego.colaPiezas.frente;
	for(int i = 0; i < 4; i++){
		if(nodo != nullptr){
			dibujarPreviewPieza(nodo->dato.tipo, false, xPanel, y);
			nodo = nodo->siguiente;
		}else{
			dibujarPreviewPieza(PIEZA_I, true, xPanel, y);
		}
		y += 4 * TAM_CELDA_PREVIEW + 8;
	}
	
	y += 12;
	al_draw_text(ventana.fuente, al_map_rgb(255, 255, 255), xPanel, y, 0, "HOLD (tecla C)");
	y += 24;
	
	if(ventana.juego.pilaHold.ocupada == true){
		dibujarPreviewPieza(ventana.juego.pilaHold.piezaGuardada.tipo, false, xPanel, y);
	}else{
		dibujarPreviewPieza(PIEZA_I, true, xPanel, y);
	}
	y += 4 * TAM_CELDA_PREVIEW + 20;
	
	al_draw_textf(ventana.fuente, al_map_rgb(255, 255, 255), xPanel, y, 0, "Puntaje: %d", ventana.juego.puntajeActual);
	y += 28;
	
	if(ventana.mensajeAvisoEvento.empty() == false){
		al_draw_text(ventana.fuente, al_map_rgb(255, 204, 0), xPanel, y, 0, ventana.mensajeAvisoEvento.c_str());
	}
}

static void dibujarPantallaPausaFrame(VentanaJuego &ventana) {
	al_draw_text(ventana.fuenteTitulo, al_map_rgb(255, 255, 255), ANCHO_VENTANA / 2, 220, ALLEGRO_ALIGN_CENTRE, "PAUSA");
	dibujarBoton(ventana, ventana.botonContinuar);
}

static void dibujarPantallaFinFrame(VentanaJuego &ventana) {
	al_draw_text(ventana.fuenteTitulo, al_map_rgb(255, 255, 255), ANCHO_VENTANA / 2, 40, ALLEGRO_ALIGN_CENTRE, "FIN DEL JUEGO");
	al_draw_textf(ventana.fuente, al_map_rgb(255, 255, 255), ANCHO_VENTANA / 2, 90, ALLEGRO_ALIGN_CENTRE, "Puntaje: %d", ventana.juego.puntajeActual);
	
	//Tabla de mejores puntajes
	float tx = ANCHO_VENTANA / 2 - 130;
	float ty = 130;
	al_draw_rectangle(tx, ty, tx + 260, ty + 220, al_map_rgb(90, 90, 110), 1.0f);
	al_draw_text(ventana.fuente, al_map_rgb(200, 200, 200), tx + 10, ty + 6, 0, "Jugador");
	al_draw_text(ventana.fuente, al_map_rgb(200, 200, 200), tx + 180, ty + 6, 0, "Puntos");
	
	for(int i = 0; i < ventana.juego.tablaPuntajes.cantidad && i < 10; i++){
		float fy = ty + 30 + i * 18;
		al_draw_text(ventana.fuente, al_map_rgb(255, 255, 255), tx + 10, fy, 0, ventana.juego.tablaPuntajes.registros[i].nombreJugador.c_str());
		al_draw_textf(ventana.fuente, al_map_rgb(255, 255, 255), tx + 180, fy, 0, "%d", ventana.juego.tablaPuntajes.registros[i].puntos);
	}
	
	dibujarBoton(ventana, ventana.botonOrdenInsercion);
	dibujarBoton(ventana, ventana.botonOrdenQuicksort);
	dibujarBoton(ventana, ventana.botonVerReplay);
	dibujarBoton(ventana, ventana.botonReiniciar);
}

static void dibujarPantallaReplayFrame(VentanaJuego &ventana) {
	al_draw_text(ventana.fuenteTitulo, al_map_rgb(255, 255, 255), ANCHO_VENTANA / 2, 20, ALLEGRO_ALIGN_CENTRE, "REPLAY DE LA PARTIDA");
	
	int xTablero = ANCHO_VENTANA / 2 - (COLUMNAS_TABLERO * TAM_CELDA) / 2;
	int yTablero = 70;
	dibujarTableroCompleto(ventana.tableroReplay, ventana.animPiezaReplay, xTablero, yTablero);
	
	al_draw_text(ventana.fuente, al_map_rgb(204, 204, 204), ANCHO_VENTANA / 2, yTablero + FILAS_TABLERO * TAM_CELDA + 12, ALLEGRO_ALIGN_CENTRE, ventana.textoInfoReplay.c_str());
	
	dibujarBoton(ventana, ventana.botonReplayAtras);
	dibujarBoton(ventana, ventana.botonReplayReproducir);
	dibujarBoton(ventana, ventana.botonReplayPausar);
	dibujarBoton(ventana, ventana.botonReplayAdelante);
	dibujarBoton(ventana, ventana.botonReplayVolver);
}

static void dibujarPantallaIngresoNombreFrame(VentanaJuego &ventana) {
	al_draw_text(ventana.fuenteTitulo, al_map_rgb(255, 255, 255), ANCHO_VENTANA / 2, 220, ALLEGRO_ALIGN_CENTRE, "Nuevo puntaje");
	al_draw_text(ventana.fuente, al_map_rgb(200, 200, 200), ANCHO_VENTANA / 2, 270, ALLEGRO_ALIGN_CENTRE, "Ingresa tu nombre y presiona ENTER:");
	
	float cx = ANCHO_VENTANA / 2 - 120;
	float cy = 300;
	al_draw_filled_rectangle(cx, cy, cx + 240, cy + 30, al_map_rgb(35, 35, 45));
	al_draw_rectangle(cx, cy, cx + 240, cy + 30, al_map_rgb(90, 90, 110), 1.0f);
	al_draw_text(ventana.fuente, al_map_rgb(255, 255, 255), cx + 8, cy + 8, 0, ventana.nombreIngresado.c_str());
}

static void dibujarFrame(VentanaJuego &ventana) {
	al_clear_to_color(al_map_rgb(20, 20, 28));
	
	switch(ventana.pantalla){
	case PANTALLA_INICIO: dibujarPantallaInicioFrame(ventana); break;
	case PANTALLA_JUEGO: dibujarPantallaJuegoFrame(ventana); break;
	case PANTALLA_PAUSA: dibujarPantallaJuegoFrame(ventana); dibujarPantallaPausaFrame(ventana); break;
	case PANTALLA_FIN: dibujarPantallaFinFrame(ventana); break;
	case PANTALLA_REPLAY: dibujarPantallaReplayFrame(ventana); break;
	case PANTALLA_INGRESO_NOMBRE: dibujarPantallaIngresoNombreFrame(ventana); break;
	}
	
	al_flip_display();
}

static void procesarTick(VentanaJuego &ventana) {
	
	if(ventana.pantalla == PANTALLA_JUEGO && ventana.juego.estadoActual == ESTADO_JUGANDO){
		int puntajeAntes = ventana.juego.puntajeActual;
		actualizarJuego(ventana.juego, DELTA_TICK);
		int puntosGanados = ventana.juego.puntajeActual - puntajeAntes;
		
		if(puntosGanados > 0){
			destacarLimpiezaAnim(ventana.animPieza, puntosGanados / 100);
		}
		
		actualizarTrasCambioJuego(ventana);
		
		if(ventana.juego.estadoActual == ESTADO_FIN){
			ventana.pantalla = PANTALLA_INGRESO_NOMBRE;
			ventana.nombreIngresado = "";
		}
	}
	
	if(ventana.pantalla == PANTALLA_REPLAY && ventana.replayReproduciendo == true){
		ventana.tiempoAcumuladoReplay += DELTA_TICK;
		
		if(ventana.tiempoAcumuladoReplay >= 0.5f){
			ventana.tiempoAcumuladoReplay = 0.0f;
			bool huboAvance = avanzarReplay(ventana.juego);
			
			if(huboAvance == false){
				ventana.replayReproduciendo = false;
			}else{
				mostrarInfoMovimientoReplay(ventana, obtenerMovimientoActual(ventana.juego.historial));
			}
		}
	}
	
	actualizarAnimacionTablero(ventana.animPieza, DELTA_TICK);
	actualizarAnimacionTablero(ventana.animPiezaReplay, DELTA_TICK);
}

static void procesarTeclaJuego(VentanaJuego &ventana, int codigoTecla) {
	switch(codigoTecla){
	case ALLEGRO_KEY_LEFT: manejarEntradaJuego(ventana.juego, TECLA_IZQUIERDA); break;
	case ALLEGRO_KEY_RIGHT: manejarEntradaJuego(ventana.juego, TECLA_DERECHA); break;
	case ALLEGRO_KEY_UP: manejarEntradaJuego(ventana.juego, TECLA_ROTAR); break;
	case ALLEGRO_KEY_DOWN: manejarEntradaJuego(ventana.juego, TECLA_BAJAR); break;
	case ALLEGRO_KEY_SPACE: manejarEntradaJuego(ventana.juego, TECLA_CAIDA_RAPIDA); break;
	case ALLEGRO_KEY_C: manejarEntradaJuego(ventana.juego, TECLA_HOLD); break;
	case ALLEGRO_KEY_Z: deshacerJuego(ventana.juego); break;
	case ALLEGRO_KEY_X: rehacerJuego(ventana.juego); break;
	case ALLEGRO_KEY_ESCAPE:
		ventana.juego.estadoActual = ESTADO_PAUSA;
		ventana.pantalla = PANTALLA_PAUSA;
		break;
	default: break;
	}
	
	actualizarTrasCambioJuego(ventana);
}

static void procesarEventoTeclado(VentanaJuego &ventana, const ALLEGRO_EVENT &evento) {
	
	if(ventana.pantalla == PANTALLA_INGRESO_NOMBRE){
		if(evento.type == ALLEGRO_EVENT_KEY_CHAR){
			if(evento.keyboard.keycode == ALLEGRO_KEY_BACKSPACE){
				if(ventana.nombreIngresado.empty() == false){
					ventana.nombreIngresado.pop_back();
				}
			}else if(evento.keyboard.keycode == ALLEGRO_KEY_ENTER || evento.keyboard.keycode == ALLEGRO_KEY_PAD_ENTER){
				std::string nombreFinal = ventana.nombreIngresado;
				if(nombreFinal.empty() == true){
					nombreFinal = "Jugador";
				}
				actualizarNombreUltimoRegistro(ventana.juego.tablaPuntajes, nombreFinal, ventana.juego.puntajeActual);
				mostrarPantallaFin(ventana);
			}else if(evento.keyboard.unichar >= 32 && evento.keyboard.unichar < 127 && ventana.nombreIngresado.size() < 20){
				ventana.nombreIngresado.push_back((char)evento.keyboard.unichar);
			}
		}
		return;
	}
	
	if(evento.type != ALLEGRO_EVENT_KEY_DOWN){
		return;
	}
	
	if(ventana.pantalla == PANTALLA_JUEGO && ventana.juego.estadoActual == ESTADO_JUGANDO){
		procesarTeclaJuego(ventana, evento.keyboard.keycode);
	}
}

static void procesarClick(VentanaJuego &ventana, float x, float y) {
	
	switch(ventana.pantalla){
	case PANTALLA_INICIO:
		if(botonContienePunto(ventana.botonJugar, x, y) == true){
			iniciarNuevaPartida(ventana);
		}
		break;
		
	case PANTALLA_PAUSA:
		if(botonContienePunto(ventana.botonContinuar, x, y) == true){
			ventana.juego.estadoActual = ESTADO_JUGANDO;
			ventana.pantalla = PANTALLA_JUEGO;
		}
		break;
		
	case PANTALLA_FIN:
		if(botonContienePunto(ventana.botonOrdenInsercion, x, y) == true){
			ordenarTabla(ventana.juego.tablaPuntajes, ALGORITMO_INSERCION);
		}else if(botonContienePunto(ventana.botonOrdenQuicksort, x, y) == true){
			ordenarTabla(ventana.juego.tablaPuntajes, ALGORITMO_QUICKSORT);
		}else if(botonContienePunto(ventana.botonVerReplay, x, y) == true){
			entrarReplay(ventana);
		}else if(botonContienePunto(ventana.botonReiniciar, x, y) == true){
			iniciarNuevaPartida(ventana);
		}
		break;
		
	case PANTALLA_REPLAY:
		if(botonContienePunto(ventana.botonReplayAtras, x, y) == true){
			ventana.replayReproduciendo = false;
			retrocederReplay(ventana.juego);
			mostrarInfoMovimientoReplay(ventana, obtenerMovimientoActual(ventana.juego.historial));
		}else if(botonContienePunto(ventana.botonReplayAdelante, x, y) == true){
			ventana.replayReproduciendo = false;
			avanzarReplay(ventana.juego);
			mostrarInfoMovimientoReplay(ventana, obtenerMovimientoActual(ventana.juego.historial));
		}else if(botonContienePunto(ventana.botonReplayReproducir, x, y) == true){
			ventana.replayReproduciendo = true;
			ventana.tiempoAcumuladoReplay = 0.0f;
		}else if(botonContienePunto(ventana.botonReplayPausar, x, y) == true){
			ventana.replayReproduciendo = false;
		}else if(botonContienePunto(ventana.botonReplayVolver, x, y) == true){
			ventana.replayReproduciendo = false;
			ventana.pantalla = PANTALLA_FIN;
		}
		break;
		
	default: break;
	}
}

VentanaJuego crearVentanaJuego() {
	VentanaJuego ventana;
	ventana.display = nullptr;
	ventana.fuente = nullptr;
	ventana.fuenteTitulo = nullptr;
	ventana.colaEventosAllegro = nullptr;
	ventana.timer = nullptr;
	ventana.pantalla = PANTALLA_INICIO;
	ventana.corriendo = false;
	ventana.replayReproduciendo = false;
	ventana.tiempoAcumuladoReplay = 0.0f;
	ventana.textoInfoReplay = "(antes del primer movimiento)";
	return ventana;
}

bool inicializarVentana(VentanaJuego &ventana) {
	
	if(!al_init()){ return false; }
	if(!al_install_keyboard()){ return false; }
	if(!al_install_mouse()){ return false; }
	if(!al_init_font_addon()){ return false; }
	if(!al_init_primitives_addon()){ return false; }
	
	ventana.display = al_create_display(ANCHO_VENTANA, ALTO_VENTANA);
	if(ventana.display == nullptr){ return false; }
	al_set_window_title(ventana.display, "TetrisAllegro");
	
	//Fuente propia de Allegro
	ventana.fuente = al_create_builtin_font();
	ventana.fuenteTitulo = al_create_builtin_font();
	
	ventana.timer = al_create_timer(DELTA_TICK);
	ventana.colaEventosAllegro = al_create_event_queue();
	al_register_event_source(ventana.colaEventosAllegro, al_get_display_event_source(ventana.display));
	al_register_event_source(ventana.colaEventosAllegro, al_get_timer_event_source(ventana.timer));
	al_register_event_source(ventana.colaEventosAllegro, al_get_keyboard_event_source());
	al_register_event_source(ventana.colaEventosAllegro, al_get_mouse_event_source());
	
	ventana.juego = crearJuego();
	ventana.interfaz = crearInterfaz(&ventana);
	ventana.tableroReplay = crearTablero();
	ventana.animPieza = crearAnimacionTablero();
	ventana.animPiezaReplay = crearAnimacionTablero();
	
	ventana.rutaArchivoPuntajes = "puntajes.txt";
	cargarPuntajesDesdeArchivo(ventana.juego.tablaPuntajes, ventana.rutaArchivoPuntajes);
	
	//Botones de cada pantalla
	ventana.botonJugar = { ANCHO_VENTANA / 2.0f - 80, 300, 160, 44, "Jugar" };
	ventana.botonContinuar = { ANCHO_VENTANA / 2.0f - 100, 280, 200, 40, "Continuar (ESC)" };
	
	ventana.botonOrdenInsercion = { ANCHO_VENTANA / 2.0f - 260, 370, 170, 34, "Ordenar (Insercion)" };
	ventana.botonOrdenQuicksort = { ANCHO_VENTANA / 2.0f + 90, 370, 170, 34, "Ordenar (Quicksort)" };
	ventana.botonVerReplay = { ANCHO_VENTANA / 2.0f - 110, 420, 220, 36, "Ver Replay de la partida" };
	ventana.botonReiniciar = { ANCHO_VENTANA / 2.0f - 90, 470, 180, 36, "Jugar de nuevo" };
	
	//Botones del replay 
	ventana.botonReplayAtras = { 50, 280, 140, 34, "< Atras" };
	ventana.botonReplayAdelante = { 50, 330, 140, 34, "Adelante >" };
	ventana.botonReplayReproducir = { 570, 280, 140, 34, "Reproducir" };
	ventana.botonReplayPausar = { 570, 330, 140, 34, "Pausar" };
	ventana.botonReplayVolver = {20, 20, 100, 32, "Volver" };
	
	return true;
}

void ejecutarVentana(VentanaJuego &ventana) {
	al_start_timer(ventana.timer);
	ventana.corriendo = true;
	
	bool necesitaRedibujar = true;
	
	while(ventana.corriendo == true){
		ALLEGRO_EVENT evento;
		al_wait_for_event(ventana.colaEventosAllegro, &evento);
		
		if(evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
			ventana.corriendo = false;
		}
		
		if(evento.type == ALLEGRO_EVENT_TIMER){
			procesarTick(ventana);
			necesitaRedibujar = true;
		}
		
		if(evento.type == ALLEGRO_EVENT_KEY_DOWN || evento.type == ALLEGRO_EVENT_KEY_CHAR){
			procesarEventoTeclado(ventana, evento);
			necesitaRedibujar = true;
		}
		
		if(evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
			procesarClick(ventana, (float)evento.mouse.x, (float)evento.mouse.y);
			necesitaRedibujar = true;
		}
		
		if(necesitaRedibujar == true && al_is_event_queue_empty(ventana.colaEventosAllegro) == true){
			dibujarFrame(ventana);
			necesitaRedibujar = false;
		}
	}
}

void destruirVentana(VentanaJuego &ventana) {
	if(ventana.timer != nullptr){ al_destroy_timer(ventana.timer); }
	if(ventana.colaEventosAllegro != nullptr){ al_destroy_event_queue(ventana.colaEventosAllegro); }
	if(ventana.fuente != nullptr){ al_destroy_font(ventana.fuente); }
	if(ventana.fuenteTitulo != nullptr){ al_destroy_font(ventana.fuenteTitulo); }
	if(ventana.display != nullptr){ al_destroy_display(ventana.display); }
}


void actualizarTablero(VentanaJuego &ventana, const Tablero &tablero) {
	(void)ventana;
	(void)tablero;
}

void actualizarPiezaActual(VentanaJuego &ventana, const Pieza &pieza) {
	establecerPiezaActualAnim(ventana.animPieza, pieza, ventana.juego.estadoActual == ESTADO_JUGANDO);
}

void actualizarSiguientesPiezas(VentanaJuego &ventana, const ColaPiezas &colaPiezas) {
	(void)ventana;
	(void)colaPiezas;
}

void actualizarHold(VentanaJuego &ventana, const PilaHold &pilaHold) {
	(void)ventana;
	(void)pilaHold;
}

void actualizarPuntaje(VentanaJuego &ventana, int puntaje) {
	(void)ventana;
	(void)puntaje;
}

void mostrarPantallaInicio(VentanaJuego &ventana) {
	ventana.pantalla = PANTALLA_INICIO;
}

void mostrarPantallaPausa(VentanaJuego &ventana) {
	ventana.pantalla = PANTALLA_PAUSA;
}

void mostrarPantallaFin(VentanaJuego &ventana) {
	guardarPuntajesEnArchivo(ventana.juego.tablaPuntajes, ventana.rutaArchivoPuntajes);
	ventana.pantalla = PANTALLA_FIN;
}

void mostrarControlesReplay(VentanaJuego &ventana) {
	(void)ventana;
}
