/*
 * Album.h
 *
 * Representa un album musical.
 * Contiene una coleccion de canciones y puede tener hasta 4 generos.
 */

#ifndef ALBUM_H
#define ALBUM_H

#include <string>
#include "ArrayDinamico.h"
#include "Cancion.h"
using namespace std;

class Album {
private:
    int codigoId;
    string nombre;
    string fechaLanzamiento;
    int duracionTotal;
    ArrayDinamico<string>* generos;
    string selloDisquero;
    string rutaPortada;
    float puntuacion;
    ArrayDinamico<Cancion*>* canciones;

    void calcularDuracionTotal();

public:
    Album();
    Album(int codigo, string nom, string fecha, string sello);
    ~Album();

    int getCodigoId() const;
    string getNombre() const;
    string getFechaLanzamiento() const;
    int getDuracionTotal() const;
    string getSelloDisquero() const;
    string getRutaPortada() const;
    float getPuntuacion() const;
    int getCantidadCanciones() const;
    int getCantidadGeneros() const;

    void setNombre(string nom);
    void setRutaPortada(string ruta);
    void setPuntuacion(float punt);

    bool agregarGenero(string genero);
    bool agregarCancion(Cancion* cancion);
    Cancion* buscarCancion(long idCancion) const;
    Cancion* obtenerCancion(int indice) const;
    string obtenerGenero(int indice) const;

    Album operator+(const Album& otro) const;

    long calcularMemoria() const;
};

#endif
