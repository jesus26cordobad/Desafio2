#ifndef GESTORARTISTAS_H
#define GESTORARTISTAS_H

#include "ArrayDinamico.h"
#include "Artista.h"
#include "Album.h"
#include "Cancion.h"
#include <string>
using namespace std;

//Administra el catálogo de artistas y sus álbumes
class GestorArtistas {
private:
    ArrayDinamico<Artista*>* artistas;  //lista de artistas cargados

public:
    GestorArtistas();
    ~GestorArtistas();

    // Gestión básica
    void agregarArtista(Artista* art);
    Artista* obtener(int indice) const;
    int getCantidad() const;

    ArrayDinamico<Artista*>* getArtistas() const;

    // Funcionalidad general
    Artista* obtenerArtistaPorAlbum(const Album* album) const;
    Cancion* obtenerCancionAleatoria() const;

    // Carga y guardado
    bool cargarDesdeArchivo(string rutaArchivo);
    bool guardarEnArchivo(string rutaArchivo) const;

    // Métrica
    long calcularMemoria() const;
};

#endif
