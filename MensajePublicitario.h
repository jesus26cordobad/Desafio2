/*
 * MensajePublicitario.h
 *
 * Representa un mensaje publicitario que se muestra a usuarios estandar.
 * Tiene una categoria que determina su prioridad de aparicion.
 */

#ifndef MENSAJEPUBLICITARIO_H
#define MENSAJEPUBLICITARIO_H

#include <string>
using namespace std;

class MensajePublicitario {
private:
    string contenido;  //Texto del anuncio
    string categoria;  //AAA, B o C
    int prioridad;     //Valor numerico segun categoria

    void calcularPrioridad(); //Asigna prioridad segun categoria

public:
    MensajePublicitario();
    MensajePublicitario(string cont, string cat);

    //Accesores
    string getContenido() const;
    string getCategoria() const;
    int getPrioridad() const;

    //Modificadores
    void setContenido(string cont);
    void setCategoria(string cat);

    //Estimacion de memoria usada
    long calcularMemoria() const;
};

#endif
