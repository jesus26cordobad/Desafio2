/*
 * GestorUsuarios.h
 *
 * Administra todos los usuarios de la plataforma.
 * Permite buscar usuarios rapidamente por su nickname usando HashMap.
 */

#ifndef GESTORUSUARIOS_H
#define GESTORUSUARIOS_H

#include "HashMap.h"
#include "Usuario.h"
#include "ArrayDinamico.h"
#include "GestorArtistas.h"
#include <string>
using namespace std;

class GestorUsuarios {
private:
    HashMap<string, Usuario*>* tablaUsuarios;
    ArrayDinamico<Usuario*>* listaUsuarios; // owner

public:
    GestorUsuarios();
    ~GestorUsuarios();

    bool agregarUsuario(Usuario* usuario);
    Usuario* buscarUsuario(string nickname) const;
    bool existeUsuario(string nickname) const;
    bool validarLogin(string nickname) const;

    // Cantidad total de usuarios cargados
    int getCantidadUsuarios() const;

    // Acceso solo-lectura por indice a la lista interna
    Usuario* obtenerPorIndice(int indice) const;

    bool cargarDesdeArchivo(string rutaArchivo);
    bool guardarEnArchivo(string rutaArchivo) const;

    void reconstruirFavoritos(GestorArtistas* gestorArtistas);

    long calcularMemoria() const;
};

#endif
