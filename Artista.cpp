#include "Artista.h"

Artista::Artista()
    : identificador(0),
    edad(0),
    pais(""),
    nombre(""),
    cantidadSeguidores(0),
    posicionTendencias(0),
    albumes(new ArrayDinamico<Album*>()) {}

Artista::Artista(long id, int edad, string pais)
    : identificador(id),
    edad(edad),
    pais(pais),
    nombre(""),
    cantidadSeguidores(0),
    posicionTendencias(0),
    albumes(new ArrayDinamico<Album*>()) {}

Artista::~Artista() {
    delete albumes;
}

long Artista::getIdentificador() const { return identificador; }
int Artista::getEdad() const { return edad; }
string Artista::getPais() const { return pais; }
string Artista::getNombre() const { return nombre; }
long Artista::getCantidadSeguidores() const { return cantidadSeguidores; }
int Artista::getPosicionTendencias() const { return posicionTendencias; }
int Artista::getCantidadAlbumes() const { return albumes->getCantidad(); }

void Artista::setNombre(const string& nom) { nombre = nom; }
void Artista::setCantidadSeguidores(long seguidores) { cantidadSeguidores = seguidores; }
void Artista::setPosicionTendencias(int pos) { posicionTendencias = pos; }

bool Artista::agregarAlbum(Album* album) {
    if (!album) return false;
    return albumes->agregar(album);
}

Album* Artista::obtenerAlbum(int indice) const {
    return albumes->obtener(indice);
}

long Artista::calcularMemoria() const {
    long memoria = sizeof(Artista);
    memoria += static_cast<long>(pais.capacity());
    memoria += static_cast<long>(nombre.capacity());
    int n = albumes->getCantidad();
    for (int i = 0; i < n; ++i) {
        Album* alb = albumes->obtener(i);
        if (alb) memoria += alb->calcularMemoria();
    }
    return memoria;
}
