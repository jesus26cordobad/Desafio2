/*
 * ArrayDinamico.h
 *
 * Implementa un arreglo dinámico genérico.
 */

#ifndef ARRAYDINAMICO_H
#define ARRAYDINAMICO_H

#include <stdexcept>

template <typename T>
class ArrayDinamico {
private:
    T* datos;
    int capacidad;
    int cantidad;

    void redimensionar(int nuevaCapacidad) {
        T* nuevosDatos = new T[nuevaCapacidad];
        for (int i = 0; i < cantidad; ++i) {
            nuevosDatos[i] = datos[i];
        }
        delete[] datos;
        datos = nuevosDatos;
        capacidad = nuevaCapacidad;
    }

public:
    ArrayDinamico(int capInicial = 10) {
        if (capInicial <= 0) capInicial = 10;
        capacidad = capInicial;
        cantidad = 0;
        datos = new T[capacidad];
    }

    ~ArrayDinamico() {
        delete[] datos;
    }

    bool agregar(const T& valor) {
        if (cantidad >= capacidad) {
            redimensionar(capacidad * 2);
        }
        datos[cantidad++] = valor;
        return true;
    }

    T obtener(int indice) const {
        if (indice < 0 || indice >= cantidad) {
            throw std::out_of_range("Indice fuera de rango en ArrayDinamico");
        }
        return datos[indice];
    }

    int getCantidad() const { return cantidad; }
    int getTamanio() const { return cantidad; }

    bool eliminar(int indice) {
        if (indice < 0 || indice >= cantidad) return false;

        for (int i = indice; i < cantidad - 1; i++) {
            datos[i] = datos[i + 1];
        }
        cantidad--;
        return true;
    }

    void limpiar() {
        cantidad = 0;
    }

    bool contiene(const T& elemento) const {
        for (int i = 0; i < cantidad; i++) {
            if (datos[i] == elemento) {
                return true;
            }
        }
        return false;
    }

    long calcularMemoria() const {
        long memoria = sizeof(ArrayDinamico<T>);
        memoria += sizeof(T) * capacidad;
        return memoria;
    }
    int getCapacidad() const { return capacidad; }

    T& operator[](int indice) {
        if (indice < 0 || indice >= cantidad) {
            throw std::out_of_range("Indice fuera de rango en ArrayDinamico");
        }
        return datos[indice];
    }

    const T& operator[](int indice) const {
        if (indice < 0 || indice >= cantidad) {
            throw std::out_of_range("Indice fuera de rango en ArrayDinamico");
        }
        return datos[indice];
    }
};

#endif
