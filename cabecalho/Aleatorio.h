#ifndef ALEATORIO_H
#define ALEATORIO_H

#include <random>
#include <vector>
#include "./state.h"

class Aleatorio
{
public:
    static std::random_device rd;
    static std::mt19937 gen;
    static int gerar_aleatorio_em_intervalo(int inicio, int fim);
    static int gera_elemento_aleatorio(int i);
    static int gerar_probabilidade();
};

#endif