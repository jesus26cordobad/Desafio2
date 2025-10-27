/*
 * Utilidades.h
 *
 * Funciones auxiliares para el sistema.
 */

#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <string>
using namespace std;

//Funciones generales para limpieza, conversión y formato
class Utilidades {
public:
    static string trim(const string& str);                      //elimina espacios extremos
    static string toLowerCase(const string& str);               //convierte a minúsculas
    static string toUpperCase(const string& str);               //convierte a mayúsculas
    static int stringToInt(const string& str);                  //convierte string a int
    static long stringToLong(const string& str);                //convierte string a long
    static string intToString(int num);                         //convierte int a string
    static string longToString(long num);                       //convierte long a string
    static bool esNumero(const string& str);                    //valida si es número
    static string reemplazarEspacios(const string& str, char reemplazo);    //reemplaza espacios por otro carácter
    static string normalizarRuta(const string& ruta);           //estandariza rutas con slashes
    static std::string formatearIdCancion9(long id);            //convierte ID a formato AAAAA-BB-CC
};

#endif
