#include "GestorPublicidad.h"
#include <cstdlib>
#include <fstream>
#include <sstream>
using namespace std;

GestorPublicidad::GestorPublicidad() {
    mensajes = new ArrayDinamico<MensajePublicitario*>();
    pesosAcumulados = nullptr;
    pesoTotal = 0;
    ultimoIndice = -1;
}

GestorPublicidad::~GestorPublicidad() {
    //Libera cada mensaje
    for (int i = 0; i < mensajes->getCantidad(); i++) {
        delete mensajes->obtener(i);
    }
    delete mensajes;

    //Libera arreglo de pesos si existe
    if (pesosAcumulados != nullptr) {
        delete[] pesosAcumulados;
    }
}

bool GestorPublicidad::agregarMensaje(MensajePublicitario* mensaje) {
    if (mensaje == nullptr) return false;
    if (mensajes->getCantidad() >= 50) return false;    //limite razonable

    mensajes->agregar(mensaje);
    calcularPesos();    //actualiza prioridad
    return true;
}

void GestorPublicidad::calcularPesos() {
    if (pesosAcumulados != nullptr) delete[] pesosAcumulados;

    int cantidad = mensajes->getCantidad();
    pesosAcumulados = new int[cantidad];
    pesoTotal = 0;

    //Acumula prioridades para seleccion ponderada
    for (int i = 0; i < cantidad; i++) {
        int prioridad = mensajes->obtener(i)->getPrioridad();
        pesoTotal += prioridad;
        pesosAcumulados[i] = pesoTotal;
    }
}

MensajePublicitario* GestorPublicidad::obtenerMensajeAleatorio() {
    if (mensajes->getCantidad() == 0) return nullptr;
    if (mensajes->getCantidad() == 1) return mensajes->obtener(0);

    int intentos = 0;
    int indiceSeleccionado;

    //seleccion aleatoria ponderada por prioridad
    do {
        int valorAleatorio = rand() % pesoTotal;

        indiceSeleccionado = 0;
        for (int i = 0; i < mensajes->getCantidad(); i++) {
            if (valorAleatorio < pesosAcumulados[i]) {
                indiceSeleccionado = i;
                break;
            }
        }
        intentos++;
    } while (indiceSeleccionado == ultimoIndice && intentos < 10);

    // Garantizar no consecutivo si el azar insiste
    if (indiceSeleccionado == ultimoIndice) {
        indiceSeleccionado = (ultimoIndice + 1) % mensajes->getCantidad();
    }

    ultimoIndice = indiceSeleccionado;
    return mensajes->obtener(indiceSeleccionado);
}

int GestorPublicidad::getCantidadMensajes() const { return mensajes->getCantidad(); }

bool GestorPublicidad::cargarDesdeArchivo(string rutaArchivo) {
    ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        return false;
    }

    string linea;
    while (getline(archivo, linea)) {
        if (linea.empty()) continue;

        // Parsear: categoria|mensaje
        size_t pos = linea.find('|');
        if (pos == string::npos) continue;

        string categoria = linea.substr(0, pos);
        string mensaje = linea.substr(pos + 1);

        MensajePublicitario* pub = new MensajePublicitario(mensaje, categoria);
        agregarMensaje(pub);
    }

    archivo.close();
    return true;
}

bool GestorPublicidad::guardarEnArchivo(string rutaArchivo) const {
    ofstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        return false;
    }

    for (int i = 0; i < mensajes->getCantidad(); i++) {
        MensajePublicitario* msg = mensajes->obtener(i);
        if (msg) {
            archivo << msg->getCategoria() << "|"
                    << msg->getContenido() << endl;
        }
    }

    archivo.close();
    return true;
}

long GestorPublicidad::calcularMemoria() const {
    long memoria = sizeof(GestorPublicidad);
    memoria += mensajes->calcularMemoria();

    for (int i = 0; i < mensajes->getCantidad(); i++) {
        memoria += mensajes->obtener(i)->calcularMemoria();
    }
    if (pesosAcumulados != nullptr) {
        memoria += sizeof(int) * mensajes->getCantidad();
    }
    return memoria;
}
