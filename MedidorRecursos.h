/*
 * MedidorRecursos.h
 *
 * Mide el consumo de recursos del sistema.
 * Cuenta iteraciones y calcula memoria usada.
 */

#ifndef MEDIDORRECURSOS_H
#define MEDIDORRECURSOS_H

class Sistema;

//Clase estática para medir rendimiento y consumo de memoria
class MedidorRecursos {
private:
    static long contadorIteraciones;    //número de operaciones realizadas
    static long memoriaTotal;           //memoria total estimada en bytes

public:
    static void reiniciarContador();        //Reinicia el contador de iteraciones
    static void incrementarIteraciones();   //Incrementa el contador en una unidad
    static long getIteraciones();           //Devuelve el total de iteraciones realizadas

    static void calcularMemoriaTotal(Sistema* sistema); //Calcula la memoria total usada por el sistema
    static long getMemoriaTotal();          //Devuelve la memoria total estimada

    static void mostrarReporte();           //Muestra un reporte en consola con iteraciones y memoria
};

#endif
