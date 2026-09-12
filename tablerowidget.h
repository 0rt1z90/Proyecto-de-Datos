#ifndef TABLEROWIDGET_H
#define TABLEROWIDGET_H

#include <QWidget>
#include <QVariantAnimation>
#include "Tablero.h"
#include "Pieza.h"

const int TAM_CELDA = 28;

//Dibuja el tablero fijo y la pieza que esta cayendo.
//Anima el deslizamiento de la pieza entre posiciones (caida, movimiento
//lateral, rotacion) y un destello blanco cuando se limpian filas.
class TableroWidget : public QWidget {
    Q_OBJECT
public:
    explicit TableroWidget(QWidget *padre = nullptr);

    void establecerTablero(const Tablero &tablero);
    void establecerPiezaActual(const Pieza &pieza, bool hayPieza);
    void establecerDuracionCaida(int milisegundos);
    void destacarLimpieza(int cantidadFilas);

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *evento) override;

private:
    Tablero tableroActual;
    bool tableroValido;

    Pieza piezaMostrada;
    bool tienePieza;

    //Posicion animada (en celdas, con decimales) de la pieza actual
    float filaAnimadaActual;
    float columnaAnimadaActual;
    QVariantAnimation animacionCaida;

    //Destello al limpiar filas
    qreal opacidadDestello;
    QVariantAnimation animacionDestello;

    QColor colorPorTipo(int tipo) const;
};

#endif