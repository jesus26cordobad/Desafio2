#include "Creditos.h"

Creditos::Creditos() {
    productores = new ArrayDinamico<Colaborador*>();
    compositores = new ArrayDinamico<Colaborador*>();
}

Creditos::~Creditos() {
    //Liberar memoria de colaboradores
    int nProd = productores->getCantidad();
    for (int i = 0; i < nProd; ++i) {
        delete productores->obtener(i);
    }
    int nComp = compositores->getCantidad();
    for (int i = 0; i < nComp; ++i) {
        delete compositores->obtener(i);
    }
    delete productores;
    delete compositores;
}

bool Creditos::agregarProductor(const string& nombre, const string& apellido, const string& lema, const string& id) {
    Colaborador* c = new Colaborador(nombre, apellido, lema, id);
    return productores->agregar(c);
}

bool Creditos::agregarCompositor(const string& nombre, const string& apellido, const string& lema, const string& id) {
    Colaborador* c = new Colaborador(nombre, apellido, lema, id);
    return compositores->agregar(c);
}

Colaborador* Creditos::obtenerProductor(int indice) const {
    return productores->obtener(indice);
}

Colaborador* Creditos::obtenerCompositor(int indice) const {
    return compositores->obtener(indice);
}

int Creditos::getCantidadProductores() const {
    return productores->getCantidad();
}

int Creditos::getCantidadCompositores() const {
    return compositores->getCantidad();
}

long Creditos::calcularMemoria() const {
    long memoria = sizeof(Creditos);
    int nProd = productores->getCantidad();
    for (int i = 0; i < nProd; ++i) {
        Colaborador* c = productores->obtener(i);
        if (c) memoria += c->calcularMemoria();
    }
    int nComp = compositores->getCantidad();
    for (int i = 0; i < nComp; ++i) {
        Colaborador* c = compositores->obtener(i);
        if (c) memoria += c->calcularMemoria();
    }
    return memoria;
}
