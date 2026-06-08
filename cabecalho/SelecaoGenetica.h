#ifndef SELECAO_GENETICA_H
#define SELECAO_GENETICA_H

#include <vector>
#include <tuple>
#include <iomanip>
#include "../cabecalho/Individuo.h"

class SelecaoGenetica
{
private:
    std::vector<Individuo> individuos;

public:
    SelecaoGenetica();

    Individuo selecionar_individuo_por_torneio();

    Individuo selecionar_melhor_individuo();

    int selecionar_indice_pior_individuo();

    std::vector<Individuo> gerar_nova_populacao();

    void imprimir_individuos();

    void imprimir_melhor_da_geracao(int indice_geracao);

    void imprimir_melhor_da_geracao_em_colunas();

    void executar();

    void executar_algoritmo_ate_encontrar_otimo(double otimo);

    double selecionar_melhor_fitness();

    double selecionar_pior_fitness();

    double selecionar_fitness_medio();

    double selecionar_variancia_populacional_fitness();

    std::tuple<double, double, double, double, double> selecionar_melhor_pior_media_variancia_fitness_geracao();
};

#endif
