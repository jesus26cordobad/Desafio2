#include "Album.h"
#include <iostream>
using namespace std;

Album::Album()
    : codigoId(0),
    nombre(""),
    fechaLanzamiento(""),
    duracionTotal(0),
    generos(new ArrayDinamico<string>()),
    selloDisquero(""),
    rutaPortada(""),
    puntuacion(0.0f),
    canciones(new ArrayDinamico<Cancion*>()) {}

Album::Album(int codigo, string nom, string fecha, string sello)
    : codigoId(codigo),
    nombre(nom),
    fechaLanzamiento(fecha),
    duracionTotal(0),
    generos(new ArrayDinamico<string>()),
    selloDisquero(sello),
    rutaPortada(""),
    puntuacion(0.0f),
    canciones(new ArrayDinamico<Cancion*>()) {}

Album::~Album() {
    // no borramos canciones si las gestiona otro módulo
    delete generos;
    delete canciones;
}

int Album::getCodigoId() const { return codigoId; }
string Album::getNombre() const { return nombre; }
string Album::getFechaLanzamiento() const { return fechaLanzamiento; }
int Album::getDuracionTotal() const { return duracionTotal; }
string Album::getSelloDisquero() const { return selloDisquero; }
string Album::getRutaPortada() const { return rutaPortada; }
float Album::getPuntuacion() const { return puntuacion; }
int Album::getCantidadCanciones() const { return canciones->getCantidad(); }
int Album::getCantidadGeneros() const { return generos->getCantidad(); }

void Album::setNombre(string nom) { nombre = nom; }
void Album::setRutaPortada(string ruta) { rutaPortada = ruta; }
void Album::setPuntuacion(float punt) { puntuacion = punt; }

bool Album::agregarGenero(string genero) {
    if (generos->getCantidad() >= 4) return false;
    return generos->agregar(genero);
}

bool Album::agregarCancion(Cancion* cancion) {
    if (!cancion) return false;
    bool ok = canciones->agregar(cancion);
    if (ok) {
        cancion->setAlbum(this); // ENLACE PARA REPRODUCTOR
        calcularDuracionTotal();
    }
    return ok;
}

Cancion* Album::buscarCancion(long idCancion) const {
    int n = canciones->getCantidad();
    for (int i = 0; i < n; ++i) {
        Cancion* c = canciones->obtener(i);
        if (!c) continue;
        if (c->getIdentificador() == idCancion) {
            return c;
        }
    }
    return nullptr;
}

Cancion* Album::obtenerCancion(int indice) const {
    return canciones->obtener(indice);
}

string Album::obtenerGenero(int indice) const {
    return generos->obtener(indice);
}

Album Album::operator+(const Album& otro) const {
    Album combinado;
    combinado.nombre = this->nombre + " + " + otro.nombre;
    combinado.selloDisquero = this->selloDisquero;
    combinado.fechaLanzamiento = this->fechaLanzamiento;
    // Combinar canciones (referencias)
    int n1 = this->canciones->getCantidad();
    for (int i = 0; i < n1; ++i) {
        combinado.agregarCancion(this->canciones->obtener(i));
    }
    int n2 = otro.canciones->getCantidad();
    for (int j = 0; j < n2; ++j) {
        combinado.agregarCancion(otro.canciones->obtener(j));
    }
    // Combinar géneros (máximo 4)
    int g1 = this->generos->getCantidad();
    for (int i = 0; i < g1 && combinado.generos->getCantidad() < 4; ++i) {
        combinado.agregarGenero(this->generos->obtener(i));
    }
    int g2 = otro.generos->getCantidad();
    for (int j = 0; j < g2 && combinado.generos->getCantidad() < 4; ++j) {
        combinado.agregarGenero(otro.generos->obtener(j));
    }
    combinado.calcularDuracionTotal();
    return combinado;
}

long Album::calcularMemoria() const {
    long memoria = sizeof(Album);
    memoria += static_cast<long>(nombre.capacity());
    memoria += static_cast<long>(fechaLanzamiento.capacity());
    memoria += static_cast<long>(selloDisquero.capacity());
    memoria += static_cast<long>(rutaPortada.capacity());
    int n = canciones->getCantidad();
    for (int i = 0; i < n; ++i) {
        Cancion* c = canciones->obtener(i);
        if (c) memoria += c->calcularMemoria();
    }
    return memoria;
}

void Album::calcularDuracionTotal() {
    int total = 0;
    int n = canciones->getCantidad();
    for (int i = 0; i < n; ++i) {
        Cancion* c = canciones->obtener(i);
        if (c) total += c->getDuracion();
    }
    duracionTotal = total;
}
