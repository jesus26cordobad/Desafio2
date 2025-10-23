#ifndef CANCION_H
#define CANCION_H

#include <string>
using namespace std;

class Album;    // forward declaration
class Creditos; // forward declaration

class Cancion {
private:
    long identificador;
    string nombre;
    int duracion;          // en segundos
    string rutaArchivo;    // ruta base o completa
    long reproducciones;
    Album* album;
    Creditos* creditos;

public:
    Cancion();
    Cancion(const Cancion& c);
    Cancion(long id, const string& titulo, int duracionSeg, const string& ruta);

    // Getters
    long getIdentificador() const;
    string getNombre() const;
    string getTitulo() const;
    int getDuracion() const;
    string getRutaArchivo() const;
    string getRutaAudio() const; // alias
    long getReproduccion() const;

    // Setters
    void setNombre(const string& nombreNuevo);
    void setDuracion(int duracionSeg);
    void setAlbum(Album* a);
    Album* getAlbum() const;
    void setCreditos(Creditos* cred);
    Creditos* getCreditos() const;

    // Reproducciones
    void incrementarReproducciones();

    // Operadores
    bool operator==(const Cancion& otra) const;
    bool operator!=(const Cancion& otra) const;

    // Métrica
    long calcularMemoria() const;
};

#endif
