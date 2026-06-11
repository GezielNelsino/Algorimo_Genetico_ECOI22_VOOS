#pragma once
#include "../cabecalho/Aleatorio.h"
int Aleatorio::gerar_aleatorio_em_intervalo(int inicio, int fim)
{
    std::uniform_int_distribution<> distrib(inicio, fim);
    return distrib(gen);
}
int Aleatorio::gera_elemento_aleatorio(int i)
{
    return gerar_aleatorio_em_intervalo(0, (int)voos_de_volta[i].size() - 1);
}
int Aleatorio::gerar_probabilidade()
{
    std::uniform_int_distribution<> distrib(0, 100);
    return distrib(gen);
}

std::random_device Aleatorio::rd;
std::mt19937 Aleatorio::gen(Aleatorio::rd());