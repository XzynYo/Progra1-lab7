#include "Lab7.h"
#include "ArchivoContactos.h"
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QStringList>
#include <QAction>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>

QtWidgetsApplication1::QtWidgetsApplication1(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    ui.tblContactos->horizontalHeader()->setStretchLastSection(true);
    ui.tblContactos->setColumnWidth(0, 220);
    ui.tblContactos->setColumnWidth(1, 140);

    connect(ui.btnGuardar, &QPushButton::clicked,
        this, &QtWidgetsApplication1::guardarContacto);
    connect(ui.actionRegistrar, &QAction::triggered,
        this, &QtWidgetsApplication1::irRegistrar);
    connect(ui.actionHistorial, &QAction::triggered,
        this, &QtWidgetsApplication1::irHistorial);
    connect(ui.actionEditar, &QAction::triggered,
        this, &QtWidgetsApplication1::irEditar);
    connect(ui.actionEliminar, &QAction::triggered,
        this, &QtWidgetsApplication1::irEliminar);
    connect(ui.btnActualizar, &QPushButton::clicked,
        this, &QtWidgetsApplication1::actualizarContacto);
    connect(ui.btnEliminar, &QPushButton::clicked,
        this, &QtWidgetsApplication1::eliminarContacto);
    connect(ui.tblContactos, &QTableWidget::itemSelectionChanged,
        this, &QtWidgetsApplication1::cargarSeleccionEnCampos);

    cambiarModo(Modo::Registrar);
}

QtWidgetsApplication1::~QtWidgetsApplication1()
{}

void QtWidgetsApplication1::guardarContacto()
{
    QString nombre = ui.txtNombre->text().trimmed();
    QString telefono = ui.txtTelefono->text().trimmed();
    QString correo = ui.txtCorreo->text().trimmed();

    if (nombre.isEmpty() || telefono.isEmpty() || correo.isEmpty()) {
        QMessageBox::warning(this, "Datos incompletos",
                             "Completá los tres campos.");
        return;
    }

    for (const QString& dato : QStringList{nombre, telefono, correo}) {
        if (dato.contains(';') || dato.contains('\n') ||
            dato.contains('\r')) {
            QMessageBox::warning(this, "Dato no permitido",
                "No usés punto y coma ni saltos de línea.");
            return;
        }
    }

    if (!archivoContactos.guardar(nombre, telefono, correo)) {
        QMessageBox::critical(this, "No se pudo guardar",
            "Revisá permisos y espacio disponible.\nArchivo: "
            + archivoContactos.ruta());
        return;
    }

    ui.txtNombre->clear();
    ui.txtTelefono->clear();
    ui.txtCorreo->clear();
    QMessageBox::information(this, "Guardado completo",
        "El contacto se guardó correctamente.\nArchivo: "
        + archivoContactos.ruta());
    ui.txtNombre->setFocus();
}

void QtWidgetsApplication1::cambiarModo(Modo modo)
{
    modoActual = modo;
    ui.paginas->setCurrentIndex(modo == Modo::Registrar ? 0 : 1);
    ui.grpEditar->setVisible(modo == Modo::Editar);
    ui.btnEliminar->setVisible(modo == Modo::Eliminar);

    switch (modo) {
    case Modo::Registrar:
        ui.txtNombre->setFocus();
        break;
    case Modo::Historial:
        ui.lblModo->setText("Historial de contactos");
        break;
    case Modo::Editar:
        ui.lblModo->setText("Editar: seleccioná una fila, cambiá los campos y presioná Actualizar.");
        break;
    case Modo::Eliminar:
        ui.lblModo->setText("Eliminar: seleccioná una fila y presioná el botón Eliminar.");
        break;
    }

    if (modo != Modo::Registrar)
        cargarTabla();
}

void QtWidgetsApplication1::irRegistrar()
{
    cambiarModo(Modo::Registrar);
}

void QtWidgetsApplication1::irHistorial()
{
    cambiarModo(Modo::Historial);
}

void QtWidgetsApplication1::irEditar()
{
    cambiarModo(Modo::Editar);
}

void QtWidgetsApplication1::irEliminar()
{
    cambiarModo(Modo::Eliminar);
}

void QtWidgetsApplication1::cargarTabla()
{
    ui.tblContactos->setRowCount(0);
    for (const QString& linea : archivoContactos.leer()) {
        QStringList partes = linea.split(';');
        if (partes.size() != 3)
            continue;
        int fila = ui.tblContactos->rowCount();
        ui.tblContactos->insertRow(fila);
        for (int columna = 0; columna < 3; ++columna) {
            QTableWidgetItem* celda = new QTableWidgetItem(partes.at(columna));
            celda->setFlags(celda->flags() & ~Qt::ItemIsEditable);
            ui.tblContactos->setItem(fila, columna, celda);
        }
    }
    ui.tblContactos->clearSelection();
    ui.txtEditNombre->clear();
    ui.txtEditTelefono->clear();
    ui.txtEditCorreo->clear();
}

void QtWidgetsApplication1::cargarSeleccionEnCampos()
{
    if (modoActual != Modo::Editar)
        return;
    int fila = ui.tblContactos->currentRow();
    if (fila < 0)
        return;
    ui.txtEditNombre->setText(ui.tblContactos->item(fila, 0)->text());
    ui.txtEditTelefono->setText(ui.tblContactos->item(fila, 1)->text());
    ui.txtEditCorreo->setText(ui.tblContactos->item(fila, 2)->text());
}

void QtWidgetsApplication1::actualizarContacto()
{
    if (modoActual != Modo::Editar)
        return;
    int fila = ui.tblContactos->currentRow();
    if (fila < 0) {
        QMessageBox::warning(this, "Sin selección",
            "Seleccioná primero el contacto que querés editar.");
        return;
    }

    QString nombre = ui.txtEditNombre->text().trimmed();
    QString telefono = ui.txtEditTelefono->text().trimmed();
    QString correo = ui.txtEditCorreo->text().trimmed();

    if (nombre.isEmpty() || telefono.isEmpty() || correo.isEmpty()) {
        QMessageBox::warning(this, "Datos incompletos",
                             "Completá los tres campos.");
        return;
    }

    for (const QString& dato : QStringList{nombre, telefono, correo}) {
        if (dato.contains(';') || dato.contains('\n') ||
            dato.contains('\r')) {
            QMessageBox::warning(this, "Dato no permitido",
                "No usés punto y coma ni saltos de línea.");
            return;
        }
    }

    if (!archivoContactos.actualizar(fila, nombre, telefono, correo)) {
        QMessageBox::critical(this, "No se pudo actualizar",
            "Revisá permisos y espacio disponible.\nArchivo: "
            + archivoContactos.ruta());
        return;
    }

    QMessageBox::information(this, "Actualización completa",
        "El contacto se actualizó correctamente.\nArchivo: "
        + archivoContactos.ruta());
    cargarTabla();
}

void QtWidgetsApplication1::eliminarContacto()
{
    if (modoActual != Modo::Eliminar)
        return;
    int fila = ui.tblContactos->currentRow();
    if (fila < 0) {
        QMessageBox::warning(this, "Sin selección",
            "Seleccioná primero el contacto que querés eliminar.");
        return;
    }

    QString nombre = ui.tblContactos->item(fila, 0)->text();
    QMessageBox::StandardButton respuesta =
        QMessageBox::question(this, "Confirmar eliminación",
            "¿Eliminar definitivamente a " + nombre + "?",
            QMessageBox::Yes | QMessageBox::No);
    if (respuesta != QMessageBox::Yes)
        return;

    if (!archivoContactos.eliminar(fila)) {
        QMessageBox::critical(this, "No se pudo eliminar",
            "Revisá permisos y espacio disponible.\nArchivo: "
            + archivoContactos.ruta());
        return;
    }

    QMessageBox::information(this, "Eliminación completa",
        "El contacto se eliminó correctamente.\nArchivo: "
        + archivoContactos.ruta());
    cargarTabla();
}
