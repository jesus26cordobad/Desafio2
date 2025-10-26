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
    ArrayDinamico<string>* generos;      // máx 4
    string selloDisquero;
    string rutaPortada;
    float puntuacion;
    ArrayDinamico<Cancion*>* canciones;

    void calcularDuracionTotal();

public:
    Album();
    Album(int codigo, string nom, string fecha, string sello);
    ~Album();

    // Getters
    int getCodigoId() const;
    string getNombre() const;
    string getFechaLanzamiento() const;
    int getDuracionTotal() const;
    string getSelloDisquero() const;
    string getRutaPortada() const;
    float getPuntuacion() const;
    int getCantidadCanciones() const;
    int getCantidadGeneros() const;

    // Setters
    void setNombre(string nom);
    void setRutaPortada(string ruta);
    void setPuntuacion(float punt);

    void setDuracionTotal(int segundos);

    // Gestión
    bool agregarGenero(string genero);          // respeta máx 4
    bool agregarCancion(Cancion* cancion);
    Cancion* buscarCancion(long idCancion) const;
    Cancion* obtenerCancion(int indice) const;
    string   obtenerGenero(int indice) const;

    Album operator+(const Album& otro) const;

    // Métrica
    long calcularMemoria() const;
};

#endif

