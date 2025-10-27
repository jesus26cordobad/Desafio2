/*
 * Sistema.h
 *
 * Clase principal que coordina todo el sistema.
 * Maneja el flujo de ejecucion y los menus de usuario.
 */

#ifndef SISTEMA_H
#define SISTEMA_H

#include "GestorUsuarios.h"
#include "GestorArtistas.h"
#include "GestorPublicidad.h"
#include "Reproductor.h"
#include "MedidorRecursos.h"
#include <string>
using namespace std;

//Orquesta todos los modulos del sistema musical
class Sistema {
private:
    GestorUsuarios* gestorUsuarios;         //administra usuarios
    GestorArtistas* gestorArtistas;         //administra catálogo musical
    GestorPublicidad* gestorPublicidad;     //administra mensajes publicitarios
    Reproductor* reproductor;               //reproductor activo
    Usuario* usuarioActual;                 //usuario logueado
    bool sistemaActivo;                     //controla el ciclo principal

    void cargarDatos();                     //carga desde archivos o genera demo
    void generarDatosPrueba();              //crea datos de ejemplo si no hay archivos
    bool login();                           //login por nickname
    void menuPrincipal();                   //menú principal según membresía
    void menuFavoritos();                   //menú exclusivo para favoritos
    void verMisFavoritos();                 //muestra lista actual
    void editarFavoritos();                 //permite eliminar canciones
    void seguirOtraLista();                 //seguir favoritos de otro usuario
    void ejecutarFavoritos();               //reproduce favoritos

public:
    Sistema();
    ~Sistema();

    void iniciar();                         //lanza el sistema
    void finalizar();                       //cierra el sistema

    //Accesores
    GestorUsuarios* getGestorUsuarios() const;
    GestorArtistas* getGestorArtistas() const;
    GestorPublicidad* getGestorPublicidad() const;

    long calcularMemoriaTotal() const;     //metrica de memoria
};

#endif
