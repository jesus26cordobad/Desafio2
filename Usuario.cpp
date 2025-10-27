#include "Usuario.h"

Usuario::Usuario() {
    nickname = "";
    tipoMembresia = "estandar";
    ciudad = "";
    pais = "";
    fechaInscripcion = "";
    favoritos = nullptr;
    favoritosIdsTemp = "";
}

Usuario::Usuario(string nick, string tipo, string ciud, string pa) {
    nickname = nick;
    tipoMembresia = tipo;
    ciudad = ciud;
    pais = pa;
    fechaInscripcion = "";
    favoritosIdsTemp = "";

    if (tipo == "premium") {
        favoritos = new ListaFavoritos();
    } else {
        favoritos = nullptr;
    }
}

Usuario::Usuario(const Usuario& otro) {
    nickname = otro.nickname;
    tipoMembresia = otro.tipoMembresia;
    ciudad = otro.ciudad;
    pais = otro.pais;
    fechaInscripcion = otro.fechaInscripcion;

    if (otro.favoritos != nullptr) {
        favoritos = new ListaFavoritos();
        for (int i = 0; i < otro.favoritos->getCantidad(); i++) {
            favoritos->agregar(otro.favoritos->obtenerCancion(i));
        }
    } else {
        favoritos = nullptr;
    }
}

Usuario::~Usuario() {
    if (favoritos != nullptr) {
        delete favoritos;
    }
}

string Usuario::getNickname() const {
    return nickname;
}

string Usuario::getTipoMembresia() const {
    return tipoMembresia;
}

string Usuario::getCiudad() const {
    return ciudad;
}

string Usuario::getPais() const {
    return pais;
}

string Usuario::getFechaInscripcion() const {
    return fechaInscripcion;
}

ListaFavoritos* Usuario::getFavoritos() const {
    return favoritos;
}

void Usuario::setTipoMembresia(string tipo) {
    if (tipo == "premium" && favoritos == nullptr) {
        favoritos = new ListaFavoritos();
    } else if (tipo == "estandar" && favoritos != nullptr) {
        delete favoritos;
        favoritos = nullptr;
    }
    tipoMembresia = tipo;
}

void Usuario::setCiudad(string ciud) {
    ciudad = ciud;
}

void Usuario::setPais(string pa) {
    pais = pa;
}

void Usuario::setFechaInscripcion(string fecha) {
    fechaInscripcion = fecha;
}

bool Usuario::esPremium() const {
    return tipoMembresia == "premium";
}

bool Usuario::agregarFavorito(Cancion* cancion) {
    if (!esPremium() || favoritos == nullptr) {
        return false;
    }

    return favoritos->agregar(cancion);
}

bool Usuario::eliminarFavorito(long idCancion) {
    if (!esPremium() || favoritos == nullptr) {
        return false;
    }

    return favoritos->eliminar(idCancion);
}

bool Usuario::seguirListaOtroUsuario(Usuario* otroUsuario) {
    if (!esPremium() || favoritos == nullptr) {
        return false;
    }

    if (otroUsuario == nullptr || !otroUsuario->esPremium()) {
        return false;
    }

    ListaFavoritos* otraLista = otroUsuario->getFavoritos();
    if (otraLista == nullptr) {
        return false;
    }

    favoritos->agregarLista(otraLista);
    return true;
}

bool Usuario::operator==(const Usuario& otro) const {
    return nickname == otro.nickname;
}

long Usuario::calcularMemoria() const {
    long memoria = sizeof(Usuario);
    memoria += nickname.length();
    memoria += tipoMembresia.length();
    memoria += ciudad.length();
    memoria += pais.length();
    memoria += fechaInscripcion.length();

    if (favoritos != nullptr) {
        memoria += favoritos->calcularMemoria();
    }

    return memoria;
}

void Usuario::setFavoritosIdsTemp(const string& ids) {
    favoritosIdsTemp = ids;
}

string Usuario::getFavoritosIdsTemp() const {
    return favoritosIdsTemp;
}
