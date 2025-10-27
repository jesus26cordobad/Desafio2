#include "ListaFavoritos.h"

ListaFavoritos::ListaFavoritos() {
    canciones = new ArrayDinamico<Cancion*>();
    cantidad = 0;
}

ListaFavoritos::~ListaFavoritos() {
    delete canciones;  // No borra canciones; solo el contenedor
}

bool ListaFavoritos::agregar(Cancion* cancion) {
    if (!cancion) return false;
    if (cantidad >= MAX_CANCIONES) return false;
    if (contiene(cancion->getIdentificador())) return false;

    canciones->agregar(cancion);
    cantidad++;
    return true;
}

bool ListaFavoritos::eliminar(long idCancion) {
    for (int i = 0; i < cantidad; ++i) {
        Cancion* c = canciones->obtener(i);
        if (c && c->getIdentificador() == idCancion) {
            canciones->eliminar(i);
            cantidad--;
            return true;
        }
    }
    return false;
}

bool ListaFavoritos::contiene(long idCancion) const {
    for (int i = 0; i < cantidad; ++i) {
        Cancion* c = canciones->obtener(i);
        if (c && c->getIdentificador() == idCancion) {
            return true;
        }
    }
    return false;
}

Cancion* ListaFavoritos::obtenerCancion(int indice) const {
    if (indice >= 0 && indice < cantidad) {
        return canciones->obtener(indice);
    }
    return nullptr;
}

int ListaFavoritos::getCantidad() const {
    return cantidad;
}

void ListaFavoritos::limpiar() {
    canciones->limpiar();
    cantidad = 0;
}

bool ListaFavoritos::estaLlena() const {
    return cantidad >= MAX_CANCIONES;
}

void ListaFavoritos::agregarLista(ListaFavoritos* otra) {
    if (!otra) return;
    for (int i = 0; i < otra->getCantidad(); ++i) {
        Cancion* c = otra->obtenerCancion(i);
        if (c) {
            if (!estaLlena()) {
                agregar(c); // evita duplicados
            } else {
                break;
            }
        }
    }
}

void ListaFavoritos::reemplazarCancion(int indice, Cancion* nuevaCancion) {
    if (!canciones) return;
    if (indice < 0 || indice >= cantidad) return;
    (*canciones)[indice] = nuevaCancion;
}

long ListaFavoritos::calcularMemoria() const {
    long memoria = sizeof(ListaFavoritos);
    if (canciones) memoria += canciones->calcularMemoria();
    return memoria;
}
