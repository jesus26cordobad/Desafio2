/*
 * Artista.h
 *
 * Representa un artista musical.
 * Contiene una colección de álbumes.
 */

#ifndef ARTISTA_H
#define ARTISTA_H

#include <string>
#include "ArrayDinamico.h"
#include "Album.h"
using namespace std;

class Artista {
private:
    long identificador;
    int edad;
    string pais;
    string nombre;
    long cantidadSeguidores;
    int posicionTendencias;
    ArrayDinamico<Album*>* albumes;

public:
    Artista();
    Artista(long id, int edad, string pais);
    ~Artista();

    // Getters
    long getIdentificador() const;
    int getEdad() const;
    string getPais() const;
    string getNombre() const;
    long getCantidadSeguidores() const;
    int getPosicionTendencias() const;
    int getCantidadAlbumes() const;

    // Setters
    void setNombre(const string& nom);
    void setCantidadSeguidores(long seguidores);
    void setPosicionTendencias(int pos);

    // Álbumes
    bool agregarAlbum(Album* album);
    Album* obtenerAlbum(int indice) const;

    // Métrica
    long calcularMemoria() const;
};

#endif
