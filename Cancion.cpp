#include "Cancion.h"
#include "Album.h"

Cancion::Cancion()
    : identificador(0), nombre(""), duracion(0),
    rutaArchivo(""), reproducciones(0),
    album(nullptr), creditos(nullptr) {}

Cancion::Cancion(const Cancion& c)
    : identificador(c.identificador), nombre(c.nombre),
    duracion(c.duracion), rutaArchivo(c.rutaArchivo),
    reproducciones(c.reproducciones),
    album(c.album), creditos(c.creditos) {}

Cancion::Cancion(long id, const string& titulo, int duracionSeg, const string& ruta)
    : identificador(id), nombre(titulo), duracion(duracionSeg),
    rutaArchivo(ruta), reproducciones(0),
    album(nullptr), creditos(nullptr) {}

long Cancion::getIdentificador() const { return identificador; }
string Cancion::getNombre() const { return nombre; }
string Cancion::getTitulo() const { return nombre; }
int Cancion::getDuracion() const { return duracion; }
string Cancion::getRutaArchivo() const { return rutaArchivo; }
string Cancion::getRutaAudio() const { return rutaArchivo; }
long Cancion::getReproduccion() const { return reproducciones; }

void Cancion::setNombre(const string& nombreNuevo) { nombre = nombreNuevo; }
void Cancion::setDuracion(int duracionSeg) { duracion = duracionSeg; }
void Cancion::setAlbum(Album* a) { album = a; }
Album* Cancion::getAlbum() const { return album; }
void Cancion::setCreditos(Creditos* cred) { creditos = cred; }
Creditos* Cancion::getCreditos() const { return creditos; }

void Cancion::incrementarReproducciones() { ++reproducciones; }

bool Cancion::operator==(const Cancion& otra) const {
    return identificador == otra.identificador;
}
bool Cancion::operator!=(const Cancion& otra) const {
    return !(*this == otra);
}

long Cancion::calcularMemoria() const {
    long memoria = sizeof(Cancion);
    memoria += static_cast<long>(nombre.capacity());
    memoria += static_cast<long>(rutaArchivo.capacity());
    return memoria;
}
