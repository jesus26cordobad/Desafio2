#ifndef COLABORADOR_H
#define COLABORADOR_H

#include <string>
using namespace std;

//Representa un colaborador musical (productor o compositor)
class Colaborador {
private:
    string nombre;
    string apellido;
    string lemaario; //frase distintiva del colaborador
    string id;       //identificador único

public:
    Colaborador();
    Colaborador(const string& nom, const string& ape, const string& lema, const string& id);

    // Getters
    string getNombre() const;
    string getApellido() const;
    string getLemaario() const;
    string getId() const;

    // Setters
    void setNombre(const string& nom);
    void setApellido(const string& ape);
    void setLemaario(const string& lema);
    void setId(const string& id);

    // Métrica
    long calcularMemoria() const;
};

#endif
