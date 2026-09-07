#include "Ordenamiento.h"

void ordenamientoInsercion(RegistroPuntaje arreglo[], int cantidad) {

    for(int i = 1; i < cantidad; i++){
        RegistroPuntaje aux = arreglo[i];
        int j = i -1;
        while(j >= 0 && arreglo[j].puntos > aux.puntos){
            arreglo[j + 1] = arreglo[j];
            j--;
        }
        arreglo[j + 1] = aux;
    }

}

void ordenamientoQuicksort(RegistroPuntaje arreglo[], int inicio, int fin) {

    if (inicio < fin) {

        int pivote = particionarQuicksort(arreglo, inicio, fin);

        ordenamientoQuicksort(arreglo, inicio, pivote - 1);
        ordenamientoQuicksort(arreglo, pivote + 1, fin);
    }
}

int particionarQuicksort(RegistroPuntaje arreglo[], int inicio, int fin) {
    int pivote = arreglo[fin].puntos;
    int i = inicio - 1;

    for(int j = inicio; j < fin; j++){
        if(arreglo[j].puntos >= pivote){
            i++;

            RegistroPuntaje aux = arreglo[i];
            arreglo[i] = arreglo[j];
            arreglo[j] = aux;
        }
    }

    RegistroPuntaje aux = arreglo[i + 1];
    arreglo[i + 1] = arreglo[fin];
    arreglo[fin] = aux;

    return i + 1;
}

//Punto de entrada unico para elegir con que algoritmo se ordena la tabla
void ordenarTablaPuntajes(RegistroPuntaje arreglo[], int cantidad, TipoAlgoritmoOrden algoritmo) {

    if(algoritmo == ALGORITMO_INSERCION){
        ordenamientoInsercion(arreglo, cantidad);
    }

    if(algoritmo == ALGORITMO_QUICKSORT){
        ordenamientoQuicksort(arreglo, 0, cantidad - 1);
    }

}