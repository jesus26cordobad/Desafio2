#include "Utilidades.h"
#include <sstream>

//Elimina espacios, tabs y saltos de línea al inicio y final
string Utilidades::trim(const string& str) {
    size_t inicio = 0;
    size_t fin = str.length();

    while (inicio < fin && (str[inicio] == ' ' || str[inicio] == '\t' || str[inicio] == '\n')) {
        inicio++;
    }

    while (fin > inicio && (str[fin - 1] == ' ' || str[fin - 1] == '\t' || str[fin - 1] == '\n')) {
        fin--;
    }

    return str.substr(inicio, fin - inicio);
}
//Convierte a minúsculas carácter por carácter
string Utilidades::toLowerCase(const string& str) {
    string resultado = str;

    for (size_t i = 0; i < resultado.length(); i++) {
        if (resultado[i] >= 'A' && resultado[i] <= 'Z') {
            resultado[i] = resultado[i] + 32;
        }
    }

    return resultado;
}
//Convierte a mayúsculas carácter por carácter
string Utilidades::toUpperCase(const string& str) {
    string resultado = str;

    for (size_t i = 0; i < resultado.length(); i++) {
        if (resultado[i] >= 'a' && resultado[i] <= 'z') {
            resultado[i] = resultado[i] - 32;
        }
    }

    return resultado;
}
// Convierte string a int (maneja signo)
int Utilidades::stringToInt(const string& str) {
    int resultado = 0;
    int signo = 1;
    size_t inicio = 0;

    if (str.length() > 0 && str[0] == '-') {
        signo = -1;
        inicio = 1;
    }

    for (size_t i = inicio; i < str.length(); i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            resultado = resultado * 10 + (str[i] - '0');
        } else {
            break;
        }
    }

    return resultado * signo;
}
//Convierte string a long (maneja signo)
long Utilidades::stringToLong(const string& str) {
    long resultado = 0;
    int signo = 1;
    size_t inicio = 0;

    if (str.length() > 0 && str[0] == '-') {
        signo = -1;
        inicio = 1;
    }

    for (size_t i = inicio; i < str.length(); i++) {
        if (str[i] >= '0' && str[i] <= '9') {
            resultado = resultado * 10 + (str[i] - '0');
        } else {
            break;
        }
    }

    return resultado * signo;
}
//Convierte int a string
string Utilidades::intToString(int num) {
    if (num == 0) {
        return "0";
    }

    string resultado = "";
    bool negativo = num < 0;

    if (negativo) {
        num = -num;
    }

    while (num > 0) {
        char digito = '0' + (num % 10);
        resultado = digito + resultado;
        num = num / 10;
    }

    if (negativo) {
        resultado = "-" + resultado;
    }

    return resultado;
}
//Convierte long a string
string Utilidades::longToString(long num) {
    if (num == 0) {
        return "0";
    }

    string resultado = "";
    bool negativo = num < 0;

    if (negativo) {
        num = -num;
    }

    while (num > 0) {
        char digito = '0' + (num % 10);
        resultado = digito + resultado;
        num = num / 10;
    }

    if (negativo) {
        resultado = "-" + resultado;
    }

    return resultado;
}
//Verifica si el string representa un número válido
bool Utilidades::esNumero(const string& str) {
    if (str.length() == 0) {
        return false;
    }

    size_t inicio = 0;

    if (str[0] == '-' || str[0] == '+') {
        inicio = 1;
    }

    if (inicio >= str.length()) {
        return false;
    }

    for (size_t i = inicio; i < str.length(); i++) {
        if (str[i] < '0' || str[i] > '9') {
            return false;
        }
    }

    return true;
}
//Reemplaza espacios por el carácter indicado
string Utilidades::reemplazarEspacios(const string& str, char reemplazo) {
    string resultado = str;

    for (size_t i = 0; i < resultado.length(); i++) {
        if (resultado[i] == ' ') {
            resultado[i] = reemplazo;
        }
    }

    return resultado;
}
// Normaliza rutas de archivo:
// - Convierte backslashes a slashes
// - Asegura que termine en "/"
string Utilidades::normalizarRuta(const string& ruta) {
    string rutaNormalizada = ruta;

    for (size_t i = 0; i < rutaNormalizada.length(); i++) {
        if (rutaNormalizada[i] == '\\') {
            rutaNormalizada[i] = '/';
        }
    }

    if (rutaNormalizada.length() > 0 && rutaNormalizada[rutaNormalizada.length() - 1] != '/') {
        rutaNormalizada += '/';
    }

    return rutaNormalizada;
}
// Formatea un ID numérico como AAAAA-BB-CC (9 dígitos con guiones)
// Ejemplo: 123456789 → "12345-67-89"
std::string Utilidades::formatearIdCancion9(long id) {
    // Asegura 9 dígitos con ceros a la izquierda
    long x = id;
    if (x < 0) x = -x;
    char tmp[10] = {0}; // 9 + '\0'
    for (int i = 8; i >= 0; --i) { tmp[i] = char('0' + (x % 10)); x /= 10; }
    std::string s(tmp, 9); // AAAAA BB CC

    // Retorna AAAAA-BB-CC
    return s.substr(0,5) + "-" + s.substr(5,2) + "-" + s.substr(7,2);
}
