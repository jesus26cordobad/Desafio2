#include "MensajePublicitario.h"

MensajePublicitario::MensajePublicitario() {
    contenido = "";
    categoria = "C"; //Por defecto, prioridad baja
    prioridad = 1;
}

MensajePublicitario::MensajePublicitario(string cont, string cat) {
    contenido = cont;
    categoria = cat;
    calcularPrioridad(); //Calcula prioridad segun categoria
}

string MensajePublicitario::getContenido() const {
    return contenido;
}

string MensajePublicitario::getCategoria() const {
    return categoria;
}

int MensajePublicitario::getPrioridad() const {
    return prioridad;
}

void MensajePublicitario::setContenido(string cont) {
    if (cont.length() <= 500) { //Limite razonable
        contenido = cont;
    }
}

void MensajePublicitario::setCategoria(string cat) {
    categoria = cat;
    calcularPrioridad(); //Recalcula prioridad si cambia categoria
}

void MensajePublicitario::calcularPrioridad() {
    //AAA = alta, B = media, C = Baja
    if (categoria == "AAA") {
        prioridad = 3;
    } else if (categoria == "B") {
        prioridad = 2;
    } else {
        prioridad = 1;
    }
}

long MensajePublicitario::calcularMemoria() const {
    long memoria = sizeof(MensajePublicitario);
    memoria += contenido.length();  //texto del anuncio
    memoria += categoria.length(); //categoria textual
    return memoria;
}
