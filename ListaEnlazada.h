/*
 * ListaEnlazada.h
 *
 * Lista doblemente enlazada circular.
 * Permite navegacion hacia adelante y atras eficientemente.
 *
 * Se usa en:
 * - Reproductor: historial de canciones reproducidas
 */

#ifndef LISTAENLAZADA_H
#define LISTAENLAZADA_H

template <typename T>
class ListaEnlazada {
private:
    struct Nodo {
        T dato;
        Nodo* siguiente;
        Nodo* anterior;

        Nodo(T d) : dato(d), siguiente(nullptr), anterior(nullptr) {}
    };

    Nodo* primero;
    Nodo* ultimo;
    int tamanio;

public:
    ListaEnlazada() {
        primero = nullptr;
        ultimo = nullptr;
        tamanio = 0;
    }

    ~ListaEnlazada() {
        while (!estaVacia()) {
            eliminarPrimero();
        }
    }

    void agregarAlFinal(const T& dato) {
        Nodo* nuevoNodo = new Nodo(dato);

        if (estaVacia()) {
            primero = ultimo = nuevoNodo;
            nuevoNodo->siguiente = nuevoNodo;
            nuevoNodo->anterior = nuevoNodo;
        } else {
            nuevoNodo->anterior = ultimo;
            nuevoNodo->siguiente = primero;
            ultimo->siguiente = nuevoNodo;
            primero->anterior = nuevoNodo;
            ultimo = nuevoNodo;
        }

        tamanio++;
    }

    void agregarAlInicio(const T& dato) {
        Nodo* nuevoNodo = new Nodo(dato);

        if (estaVacia()) {
            primero = ultimo = nuevoNodo;
            nuevoNodo->siguiente = nuevoNodo;
            nuevoNodo->anterior = nuevoNodo;
        } else {
            nuevoNodo->siguiente = primero;
            nuevoNodo->anterior = ultimo;
            primero->anterior = nuevoNodo;
            ultimo->siguiente = nuevoNodo;
            primero = nuevoNodo;
        }

        tamanio++;
    }

    T obtenerPrimero() const {
        if (!estaVacia()) {
            return primero->dato;
        }
        return T();
    }

    T obtenerUltimo() const {
        if (!estaVacia()) {
            return ultimo->dato;
        }
        return T();
    }

    T obtener(int indice) const {
        if (indice < 0 || indice >= tamanio) {
            return T();
        }

        Nodo* actual = primero;
        for (int i = 0; i < indice; i++) {
            actual = actual->siguiente;
        }

        return actual->dato;
    }

    bool eliminarPrimero() {
        if (estaVacia()) {
            return false;
        }

        Nodo* temp = primero;

        if (tamanio == 1) {
            primero = ultimo = nullptr;
        } else {
            primero = primero->siguiente;
            primero->anterior = ultimo;
            ultimo->siguiente = primero;
        }

        delete temp;
        tamanio--;
        return true;
    }

    bool eliminarUltimo() {
        if (estaVacia()) {
            return false;
        }

        Nodo* temp = ultimo;

        if (tamanio == 1) {
            primero = ultimo = nullptr;
        } else {
            ultimo = ultimo->anterior;
            ultimo->siguiente = primero;
            primero->anterior = ultimo;
        }

        delete temp;
        tamanio--;
        return true;
    }

    void limpiar() {
        while (!estaVacia()) {
            eliminarPrimero();
        }
    }

    int getTamanio() const { return tamanio; }
    bool estaVacia() const { return tamanio == 0; }

    long calcularMemoria() const {
        long memoria = sizeof(ListaEnlazada<T>);
        memoria += sizeof(Nodo) * tamanio;
        return memoria;
    }
};

#endif
