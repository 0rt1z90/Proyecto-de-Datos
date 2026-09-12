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
    }

    int indiceMinimo = 0;
    for(int i = 1; i < tabla.cantidad; i++){
        if(tabla.registros[i].puntos < tabla.registros[indiceMinimo].puntos){
            indiceMinimo = i;
        }
    }

    return puntos > tabla.registros[indiceMinimo].puntos;
}

void agregarRegistroPuntaje(TablaPuntajes &tabla, RegistroPuntaje registro) {
    if(tabla.cantidad < MAXIMO_PUNTAJES){
        tabla.registros[tabla.cantidad] = registro;
        tabla.cantidad++;
    }else{
        int indiceMinimo = 0;
        for(int i = 1; i < tabla.cantidad; i++){
            if(tabla.registros[i].puntos < tabla.registros[indiceMinimo].puntos){
                indiceMinimo = i;
            }
        }
        tabla.registros[indiceMinimo] = registro;
    }
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

void actualizarNombreUltimoRegistro(TablaPuntajes &tabla, std::string nombreNuevo, int puntos) {

    for(int i = 0; i < tabla.cantidad; i++){
        if(tabla.registros[i].nombreJugador == "Jugador" && tabla.registros[i].puntos == puntos){
            tabla.registros[i].nombreJugador = nombreNuevo;
            return;
        }
    }

}