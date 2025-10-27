#ifndef LISTAFAVORITOS_H
#define LISTAFAVORITOS_H

#include "ArrayDinamico.h"
#include "Cancion.h"

class ListaFavoritos {
private:
    // Contenedor interno: arreglo dinámico de punteros a Cancion
    ArrayDinamico<Cancion*>* canciones;
    int cantidad;
    static const int MAX_CANCIONES = 10000;

public:
    ListaFavoritos();
    ~ListaFavoritos();

    // Operaciones
    bool agregar(Cancion* cancion);
    bool eliminar(long idCancion);
    bool contiene(long idCancion) const;

    // Acceso
    Cancion* obtenerCancion(int indice) const;
    int getCantidad() const;

    // Utilidades
    void limpiar();
    bool estaLlena() const;
    void agregarLista(ListaFavoritos* otra);

    void reemplazarCancion(int indice, Cancion* nuevaCancion);

    // Métrica
    long calcularMemoria() const;
};

#endif
