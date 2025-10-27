#include "GestorUsuarios.h"
#include "Utilidades.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

GestorUsuarios::GestorUsuarios() {
    tablaUsuarios = new HashMap<string, Usuario*>(101);
    listaUsuarios = new ArrayDinamico<Usuario*>();
}

GestorUsuarios::~GestorUsuarios() {
    for (int i = 0; i < listaUsuarios->getTamanio(); i++) {
        delete listaUsuarios->obtener(i);
    }
    delete listaUsuarios;
    delete tablaUsuarios;
}

bool GestorUsuarios::agregarUsuario(Usuario* usuario) {
    if (usuario == nullptr) return false;

    string nickname = usuario->getNickname();
    if (existeUsuario(nickname)) return false;

    tablaUsuarios->insertar(nickname, usuario);
    listaUsuarios->agregar(usuario);
    return true;
}

Usuario* GestorUsuarios::buscarUsuario(string nickname) const {
    return tablaUsuarios->buscar(nickname);
}

bool GestorUsuarios::existeUsuario(string nickname) const {
    return tablaUsuarios->contiene(nickname);
}

bool GestorUsuarios::validarLogin(string nickname) const {
    return existeUsuario(nickname);
}

int GestorUsuarios::getCantidadUsuarios() const {
    return listaUsuarios->getCantidad();
}

Usuario* GestorUsuarios::obtenerPorIndice(int indice) const {
    if (indice < 0 || indice >= listaUsuarios->getCantidad()) return nullptr;
    return listaUsuarios->obtener(indice);
}

bool GestorUsuarios::cargarDesdeArchivo(string rutaArchivo) {
    ifstream archivo(rutaArchivo);
    if (!archivo.is_open()){
        return false;
    }

    string linea;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        // Parsear: nickname|tipo|ciudad|pais|fecha|[id1,id2,id3]
        size_t pos1 = linea.find('|');
        size_t pos2 = linea.find('|', pos1 + 1);
        size_t pos3 = linea.find('|', pos2 + 1);
        size_t pos4 = linea.find('|', pos3 + 1);
        size_t pos5 = linea.find('|', pos4 + 1);

        if (pos1 == string::npos || pos2 == string::npos ||
            pos3 == string::npos || pos4 == string::npos) continue;

        string nickname = linea.substr(0, pos1);
        string tipo = linea.substr(pos1 + 1, pos2 - pos1 - 1);
        string ciudad = linea.substr(pos2 + 1, pos3 - pos2 - 1);
        string pais = linea.substr(pos3 + 1, pos4 - pos3 - 1);
        string fecha = linea.substr(pos4 + 1, pos5 - pos4 - 1);

        Usuario* usuario = new Usuario(nickname, tipo, ciudad, pais);
        usuario->setFechaInscripcion(fecha);

        // ========== Guardar IDs de favoritos temporalmente ==========
        if (pos5 != string::npos && pos5 + 1 < linea.length()) {
            string favoritosStr = linea.substr(pos5 + 1);

            // Buscar [id1,id2,id3]
            size_t inicio = favoritosStr.find('[');
            size_t fin = favoritosStr.find(']');

            if (inicio != string::npos && fin != string::npos && fin > inicio) {
                string idsStr = favoritosStr.substr(inicio + 1, fin - inicio - 1);

                // Guardar temporalmente en una propiedad del usuario
                // (necesitamos agregar esto al Usuario)
                usuario->setFavoritosIdsTemp(idsStr);
            }
        }

        agregarUsuario(usuario);
    }

    archivo.close();
    return true;
}

bool GestorUsuarios::guardarEnArchivo(string rutaArchivo) const {
    ofstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        //cout << "ERROR: No se pudo abrir el archivo para escribir: " << rutaArchivo << endl;
        return false;
    }

    //int usuariosGuardados = 0;

    for (int i = 0; i < listaUsuarios->getCantidad(); i++) {
        Usuario* usuario = listaUsuarios->obtener(i);
        if (!usuario) continue;

        archivo << usuario->getNickname() << "|"
                << usuario->getTipoMembresia() << "|"
                << usuario->getCiudad() << "|"
                << usuario->getPais() << "|"
                << usuario->getFechaInscripcion() << "|";

        // Guardar IDs de favoritos si es premium
        if (usuario->esPremium()) {
            ListaFavoritos* favs = usuario->getFavoritos();
            if (favs != nullptr && favs->getCantidad() > 0) {
                archivo << "[";
                for (int j = 0; j < favs->getCantidad(); j++) {
                    Cancion* cancion = favs->obtenerCancion(j);
                    if (cancion) {
                        archivo << cancion->getIdentificador();
                        if (j < favs->getCantidad() - 1) {
                            archivo << ",";
                        }
                    }
                }
                archivo << "]";
            } else {
                archivo << "[]";
            }
        } else {
            archivo << "[]";
        }

        archivo << endl;
        //usuariosGuardados++;
    }

    archivo.close();

    //cout << "DEBUG: Se guardaron " << usuariosGuardados << " usuarios en " << rutaArchivo << endl;
    return true;
}

long GestorUsuarios::calcularMemoria() const {
    long memoria = sizeof(GestorUsuarios);
    memoria += tablaUsuarios->calcularMemoria();
    memoria += listaUsuarios->calcularMemoria();
    for (int i = 0; i < listaUsuarios->getCantidad(); i++) {
        memoria += listaUsuarios->obtener(i)->calcularMemoria();
    }
    return memoria;
}

// ========== Reconstruir favoritos desde IDs ==========
void GestorUsuarios::reconstruirFavoritos(GestorArtistas* gestorArtistas) {
    if (!gestorArtistas) return;

    for (int i = 0; i < listaUsuarios->getCantidad(); i++) {
        Usuario* u = listaUsuarios->obtener(i);
        if (!u || !u->esPremium()) continue;

        string idsTemp = u->getFavoritosIdsTemp();
        if (idsTemp.empty()) continue;

        //int cancionesReconstruidas = 0;

        // Parsear IDs: "id1,id2,id3"
        size_t inicio = 0;
        size_t fin = idsTemp.find(',');

        while (inicio < idsTemp.length()) {
            string idStr;
            if (fin == string::npos) {
                idStr = idsTemp.substr(inicio);
                inicio = idsTemp.length();
            } else {
                idStr = idsTemp.substr(inicio, fin - inicio);
                inicio = fin + 1;
                fin = idsTemp.find(',', inicio);
            }

            if (idStr.empty()) continue;

            long id = Utilidades::stringToLong(idStr);

            // Buscar canción en el catálogo
            Cancion* cancion = nullptr;
            for (int j = 0; j < gestorArtistas->getCantidad() && !cancion; j++) {
                Artista* art = gestorArtistas->obtener(j);
                if (!art) continue;

                for (int k = 0; k < art->getCantidadAlbumes() && !cancion; k++) {
                    Album* alb = art->obtenerAlbum(k);
                    if (!alb) continue;

                    cancion = alb->buscarCancion(id);
                }
            }

            if (cancion) {
                u->agregarFavorito(cancion);
                //cancionesReconstruidas++;
            }
        }

        // Limpiar IDs temporales
        u->setFavoritosIdsTemp("");
    }
}
