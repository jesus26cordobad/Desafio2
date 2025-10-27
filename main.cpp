// main.cpp
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstdint>
#include <chrono>
#include "Sistema.h"
using namespace std;

//Inicializa el generador aleatorio con entropía mixta y calentamiento
static void seedAleatorioUnaSolaVez() {
    static bool hecho = false;
    if (hecho) return;

    //Mezcla múltiples fuentes de entropía
    uintptr_t p1 = (uintptr_t)&hecho;
    auto tick = std::chrono::high_resolution_clock::now().time_since_epoch().count();

    unsigned s = 0u;
    s ^= (unsigned)time(0);
    s ^= (unsigned)clock();
    s ^= (unsigned)(p1 >> 4);
    s ^= (unsigned)(tick & 0xFFFFFFFFu);
    s ^= (unsigned)((tick >> 32) & 0xFFFFFFFFu);

    //Mezcla adicional con XORshift
    s ^= s << 13;
    s ^= s >> 17;
    s ^= s << 5;

    //Aplica la mezcla varias veces para mayor dispersión
    for (int i = 0; i < 10; i++) {
        s ^= s << 13;
        s ^= s >> 17;
        s ^= s << 5;
    }

    srand(s);   //inicializa rand()

    //"Calienta" el generador descartando algunos valores
    for (int i = 0; i < 100; i++) {
        rand();
    }

    hecho = true;
}

int main() {
    seedAleatorioUnaSolaVez();             //asegura aleatoriedad robusta

    Sistema* sistema = new Sistema();      //instancia principal del sistema
    sistema->iniciar();                    //lanza el flujo principal
    delete sistema;                        //libera recursos

    return 0;
}
