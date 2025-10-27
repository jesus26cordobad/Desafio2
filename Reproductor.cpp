#include "Reproductor.h"
#include "MedidorRecursos.h"
#include "GestorArtistas.h"
#include "GestorPublicidad.h"
#include "MensajePublicitario.h"
#include "ListaFavoritos.h"
#include "Album.h"
#include "Artista.h"
#include "Utilidades.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
using namespace std;

// =============== Utilidades internas ===============

//Genera un numero aleatorio entre 0ymaxIncl
static int randEnRango(int maxIncl) {
    if (maxIncl <= 0) return 0;
    return rand() % maxIncl;
}

//Baraja un arreglo de enteros usando Fisher-Yates
static void fisherYates(int* arr, int n) {
    if (!arr || n <= 1) return;
    for (int i = n - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}

//Verifica si un ID ya esta presente en el arreglo
static bool idPresente(long id, const long* vec, int n) {
    for (int i = 0; i < n; ++i) if (vec[i] == id) return true;
    return false;
}

// Normaliza y quita “/” final si existe
static string normalizarSinSlashFinal(const string& ruta) {
    string r = Utilidades::normalizarRuta(ruta);
    if (!r.empty() && r[r.length() - 1] == '/') r.erase(r.length() - 1);
    return r;
}

// Reemplaza o agrega carpeta de bitrate (128/320)
static string aplicarBitrate(const string& dirNorm, const string& kb) {
    int len = (int)dirNorm.length();
    if (len == 0) return kb + string("/") ;

    int posUlt = -1;
    for (int i = len - 1; i >= 0; --i) if (dirNorm[i] == '/') { posUlt = i; break; }

    string parent, ultimo;
    if (posUlt == -1) { parent = ""; ultimo = dirNorm; }
    else { parent = dirNorm.substr(0, posUlt); ultimo = dirNorm.substr(posUlt + 1); }

    if (ultimo == "128" || ultimo == "320")
        return parent.empty() ? (kb + string("/")) : (parent + "/" + kb + "/");
    return dirNorm + "/" + kb + "/";
}

// =============== Visualización de canción ===============
void Reproductor::mostrarCancion(Cancion* cancion) {
    if (!cancion) return;

    Album*  alb = cancion->getAlbum();
    Artista* art = (alb && gestorArtistas) ? gestorArtistas->obtenerArtistaPorAlbum(alb) : 0;

    string ruta = cancion->getRutaArchivo();
    int pos = -1;
    for (int i = (int)ruta.length() - 1; i >= 0; --i)
        if (ruta[i] == '/' || ruta[i] == '\\') { pos = i; break; }

    string dir  = (pos == -1) ? "" : ruta.substr(0, pos);
    string file = (pos == -1) ? ruta : ruta.substr(pos + 1);

    bool premium = (usuarioActual && usuarioActual->esPremium());
    string kb = premium ? "320" : "128";

    string dirNorm   = normalizarSinSlashFinal(dir);
    string dirConBit = aplicarBitrate(dirNorm, kb);
    string rutaAudio = dirConBit + file;

    string rutaPortada = alb ? normalizarSinSlashFinal(alb->getRutaPortada()) : "N/A";

    cout << "\n========================================" << endl;
    cout << "Cantante: " << (art ? art->getNombre() : "Desconocido") << endl;
    cout << "Album: " << (alb ? alb->getNombre() : "Desconocido") << endl;
    cout << "Ruta a la portada del album: " << rutaPortada << endl;
    cout << "Titulo de la cancion reproducida: " << cancion->getNombre() << endl;
    // cout << "ID: " << Utilidades::formatearIdCancion9(cancion->getIdentificador()) << endl;
    cout << "Ruta al archivo de audio: " << rutaAudio << endl;
    cout << "Duracion: " << cancion->getDuracion() << " segundos" << endl;
    cout << "========================================" << endl;
}

// =============== Ctor/Dtor ===============
Reproductor::Reproductor(Usuario* usuario, GestorArtistas* ga, GestorPublicidad* gp) {
    usuarioActual     = usuario;
    gestorArtistas    = ga;
    gestorPublicidad  = gp;
    historial         = new ListaEnlazada<Cancion*>();
    cancionActual     = 0;
    modoRepetir       = false;
    contadorPublicidad= 0;
    contadorCanciones = 0;
    reproduciendo     = false;
    indiceHistorial   = 0;
    enFavoritos       = false;
    maxHistorial      = 4; //por defecto, historial limitado a 4 canciones
}

Reproductor::~Reproductor() {
    delete historial;
    historial = 0;
}

// =============== Reproducción aleatoria sin repeticiones ===============

void Reproductor::reproduccionAleatoria() {
    if (!gestorArtistas) {
        cout << "No hay catalogo cargado." << endl;
        return;
    }

    cout << endl;
    cout << "=== Modo Reproduccion Aleatoria ===" << endl;
    cout << "Se reproduciran 5 canciones automaticamente" << endl;
    cout << "Temporizador: 3 segundos por cancion" << endl;

    MedidorRecursos::reiniciarContador();

    const int MAX_POOL = 1024;
    Cancion* pool[MAX_POOL];
    long     idsPool[MAX_POOL];
    int      total = 0;

    //construye pool de canciones unicas por ID
    int nArt = gestorArtistas->getCantidad();
    for (int i = 0; i < nArt && total < MAX_POOL; ++i) {
        Artista* ar = gestorArtistas->obtener(i);
        if (!ar) continue;

        int nAlb = ar->getCantidadAlbumes();
        for (int j = 0; j < nAlb && total < MAX_POOL; ++j) {
            Album* al = ar->obtenerAlbum(j);
            if (!al) continue;

            int nCan = al->getCantidadCanciones();
            for (int k = 0; k < nCan && total < MAX_POOL; ++k) {
                Cancion* c = al->obtenerCancion(k);
                if (!c) continue;

                long id = c->getIdentificador();
                if (idPresente(id, idsPool, total)) continue;
                pool[total]   = c;
                idsPool[total]= id;
                total++;
            }
        }
    }

    if (total == 0) {
        cout << "No hay canciones disponibles." << endl;
        return;
    }

    //Baraja indices del pool
    int* idx = new int[total];
    for (int i = 0; i < total; ++i) idx[i] = i;
    fisherYates(idx, total);

    //Selecciona hasta 5 canciones unicas
    const int LIM = (total < 5) ? total : 5;
    Cancion* seleccion[LIM];
    long     selectedIds[LIM];
    int      nSel = 0;

    for (int p = 0; p < total && nSel < LIM; ++p) {
        Cancion* c = pool[idx[p]];
        if (!c) continue;
        long id = c->getIdentificador();
        if (idPresente(id, selectedIds, nSel)) continue;
        seleccion[nSel]   = c;
        selectedIds[nSel] = id;
        nSel++;
    }

    //Reproduce cada cancion seleccionada
    for (int t = 0; t < nSel; ++t) {
        Cancion* c = seleccion[t];
        if (!c) continue;

        mostrarCancion(c);
        MedidorRecursos::incrementarIteraciones();

        //Mostrar publicidad cada 2 canciones si el usuario no es premium
        if (usuarioActual && !usuarioActual->esPremium() && (t % 2 == 1)) {
            MensajePublicitario* msg =
                (gestorPublicidad ? gestorPublicidad->obtenerMensajeAleatorio() : 0);
            if (msg) {
                cout << endl << "=======================================" << endl;
                cout << "           PUBLICIDAD" << endl;
                cout << "=======================================" << endl;
                cout << msg->getContenido() << endl;
                cout << "=======================================" << endl;
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    delete[] idx;
    cout << endl << "=== Reproduccion finalizada ===" << endl;
}

// =============== Favoritos: una por una + menú de controles ===============
void Reproductor::reproducirFavoritos(bool aleatorio) {
    if (!usuarioActual || !usuarioActual->esPremium()) {
        cout << "Esta funcion es solo para usuarios premium." << endl;
        return;
    }
    if (!usuarioActual->getFavoritos() || usuarioActual->getFavoritos()->getCantidad() == 0) {
        cout << "No hay favoritos para reproducir." << endl;
        return;
    }

    ListaFavoritos* fav = usuarioActual->getFavoritos();
    int n = fav->getCantidad();

    int* orden = new int[n];
    for (int i = 0; i < n; ++i) orden[i] = i;
    if (aleatorio) fisherYates(orden, n); //baraja si se pidio aleatorio

    MedidorRecursos::reiniciarContador();

    maxHistorial = 6; //M=6 para favoritos
    historial->limpiar();

    int indiceActual = 0;
    bool modoRepetir = false;
    bool continuar = true;

    while (continuar && indiceActual < n) {
        Cancion* c = fav->obtenerCancion(orden[indiceActual]);
        if (!c) {
            indiceActual++;
            continue;
        }

        agregarAlHistorial(c);  //guarda en historial

        mostrarCancion(c);
        MedidorRecursos::incrementarIteraciones();

        // Menú de controles
        int opcion = 0;
        do {
            cout << "\n========================================" << endl;
            cout << "         CONTROLES DE REPRODUCCION" << endl;
            cout << "========================================" << endl;
            cout << "Cancion " << (indiceActual + 1) << " de " << n << endl;

            //Mostrar cuantas canciones puede retroceder
            int canRetroceder = historial->getTamanio() - 1; //-1 porque la actul no cuenta
            cout << "Historial: " << canRetroceder << " canciones disponibles para retroceder" << endl;

            if (modoRepetir) {
                cout << "[MODO REPETIR ACTIVO]" << endl;
            }
            cout << "========================================" << endl;
            cout << "1. Siguiente" << endl;
            cout << "2. Anterior" << endl;
            cout << "3. " << (modoRepetir ? "Desactivar" : "Activar") << " repetir" << endl;
            cout << "4. Detener reproduccion" << endl;
            cout << "========================================" << endl;
            cout << "Seleccione una opcion: ";
            cin >> opcion;

            switch (opcion) {
            case 1: //Siguiente
                if (modoRepetir) {
                    //Si está en modo repetir, solo reproduce la misma canción de nuevo
                    mostrarCancion(c);
                    MedidorRecursos::incrementarIteraciones();
                } else {
                    //Avanzar a la siguiente
                    indiceActual++;
                    opcion = 0; //Salir del menú para mostrar siguiente canción
                }
                break;

            case 2: //Anterior
            {
                int historialSize = historial->getTamanio();

                if (historialSize <= 1) {
                    cout << "\nNo hay cancion anterior. Esta es la primera cancion del historial." << endl;
                } else {
                    //Retroceder: eliminar la canción actual del historial
                    historial->eliminarUltimo();

                    //Obtener la canción anterior
                    Cancion* anterior = historial->obtenerUltimo();

                    if (anterior) {
                        //Buscar su índice en el orden
                        for (int i = 0; i < n; i++) {
                            if (fav->obtenerCancion(orden[i]) == anterior) {
                                indiceActual = i;
                                break;
                            }
                        }
                        opcion = 0; //Salir para mostrar la canción anterior
                    }
                }
            }
            break;

            case 3: //Activar/Desactivar repetir
                modoRepetir = !modoRepetir;
                if (modoRepetir) {
                    cout << "\nModo repetir ACTIVADO. La cancion actual se repetira." << endl;
                } else {
                    cout << "\nModo repetir DESACTIVADO." << endl;
                }
                break;

            case 4: //Detener
                cout << "\nReproduccion detenida." << endl;
                continuar = false;
                opcion = 0; //Salir del menú
                break;

            default:
                cout << "Opcion no valida." << endl;
                break;
            }
        } while (opcion != 0);
    }

    delete[] orden;

    if (indiceActual >= n) {
        cout << "\n=== Reproduccion finalizada - Todas las canciones reproducidas ===" << endl;
    }

    //Limpiar historial al terminar
    historial->limpiar();
}

//Reproducción automática completa (como reproduccionAleatoria pero con TODA la lista)
void Reproductor::reproducirFavoritosAutomatico(bool aleatorio) {
    if (!usuarioActual || !usuarioActual->esPremium()) {
        cout << "Esta funcion es solo para usuarios premium." << endl;
        return;
    }
    if (!usuarioActual->getFavoritos() || usuarioActual->getFavoritos()->getCantidad() == 0) {
        cout << "No hay favoritos para reproducir." << endl;
        return;
    }

    ListaFavoritos* fav = usuarioActual->getFavoritos();
    int n = fav->getCantidad();

    cout << "\n=== Reproduccion Automatica de Favoritos ===" << endl;
    cout << "Se reproduciran " << n << " canciones automaticamente" << endl;
    cout << "Temporizador: 3 segundos por cancion" << endl;

    int* orden = new int[n];
    for (int i = 0; i < n; ++i) orden[i] = i;
    if (aleatorio) fisherYates(orden, n);

    MedidorRecursos::reiniciarContador();

    //Reproducir todas las canciones consecutivamente
    for (int t = 0; t < n; ++t) {
        Cancion* c = fav->obtenerCancion(orden[t]);
        if (!c) continue;

        cout << "\n[" << (t + 1) << "/" << n << "]" << endl;
        mostrarCancion(c);
        MedidorRecursos::incrementarIteraciones();

        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    delete[] orden;
    cout << "\n=== Reproduccion finalizada ===" << endl;
}

void Reproductor::agregarAlHistorial(Cancion* c) {
    if (!c) return;

    //Si ya existe la última canción y es la misma, no agregar (modo repetir)
    if (historial->getTamanio() > 0) {
        Cancion* ultima = historial->obtenerUltimo();
        if (ultima == c) {
            return; //No agregar duplicados consecutivos
        }
    }

    historial->agregarAlFinal(c);

    //Mantener solo las últimas maxHistorial canciones
    while (historial->getTamanio() > maxHistorial) {
        historial->eliminarPrimero();
    }
}

// =============== Memoria ===============
long Reproductor::calcularMemoria() const {
    long m = sizeof(Reproductor);
    if (historial)     m += historial->calcularMemoria();
    if (cancionActual) m += cancionActual->calcularMemoria();
    return m;
}
