#include "Colaborador.h"

Colaborador::Colaborador()
    : nombre(""), apellido(""), lemaario(""), id("") {}

Colaborador::Colaborador(const string& nom, const string& ape, const string& lema, const string& id)
    : nombre(nom), apellido(ape), lemaario(lema), id(id) {}

string Colaborador::getNombre() const { return nombre; }
string Colaborador::getApellido() const { return apellido; }
string Colaborador::getLemaario() const { return lemaario; }
string Colaborador::getId() const { return id; }

void Colaborador::setNombre(const string& nom) { nombre = nom; }
void Colaborador::setApellido(const string& ape) { apellido = ape; }
void Colaborador::setLemaario(const string& lema) { lemaario = lema; }
void Colaborador::setId(const string& idNuevo) { id = idNuevo; }

long Colaborador::calcularMemoria() const {
    //Calcula memoria aproximada incluyendo strings internos
    long memoria = sizeof(Colaborador);
    memoria += static_cast<long>(nombre.capacity());
    memoria += static_cast<long>(apellido.capacity());
    memoria += static_cast<long>(lemaario.capacity());
    memoria += static_cast<long>(id.capacity());
    return memoria;
}
