#ifndef PIEZAPREVIEWWIDGET_H
#define PIEZAPREVIEWWIDGET_H

#include <QWidget>
#include "Pieza.h"

//Caja pequena que muestra una pieza en su rotacion base.
//Se reutiliza tanto para el "hold" como para cada una de las "siguientes".
class PiezaPreviewWidget : public QWidget {
    Q_OBJECT
public:
    explicit PiezaPreviewWidget(QWidget *padre = nullptr);

    void establecerPieza(TipoPieza tipo, bool vacio);
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *evento) override;

private:
    TipoPieza tipoMostrado;
    bool estaVacio;

    QColor colorPorTipo(int tipo) const;
};

#endif