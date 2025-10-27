#include "GestorArtistas.h"
#include "Artista.h"
#include "Album.h"
#include "Cancion.h"
#include "Creditos.h"
#include "Colaborador.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
using namespace std;

GestorArtistas::GestorArtistas() {
    artistas = new ArrayDinamico<Artista*>();
}

GestorArtistas::~GestorArtistas() {
    delete artistas;
}

void GestorArtistas::agregarArtista(Artista* art) { artistas->agregar(art); }
Artista* GestorArtistas::obtener(int i) const { return artistas->obtener(i); }
int GestorArtistas::getCantidad() const { return artistas->getCantidad(); }

ArrayDinamico<Artista*>* GestorArtistas::getArtistas() const { return artistas; }

Artista* GestorArtistas::obtenerArtistaPorAlbum(const Album* album) const {
    if (!album) return nullptr;
    for (int i = 0; i < artistas->getCantidad(); ++i) {
        Artista* a = artistas->obtener(i);
        if (!a) continue;
        for (int j = 0; j < a->getCantidadAlbumes(); ++j) {
            if (a->obtenerAlbum(j) == album) return a;
        }
    }
    return nullptr;
}

Cancion* GestorArtistas::obtenerCancionAleatoria() const {
    if (artistas->getCantidad() == 0) return nullptr;
    Artista* art = artistas->obtener(rand() % artistas->getCantidad());
    if (!art || art->getCantidadAlbumes() == 0) return nullptr;
    Album* alb = art->obtenerAlbum(rand() % art->getCantidadAlbumes());
    if (!alb || alb->getCantidadCanciones() == 0) return nullptr;
    return alb->obtenerCancion(rand() % alb->getCantidadCanciones());
}

bool GestorArtistas::cargarDesdeArchivo(string rutaArchivo) {
    ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) return false;

    string linea;
    Artista* artistaActual = nullptr;
    Album* albumActual = nullptr;
    Cancion* cancionActual = nullptr;

    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        // nivel de indentación por espacios
        int nivel = 0;
        while (nivel < (int)linea.length() && linea[nivel] == ' ') nivel++;
        string contenido = linea.substr(nivel);

        if (nivel == 0) {
            // ARTISTA: id|edad|pais|nombre|seguidores|pos
            size_t p[5];
            p[0] = contenido.find('|');
            for (int i = 1; i < 5; ++i) p[i] = contenido.find('|', p[i-1] + 1);
            if (p[4] == string::npos) continue;

            long   id   = stol(contenido.substr(0, p[0]));
            int    edad = stoi(contenido.substr(p[0] + 1, p[1] - p[0] - 1));
            string pais = contenido.substr(p[1] + 1, p[2] - p[1] - 1);
            string nom  = contenido.substr(p[2] + 1, p[3] - p[2] - 1);
            long   seg  = stol(contenido.substr(p[3] + 1, p[4] - p[3] - 1));
            int    pos  = stoi(contenido.substr(p[4] + 1));

            artistaActual = new Artista(id, edad, pais);
            artistaActual->setNombre(nom);
            artistaActual->setCantidadSeguidores(seg);
            artistaActual->setPosicionTendencias(pos);
            agregarArtista(artistaActual);
        }
        else if (nivel == 2) {
            // ALBUM: id|nombre|fecha|durTotal|generos(coma)|sello|ruta|puntuacion
            if (!artistaActual) continue;

            size_t p[7];
            p[0] = contenido.find('|');
            for (int i = 1; i < 7; ++i) p[i] = contenido.find('|', p[i-1] + 1);
            if (p[6] == string::npos) continue;

            int    aid    = stoi(contenido.substr(0, p[0]));
            string nom    = contenido.substr(p[0] + 1, p[1] - p[0] - 1);
            string fecha  = contenido.substr(p[1] + 1, p[2] - p[1] - 1);
            int    durTot = stoi(contenido.substr(p[2] + 1, p[3] - p[2] - 1));
            string gens   = contenido.substr(p[3] + 1, p[4] - p[3] - 1);
            string sello  = contenido.substr(p[4] + 1, p[5] - p[4] - 1);
            string ruta   = contenido.substr(p[5] + 1, p[6] - p[5] - 1);
            float  punt   = stof(contenido.substr(p[6] + 1));

            albumActual = new Album(aid, nom, fecha, sello);
            albumActual->setRutaPortada(ruta);
            albumActual->setPuntuacion(punt);
            albumActual->setDuracionTotal(durTot); // evita warning y conserva valor

            // géneros separados por coma
            size_t s = 0, e = gens.find(',');
            while (e != string::npos) {
                albumActual->agregarGenero(gens.substr(s, e - s));
                s = e + 1;
                e = gens.find(',', s);
            }
            if (s < gens.length()) albumActual->agregarGenero(gens.substr(s));

            artistaActual->agregarAlbum(albumActual);
        }
        else if (nivel == 4) {
            // CANCION: id|nombre|duracion|ruta|reproducciones
            if (!albumActual) continue;

            size_t p[4];
            p[0] = contenido.find('|');
            for (int i = 1; i < 4; ++i) p[i] = contenido.find('|', p[i-1] + 1);
            if (p[3] == string::npos) continue;

            long   cid  = stol(contenido.substr(0, p[0]));
            string nom  = contenido.substr(p[0] + 1, p[1] - p[0] - 1);
            int    dur  = stoi(contenido.substr(p[1] + 1, p[2] - p[1] - 1));
            string ruta = contenido.substr(p[2] + 1, p[3] - p[2] - 1);
            long   reps = stol(contenido.substr(p[3] + 1));

            cancionActual = new Cancion(cid, nom, dur, ruta);
            for (long r = 0; r < reps; ++r) cancionActual->incrementarReproducciones();
            albumActual->agregarCancion(cancionActual);
        }
        else if (nivel == 6) {
            // CREDITOS de canción: "PROD:..." o "COMP:..."
            if (!cancionActual) continue;

            Creditos* cred = cancionActual->getCreditos();
            if (!cred) { cred = new Creditos(); cancionActual->setCreditos(cred); }

            if (contenido.rfind("PROD:", 0) == 0) {
                string datos = contenido.substr(5);
                size_t start = 0, end = datos.find(';');
                while (end != string::npos) {
                    string seg = datos.substr(start, end - start);
                    size_t q1 = seg.find(','), q2 = seg.find(',', q1 + 1), q3 = seg.find(',', q2 + 1);
                    if (q3 != string::npos) {
                        cred->agregarProductor(seg.substr(0, q1),
                                               seg.substr(q1+1, q2-q1-1),
                                               seg.substr(q2+1, q3-q2-1),
                                               seg.substr(q3+1));
                    }
                    start = end + 1;
                    end = datos.find(';', start);
                }
                // agregar el último si no terminó en ';'
                if (start < datos.length()) {
                    string seg = datos.substr(start);
                    size_t q1 = seg.find(','), q2 = seg.find(',', q1 + 1), q3 = seg.find(',', q2 + 1);
                    if (q3 != string::npos) {
                        cred->agregarProductor(seg.substr(0, q1),
                                               seg.substr(q1+1, q2-q1-1),
                                               seg.substr(q2+1, q3-q2-1),
                                               seg.substr(q3+1));
                    }
                }
            } else if (contenido.rfind("COMP:", 0) == 0) {
                string datos = contenido.substr(5);
                size_t start = 0, end = datos.find(';');
                while (end != string::npos) {
                    string seg = datos.substr(start, end - start);
                    size_t q1 = seg.find(','), q2 = seg.find(',', q1 + 1), q3 = seg.find(',', q2 + 1);
                    if (q3 != string::npos) {
                        cred->agregarCompositor(seg.substr(0, q1),
                                                seg.substr(q1+1, q2-q1-1),
                                                seg.substr(q2+1, q3-q2-1),
                                                seg.substr(q3+1));
                    }
                    start = end + 1;
                    end = datos.find(';', start);
                }
                // agregar el último si no terminó en ';'
                if (start < datos.length()) {
                    string seg = datos.substr(start);
                    size_t q1 = seg.find(','), q2 = seg.find(',', q1 + 1), q3 = seg.find(',', q2 + 1);
                    if (q3 != string::npos) {
                        cred->agregarCompositor(seg.substr(0, q1),
                                                seg.substr(q1+1, q2-q1-1),
                                                seg.substr(q2+1, q3-q2-1),
                                                seg.substr(q3+1));
                    }
                }
            }
        }
    }

    archivo.close();
    return true;
}

bool GestorArtistas::guardarEnArchivo(string rutaArchivo) const {
    ofstream archivo(rutaArchivo);
    if (!archivo.is_open()) return false;

    for (int i = 0; i < artistas->getCantidad(); ++i) {
        Artista* art = artistas->obtener(i);
        if (!art) continue;

        // ARTISTA
        archivo << art->getIdentificador() << "|"
                << art->getEdad() << "|"
                << art->getPais() << "|"
                << art->getNombre() << "|"
                << art->getCantidadSeguidores() << "|"
                << art->getPosicionTendencias() << endl;

        // ÁLBUMES
        for (int j = 0; j < art->getCantidadAlbumes(); ++j) {
            Album* alb = art->obtenerAlbum(j);
            if (!alb) continue;

            archivo << "  " << alb->getCodigoId() << "|"
                    << alb->getNombre() << "|"
                    << alb->getFechaLanzamiento() << "|"
                    << alb->getDuracionTotal() << "|";

            // géneros (coma)
            for (int g = 0; g < alb->getCantidadGeneros(); ++g) {
                archivo << alb->obtenerGenero(g);
                if (g < alb->getCantidadGeneros() - 1) archivo << ",";
            }

            archivo << "|" << alb->getSelloDisquero() << "|"
                    << alb->getRutaPortada() << "|"
                    << alb->getPuntuacion() << endl;

            // CANCIONES
            for (int k = 0; k < alb->getCantidadCanciones(); ++k) {
                Cancion* c = alb->obtenerCancion(k);
                if (!c) continue;

                archivo << "    " << c->getIdentificador() << "|"
                        << c->getNombre() << "|"
                        << c->getDuracion() << "|"
                        << c->getRutaArchivo() << "|"
                        << c->getReproduccion() << endl;

                // CRÉDITOS (si existen)
                Creditos* cred = c->getCreditos();
                if (cred) {
                    if (cred->getCantidadProductores() > 0) {
                        archivo << "      PROD:";
                        for (int p = 0; p < cred->getCantidadProductores(); ++p) {
                            Colaborador* col = cred->obtenerProductor(p);
                            if (col) {
                                archivo << col->getNombre() << ","
                                        << col->getApellido() << ","
                                        << col->getLemaario() << ","
                                        << col->getId();
                                if (p < cred->getCantidadProductores() - 1) archivo << ";";
                            }
                        }
                        archivo << endl;
                    }
                    if (cred->getCantidadCompositores() > 0) {
                        archivo << "      COMP:";
                        for (int q = 0; q < cred->getCantidadCompositores(); ++q) {
                            Colaborador* col = cred->obtenerCompositor(q);
                            if (col) {
                                archivo << col->getNombre() << ","
                                        << col->getApellido() << ","
                                        << col->getLemaario() << ","
                                        << col->getId();
                                if (q < cred->getCantidadCompositores() - 1) archivo << ";";
                            }
                        }
                        archivo << endl;
                    }
                }
            }
        }
    }

    archivo.close();
    return true;
}

long GestorArtistas::calcularMemoria() const {
    long m = sizeof(GestorArtistas);
    if (artistas) {
        m += artistas->calcularMemoria();
        for (int i = 0; i < artistas->getCantidad(); ++i) {
            Artista* a = artistas->obtener(i);
            if (a) m += a->calcularMemoria();
        }
    }
    return m;
}
