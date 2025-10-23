/*
 * HashMap.h
 *
 * Tabla hash generica para almacenar pares clave-valor.
 * Permite busquedas rapidas en tiempo constante O(1).
 *
 * Se usa en:
 * - GestorUsuarios: buscar usuarios por nickname
 * - GestorArtistas: buscar artistas por codigo
 */

#ifndef HASHMAP_H
#define HASHMAP_H

//#include "MedidorRecursos.h"
#include <string>
#include <cstdlib>
#include <typeinfo>
using namespace std;

template <typename K, typename V>
class HashMap {
private:
    struct Nodo {
        K clave;
        V valor;
        Nodo* siguiente;

        Nodo(K c, V v) : clave(c), valor(v), siguiente(nullptr) {}
    };

    Nodo** tabla;
    int capacidad;
    int tamanio;
    float factorCarga;

    int hashString(const string& clave) const {
        unsigned long hash = 0;
        const int primo = 31;

        for (size_t i = 0; i < clave.length(); i++) {
            hash = hash * primo + clave[i];
        }

        return hash % capacidad;
    }

    int hashInt(int clave) const {
        int valor = clave;
        if (valor < 0) valor = -valor;
        return valor % capacidad;
    }

    int hashLong(long clave) const {
        long valor = clave;
        if (valor < 0) valor = -valor;
        return (int)(valor % capacidad);
    }

    int hash(const K& clave) const {
        return 0;
    }

    void redimensionar() {
        int capacidadAntigua = capacidad;
        Nodo** tablaAntigua = tabla;

        capacidad = capacidad * 2;
        tabla = new Nodo*[capacidad];

        for (int i = 0; i < capacidad; i++) {
            tabla[i] = nullptr;
        }

        tamanio = 0;

        for (int i = 0; i < capacidadAntigua; i++) {
            Nodo* actual = tablaAntigua[i];
            while (actual != nullptr) {
                insertar(actual->clave, actual->valor);
                Nodo* temp = actual;
                actual = actual->siguiente;
                delete temp;
            }
        }

        delete[] tablaAntigua;
    }

    int obtenerIndice(const K& clave) const {
        if (typeid(K) == typeid(string)) {
            const string* strPtr = reinterpret_cast<const string*>(&clave);
            return hashString(*strPtr);
        } else if (typeid(K) == typeid(int)) {
            const int* intPtr = reinterpret_cast<const int*>(&clave);
            return hashInt(*intPtr);
        } else if (typeid(K) == typeid(long)) {
            const long* longPtr = reinterpret_cast<const long*>(&clave);
            return hashLong(*longPtr);
        }
        return 0;
    }

public:
    HashMap(int capacidadInicial = 101) {
        capacidad = capacidadInicial;
        tamanio = 0;
        factorCarga = 0.75f;
        tabla = new Nodo*[capacidad];

        for (int i = 0; i < capacidad; i++) {
            tabla[i] = nullptr;
        }
    }

    ~HashMap() {
        for (int i = 0; i < capacidad; i++) {
            Nodo* actual = tabla[i];
            while (actual != nullptr) {
                Nodo* siguiente = actual->siguiente;
                delete actual;
                actual = siguiente;
            }
        }
        delete[] tabla;
    }

    bool insertar(const K& clave, const V& valor) {
        if ((float)tamanio / capacidad >= factorCarga) {
            redimensionar();
        }

        int indice = obtenerIndice(clave);

        Nodo* actual = tabla[indice];
        while (actual != nullptr) {
            if (actual->clave == clave) {
                actual->valor = valor;
                return true;
            }
            actual = actual->siguiente;
        }

        Nodo* nuevoNodo = new Nodo(clave, valor);
        nuevoNodo->siguiente = tabla[indice];
        tabla[indice] = nuevoNodo;
        tamanio++;

        return true;
    }

    V buscar(const K& clave) const {
        int indice = obtenerIndice(clave);

        Nodo* actual = tabla[indice];
        while (actual != nullptr) {
            MedidorRecursos::incrementarIteraciones();
            if (actual->clave == clave) {
                return actual->valor;
            }
            actual = actual->siguiente;
        }

        return V();
    }

    bool contiene(const K& clave) const {
        int indice = obtenerIndice(clave);

        Nodo* actual = tabla[indice];
        while (actual != nullptr) {
            MedidorRecursos::incrementarIteraciones();
            if (actual->clave == clave) {
                return true;
            }
            actual = actual->siguiente;
        }

        return false;
    }

    bool eliminar(const K& clave) {
        int indice = obtenerIndice(clave);

        Nodo* actual = tabla[indice];
        Nodo* anterior = nullptr;

        while (actual != nullptr) {
            MedidorRecursos::incrementarIteraciones();
            if (actual->clave == clave) {
                if (anterior == nullptr) {
                    tabla[indice] = actual->siguiente;
                } else {
                    anterior->siguiente = actual->siguiente;
                }
                delete actual;
                tamanio--;
                return true;
            }
            anterior = actual;
            actual = actual->siguiente;
        }

        return false;
    }

    int getTamanio() const { return tamanio; }
    int getCapacidad() const { return capacidad; }
    bool estaVacio() const { return tamanio == 0; }

    long calcularMemoria() const {
        long memoria = sizeof(HashMap<K, V>);
        memoria += sizeof(Nodo*) * capacidad;

        for (int i = 0; i < capacidad; i++) {
            Nodo* actual = tabla[i];
            while (actual != nullptr) {
                memoria += sizeof(Nodo);
                actual = actual->siguiente;
            }
        }

        return memoria;
    }
};

#endif
