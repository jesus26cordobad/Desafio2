/*
 * Usuario.h
 *
 * Representa un usuario de la plataforma.
 * Puede ser estandar o premium.
 * Solo los premium tienen lista de favoritos.
 */

#ifndef USUARIO_H
#define USUARIO_H

#include <string>
#include "ListaFavoritos.h"
using namespace std;

class Usuario {
private:
    string nickname;
    string tipoMembresia;
    string ciudad;
    string pais;
    string fechaInscripcion;
    ListaFavoritos* favoritos;
    string favoritosIdsTemp;

public:
    Usuario();
    Usuario(string nick, string tipo, string ciud, string pa);
    Usuario(const Usuario& otro);
    ~Usuario();

    //Accesores
    string getNickname() const;
    string getTipoMembresia() const;
    string getCiudad() const;
    string getPais() const;
    string getFechaInscripcion() const;
    ListaFavoritos* getFavoritos() const;

    //Modificadores
    void setTipoMembresia(string tipo);
    void setCiudad(string ciud);
    void setPais(string pa);
    void setFechaInscripcion(string fecha);

    //Funcionalidad
    bool esPremium() const;
    bool agregarFavorito(Cancion* cancion);
    bool eliminarFavorito(long idCancion);
    bool seguirListaOtroUsuario(Usuario* otroUsuario);

    //Getters/setters para IDs temporales
    void setFavoritosIdsTemp(const string& ids);
    string getFavoritosIdsTemp() const;

    bool operator==(const Usuario& otro) const;

    long calcularMemoria() const;
};

#endif
