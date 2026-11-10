#pragma once
#include <QString>
#include <QStringList>

class ArchivoContactos
{
public:
    QString ruta() const;
    bool guardar(const QString& nombre,
        const QString& telefono,
        const QString& correo) const;
    QStringList leer() const;
    bool actualizar(int indice,
        const QString& nombre,
        const QString& telefono,
        const QString& correo) const;
    bool eliminar(int indice) const;

private:
    bool reescribir(const QStringList& lineas) const;
};
