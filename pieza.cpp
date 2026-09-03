#include "Pieza.h"

//Tabla de offsets: [tipo][rotacion][bloque][0 es fila, 1 es columna]
//El orden de tipos sigue el enum: I, O, T, S, Z, J, L
const int offsetsPiezas[7][4][4][2] = {

//Pieza I
{
    { {0,0}, {0,1}, {0,2}, {0,3} },
    { {0,0}, {1,0}, {2,0}, {3,0} },
    { {0,0}, {0,1}, {0,2}, {0,3} },
    { {0,0}, {1,0}, {2,0}, {3,0} }
},

    //Pieza O
    {
        { {0,0}, {0,1}, {1,0}, {1,1} },
        { {0,0}, {0,1}, {1,0}, {1,1} },
        { {0,0}, {0,1}, {1,0}, {1,1} },
        { {0,0}, {0,1}, {1,0}, {1,1} }
    },

    //Pieza T
    {
        { {0,0}, {0,1}, {0,2}, {1,1} },
        { {0,0}, {1,0}, {2,0}, {1,1} },
        { {0,1}, {1,0}, {1,1}, {1,2} },
        { {0,1}, {1,1}, {2,1}, {1,0} }
    },

    //Pieza S
    {
        { {0,1}, {0,2}, {1,0}, {1,1} },
        { {0,0}, {1,0}, {1,1}, {2,1} },
        { {0,1}, {0,2}, {1,0}, {1,1} },
        { {0,0}, {1,0}, {1,1}, {2,1} }
    },

    //Pieza Z
    {
        { {0,0}, {0,1}, {1,1}, {1,2} },
        { {0,1}, {1,0}, {1,1}, {2,0} },
        { {0,0}, {0,1}, {1,1}, {1,2} },
        { {0,1}, {1,0}, {1,1}, {2,0} }
    },

    //Pieza J
    {
        { {0,0}, {1,0}, {1,1}, {1,2} },
        { {0,0}, {0,1}, {1,0}, {2,0} },
        { {0,0}, {0,1}, {0,2}, {1,2} },
        { {0,1}, {1,1}, {2,0}, {2,1} }
    },

//Pieza L
{
    { {0,2}, {1,0}, {1,1}, {1,2} },
    { {0,0}, {1,0}, {2,0}, {2,1} },
    { {0,0}, {0,1}, {0,2}, {1,0} },
    { {0,0}, {0,1}, {1,1}, {2,1} }
}
};

Pieza crearPieza(TipoPieza tipo) {
    Pieza pieza;
    pieza.tipo = tipo;
    pieza.rotacionActual = 0;
    pieza.filaPivote = 0;
    pieza.columnaPivote = 3;
    return pieza;
}

void obtenerBloquesPieza(Pieza pieza, Bloque bloques[4]) {
    for(int i = 0; i < 4; i++){
        int desplazamientoFila = offsetsPiezas[pieza.tipo][pieza.rotacionActual][i][0];
        int desplazamientoColumna = offsetsPiezas[pieza.tipo][pieza.rotacionActual][i][1];

        bloques[i].fila = pieza.filaPivote + desplazamientoFila;
        bloques[i].columna = pieza.columnaPivote + desplazamientoColumna;
    }

}

void rotarPieza(Pieza &pieza) {
    pieza.rotacionActual = pieza.rotacionActual + 1;

    if(pieza.rotacionActual == 4){
        pieza.rotacionActual = 0;
    }

}

void moverPiezaIzquierda(Pieza &pieza) {

    pieza.columnaPivote = pieza.columnaPivote - 1;

}

void moverPiezaDerecha(Pieza &pieza) {

    pieza.columnaPivote = pieza.columnaPivote +1;

}

void bajarPieza(Pieza &pieza) {

    pieza.filaPivote = pieza.filaPivote + 1;

}
