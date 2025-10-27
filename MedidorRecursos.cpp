#include "MedidorRecursos.h"
#include "Sistema.h"
#include <iostream>
using namespace std;

long MedidorRecursos::contadorIteraciones = 0;
long MedidorRecursos::memoriaTotal = 0;

//Reinicia el contador de iteraciones
void MedidorRecursos::reiniciarContador() { contadorIteraciones = 0; }
//Incrementa el contador en una unidad
void MedidorRecursos::incrementarIteraciones() { contadorIteraciones++; }
//Devuelve el total de iteraciones realizadas
long MedidorRecursos::getIteraciones() { return contadorIteraciones; }

//Calcula la memoria total usada por el sistema
void MedidorRecursos::calcularMemoriaTotal(Sistema* sistema) {
    if (!sistema) { memoriaTotal = 0; return; }
    memoriaTotal = sistema->calcularMemoriaTotal();
    //Colchón estimado por variables locales/parámetros por valor
    memoriaTotal += 256;
}
//Devuelve la memoria total estimada
long MedidorRecursos::getMemoriaTotal() { return memoriaTotal; }

void MedidorRecursos::mostrarReporte() {
    cout << "\n========================================" << endl;
    cout << "     REPORTE DE CONSUMO DE RECURSOS" << endl;
    cout << "========================================" << endl;
    cout << "Iteraciones ejecutadas: " << contadorIteraciones << endl;
    cout << "Memoria total consumida: " << memoriaTotal << " bytes" << endl;
    cout << "Memoria en KB: " << (memoriaTotal / 1024.0) << " KB" << endl;
    cout << "Memoria en MB: " << (memoriaTotal / (1024.0 * 1024.0)) << " MB" << endl;
    cout << "========================================\n" << endl;
}
