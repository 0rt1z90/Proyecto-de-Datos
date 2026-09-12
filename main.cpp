#include <QApplication>
#include <ctime>
#include <cstdlib>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    srand((unsigned int)time(nullptr));

    QApplication app(argc, argv);

    MainWindow ventana;
    ventana.show();

    return app.exec();
}