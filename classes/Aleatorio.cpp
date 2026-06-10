#pragma once
#include "../cabecalho/Aleatorio.h"

int Aleatorio::gerar_aleatorio_em_intervalo(int inicio, int fim)
{
    std::uniform_int_distribution<> distrib(inicio, fim);
    return distrib(gen);
}

int Aleatorio::gera_elemento_aleatorio(int i, std::string direcao)
{
    if (direcao != "ida" && direcao != "volta")
    {
        throw std::invalid_argument("Direcao deve ser ida ou volta");
    }

    const std::vector<std::vector<int>> *voos_por_cidade = nullptr;

    voos_por_cidade = direcao == "ida" ? &voos_de_ida : &voos_de_volta;

    if (i < 0 || i >= (int)voos_por_cidade->size() || (*voos_por_cidade)[i].empty())
    {
        throw std::out_of_range("Cidade sem voos disponiveis para a direcao informada");
    }

    return gerar_aleatorio_em_intervalo(0, (int)(*voos_por_cidade)[i].size() - 1);
}

int Aleatorio::gerar_probabilidade()
{
    std::uniform_int_distribution<> distrib(0, 100);
    return distrib(gen);
}

std::random_device Aleatorio::rd;
std::mt19937 Aleatorio::gen(Aleatorio::rd());
