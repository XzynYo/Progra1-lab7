#include "ArchivoContactos.h"
#include <QFile>
#include <QDir>
#include <QByteArray>
#include <QSaveFile>

QString ArchivoContactos::ruta() const
{
    return QDir::homePath() + "/Lab7Datos/contactos.txt";
}

bool ArchivoContactos::guardar(const QString& nombre,
    const QString& telefono,
    const QString& correo) const
{
    QString carpeta = QDir::homePath() + "/Lab7Datos";
    if (!QDir().mkpath(carpeta))
        return false;

    QFile archivo(ruta());
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Append))
        return false;

    QString linea = nombre + ";" + telefono + ";" + correo + "\n";
    QByteArray datos = linea.toUtf8();
    bool completo = archivo.write(datos) == datos.size();
    bool vaciado = archivo.flush();
    archivo.close();
    return completo && vaciado;
}

QStringList ArchivoContactos::leer() const
{
    QStringList contactos;
    QFile archivo(ruta());
    if (!archivo.exists())
        return contactos;
    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text))
        return contactos;
    while (!archivo.atEnd()) {
        QString linea = QString::fromUtf8(archivo.readLine()).trimmed();
        if (linea.isEmpty())
            continue;
        if (linea.count(';') != 2)
            continue;
        contactos.append(linea);
    }
    archivo.close();
    return contactos;
}

bool ArchivoContactos::actualizar(int indice,
    const QString& nombre,
    const QString& telefono,
    const QString& correo) const
{
    QStringList lineas = leer();
    if (indice < 0 || indice >= lineas.size())
        return false;
    lineas[indice] = nombre + ";" + telefono + ";" + correo;
    return reescribir(lineas);
}

bool ArchivoContactos::eliminar(int indice) const
{
    QStringList lineas = leer();
    if (indice < 0 || indice >= lineas.size())
        return false;
    lineas.removeAt(indice);
    return reescribir(lineas);
}

bool ArchivoContactos::reescribir(const QStringList& lineas) const
{
    QString carpeta = QDir::homePath() + "/Lab7Datos";
    if (!QDir().mkpath(carpeta))
        return false;

    QSaveFile archivo(ruta());
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;

    QByteArray datos;
    for (const QString& linea : lineas) {
        datos += linea.toUtf8();
        datos += '\n';
    }
    bool completo = archivo.write(datos) == datos.size();
    bool confirmado = completo && archivo.commit();
    return confirmado;
}
