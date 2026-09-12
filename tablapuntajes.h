#ifndef TABLAPUNTAJES_H
#define TABLAPUNTAJES_H

#include "RegistroPuntaje.h"
#include "Ordenamiento.h"

const int MAXIMO_PUNTAJES = 10;

struct TablaPuntajes {
    RegistroPuntaje registros[MAXIMO_PUNTAJES];
    int cantidad;
};

TablaPuntajes crearTablaPuntajes();
void cargarPuntajesDesdeArchivo(TablaPuntajes &tabla, std::string rutaArchivo);
void guardarPuntajesEnArchivo(TablaPuntajes tabla, std::string rutaArchivo);
bool calificaParaTabla(TablaPuntajes tabla, int puntos);
void agregarRegistroPuntaje(TablaPuntajes &tabla, RegistroPuntaje registro);
void ordenarTabla(TablaPuntajes &tabla, TipoAlgoritmoOrden algoritmo);
void mostrarTablaPuntajes(TablaPuntajes tabla);
void actualizarNombreUltimoRegistro(TablaPuntajes &tabla, std::string nombreNuevo, int puntos);

#endif