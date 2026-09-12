cmake_minimum_required(VERSION 3.19)
project(TetrisQT LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(TetrisQT
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    Celda.h
    Fila.h
    Fila.cpp
    Pieza.h
    Pieza.cpp
    ColaPiezas.h
    ColaPiezas.cpp
    ColaEventos.h
    ColaEventos.cpp
    EventoJuego.h
    Tablero.h
    Tablero.cpp
    Movimiento.h
    ListaHistorial.h
    ListaHistorial.cpp
    PilaHold.h
    PilaHold.cpp
    RegistroPuntaje.h
    TablaPuntajes.h
    TablaPuntajes.cpp
    Ordenamiento.h
    Ordenamiento.cpp
    Juego.h
    Juego.cpp
    Interfaz.h
    Interfaz.cpp
    tablerowidget.h tablerowidget.cpp
    piezapreviewwidget.h piezapreviewwidget.cpp

)

target_link_libraries(TetrisQT
    PRIVATE
        Qt::Core
        Qt::Widgets
)

install(TARGETS TetrisQT
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET TetrisQT
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
