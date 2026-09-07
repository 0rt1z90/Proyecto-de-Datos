#include "TablaPuntajes.h"
#include "Ordenamiento.h"
#include <iostream>
#include <fstream>

TablaPuntajes crearTablaPuntajes() {
    TablaPuntajes tabla;
    tabla.cantidad = 0;
    return tabla;
}

void cargarPuntajesDesdeArchivo(TablaPuntajes &tabla, std::string rutaArchivo) {
    std::ifstream archivo(rutaArchivo);
    tabla.cantidad = 0;

    while(tabla.cantidad < MAXIMO_PUNTAJES && archivo >> tabla.registros[tabla.cantidad].nombreJugador >> tabla.registros[tabla.cantidad].puntos){
        tabla.cantidad++;
    }

    archivo.close();
}

void guardarPuntajesEnArchivo(TablaPuntajes tabla, std::string rutaArchivo) {
    std::ofstream archivo(rutaArchivo);

    for(int i = 0; i < tabla.cantidad; i++){
        archivo << tabla.registros[i].nombreJugador << " " << tabla.registros[i].puntos << std::endl;
    }

    archivo.close();
}

bool calificaParaTabla(TablaPuntajes tabla, int puntos) {
    if(tabla.cantidad < MAXIMO_PUNTAJES){
        return true;
    }else{
        int cantidad = tabla.cantidad;

        if(puntos > tabla.registros[cantidad - 1].puntos ){
            return true;
        }else{
            return false;
        }
    }
}

void agregarRegistroPuntaje(TablaPuntajes &tabla, RegistroPuntaje registro) {
    if(tabla.cantidad < MAXIMO_PUNTAJES){
        tabla.registros[tabla.cantidad] = registro;
        tabla.cantidad++;
    }else{
        tabla.registros[MAXIMO_PUNTAJES - 1] = registro;
    }

    ordenamientoInsercion(tabla.registros, tabla.cantidad);
}

//Reordena la tabla completa con el algoritmo que el usuario elija en la pantalla de fin
void ordenarTabla(TablaPuntajes &tabla, TipoAlgoritmoOrden algoritmo) {
    ordenarTablaPuntajes(tabla.registros, tabla.cantidad, algoritmo);
}

void mostrarTablaPuntajes(TablaPuntajes tabla) {
    for(int i = 0; i < tabla.cantidad; i++){
        std::cout << tabla.registros[i].nombreJugador << " " << tabla.registros[i].puntos << std::endl;
    }
}