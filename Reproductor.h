/*
 * Reproductor.h
 *
 * Controla la reproduccion de canciones.
 * Maneja el historial para poder retroceder canciones.
 * Aplica publicidad para usuarios estandar.
 */

#ifndef REPRODUCTOR_H
#define REPRODUCTOR_H

#include "ListaEnlazada.h"
#include "Cancion.h"
#include "Usuario.h"
#include "GestorArtistas.h"
#include "GestorPublicidad.h"
#include <string>
using namespace std;

class Reproductor {
private:
    ListaEnlazada<Cancion*>* historial;     //historial de canciones reproducidas
    Usuario* usuarioActual;                 //usuario que esta usando el reproductor
    GestorArtistas* gestorArtistas;         //acceso al catalogo musical
    GestorPublicidad* gestorPublicidad;     //acceso a los mensajes publicitarios
    Cancion* cancionActual;                 //canción que se esta reproduciendo
    bool modoRepetir;                       //si esta activado el modo repetir
    int contadorPublicidad;                 //contador para mostrar publicidad
    int contadorCanciones;                  //canciones reproducidas en la sesión
    bool reproduciendo;                     //estado de reproducción

    // control de retroceso y contexto
    int indiceHistorial;                    //indice actual en historial
    bool enFavoritos;                       //si esta en modo favoritos

    int maxHistorial; //Limite de retroceso

    //funciones auxiliares
    void mostrarPublicidad();
    void mostrarCancion(Cancion* cancion);

    void agregarAlHistorial(Cancion* c);

public:
    Reproductor(Usuario* usuario, GestorArtistas* gestorArt, GestorPublicidad* gestorPub);
    ~Reproductor();

    //modos de reproduccion
    void reproduccionAleatoria();
    void reproducirFavoritos(bool aleatorio);
    void reproducirFavoritosAutomatico(bool aleatorio);

    //navegacion
    void siguiente();
    void anterior();
    void detener();

    //control de repeticion
    void activarRepetir();
    void desactivarRepetir();
    bool estaReproduciendo() const;
    bool getModoRepetir() const;

    //metrica de memoria
    long calcularMemoria() const;
};

#endif
