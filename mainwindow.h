#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QTimer>
#include <vector>

#include "Juego.h"
#include "Interfaz.h"
#include "TableroWidget.h"
#include "PiezaPreviewWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *padre = nullptr);

    void actualizarTablero(const Tablero &tablero);
    void actualizarPiezaActual(const Pieza &pieza);
    void actualizarSiguientesPiezas(const ColaPiezas &colaPiezas);
    void actualizarHold(const PilaHold &pilaHold);
    void actualizarPuntaje(int puntaje);
    void mostrarPantallaInicio();
    void mostrarPantallaPausa();
    void mostrarPantallaFin();
    void mostrarControlesReplay();

protected:
    void keyPressEvent(QKeyEvent *evento) override;

private slots:
    void alPresionarJugar();
    void alTickJuego();
    void alPresionarContinuar();
    void alPresionarVerReplay();
    void alPresionarReplayAtras();
    void alPresionarReplayAdelante();
    void alPresionarReplayReproducir();
    void alPresionarReplayPausar();
    void alPresionarReplayVolver();
    void alTickReplay();
    void alPresionarReiniciar();
    void alPresionarOrdenInsercion();
    void alPresionarOrdenQuicksort();

private:
    Juego juego;
    Interfaz interfaz;
    QTimer timerJuego;
    std::string rutaArchivoPuntajes;

    //Tablero reutilizado solo para dibujar los pasos del replay
    Tablero tableroReplay;

    QStackedWidget *pilaDePantallas;

    QWidget *pantallaInicio;
    QPushButton *botonJugar;

    QWidget *pantallaJuego;
    TableroWidget *widgetTablero;
    PiezaPreviewWidget *widgetHold;
    std::vector<PiezaPreviewWidget*> widgetsSiguientes;
    QLabel *etiquetaPuntaje;
    QLabel *etiquetaAvisoEvento;



    QWidget *pantallaPausa;

    QWidget *pantallaFin;
    QLabel *etiquetaPuntajeFinal;
    QTableWidget *tablaMejoresPuntajes;
    QPushButton *botonReiniciar;
    QPushButton *botonOrdenInsercion;
    QPushButton *botonOrdenQuicksort;
    QPushButton *botonVerReplay;

    //Pantalla nueva dedicada al replay de la partida
    QWidget *pantallaReplay;
    TableroWidget *widgetTableroReplay;
    QLabel *etiquetaInfoReplay;
    QPushButton *botonReplayAtras;
    QPushButton *botonReplayAdelante;
    QPushButton *botonReplayReproducir;
    QPushButton *botonReplayPausar;
    QPushButton *botonReplayVolver;
    QTimer timerReplay;

    void construirPantallaInicio();
    void construirPantallaJuego();
    void construirPantallaPausa();
    void construirPantallaFin();
    void construirPantallaReplay();
    void refrescarTablaMejoresPuntajes();
    void refrescarPantallaJuego();
    void actualizarAvisoEvento();
    void mostrarInfoMovimientoReplay(NodoHistorial *nodo);
};

#endif