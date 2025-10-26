/*
 * Creditos.h
 *
 * Representa los créditos de una canción.
 * Contiene productores y compositores (Colaboradores).
 */

#ifndef CREDITOS_H
#define CREDITOS_H

#include <string>
#include "ArrayDinamico.h"
#include "Colaborador.h"
using namespace std;

//Agrupa los créditos musicales de una canción
class Creditos {
private:
    ArrayDinamico<Colaborador*>* productores;
    ArrayDinamico<Colaborador*>* compositores;

public:
    Creditos();
    ~Creditos();

    //Agregar colaboradores
    bool agregarProductor(const string& nombre, const string& apellido, const string& lema, const string& id);
    bool agregarCompositor(const string& nombre, const string& apellido, const string& lema, const string& id);

    //Obtener colaboradores
    Colaborador* obtenerProductor(int indice) const;
    Colaborador* obtenerCompositor(int indice) const;

    //Cantidades actuales
    int getCantidadProductores() const;
    int getCantidadCompositores() const;

    //Métrica
    long calcularMemoria() const;
};

#endif
