/*
 * GestorPublicidad.h
 *
 * Administra los mensajes publicitarios.
 * Selecciona mensajes aleatoriamente respetando prioridades.
 */

#ifndef GESTORPUBLICIDAD_H
#define GESTORPUBLICIDAD_H

#include "ArrayDinamico.h"
#include "MensajePublicitario.h"
#include <string>
using namespace std;

class GestorPublicidad {
private:
    ArrayDinamico<MensajePublicitario*>* mensajes;
    int* pesosAcumulados;  //acumulado de prioridades
    int pesoTotal;         //suma total de prioridades
    int ultimoIndice;      //evita repetir el mismo anuncio

    void calcularPesos();  //actualiza pesos acumulados

public:
    GestorPublicidad();
    ~GestorPublicidad();

    bool agregarMensaje(MensajePublicitario* mensaje);
    MensajePublicitario* obtenerMensajeAleatorio();
    int getCantidadMensajes() const;

    bool cargarDesdeArchivo(string rutaArchivo);
    bool guardarEnArchivo(string rutaArchivo) const;

    long calcularMemoria() const;
};

#endif
