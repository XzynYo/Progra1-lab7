#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Lab7.h"
#include "ArchivoContactos.h"

class QtWidgetsApplication1 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication1(QWidget *parent = nullptr);
    ~QtWidgetsApplication1();

private:
    enum class Modo { Registrar, Historial, Editar, Eliminar };

    Ui::QtWidgetsApplication1Class ui;
    ArchivoContactos archivoContactos;
    Modo modoActual = Modo::Registrar;

    void guardarContacto();
    void cambiarModo(Modo modo);
    void irRegistrar();
    void irHistorial();
    void irEditar();
    void irEliminar();
    void cargarTabla();
    void cargarSeleccionEnCampos();
    void actualizarContacto();
    void eliminarContacto();
};
