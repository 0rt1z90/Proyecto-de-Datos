#ifndef ORDENAMIENTO_H
#define ORDENAMIENTO_H

#include "RegistroPuntaje.h"

enum TipoAlgoritmoOrden { ALGORITMO_INSERCION, ALGORITMO_QUICKSORT };

void ordenamientoInsercion(RegistroPuntaje arreglo[], int cantidad);
void ordenamientoQuicksort(RegistroPuntaje arreglo[], int inicio, int fin);
int particionarQuicksort(RegistroPuntaje arreglo[], int inicio, int fin);
void ordenarTablaPuntajes(RegistroPuntaje arreglo[], int cantidad, TipoAlgoritmoOrden algoritmo);

#endif
