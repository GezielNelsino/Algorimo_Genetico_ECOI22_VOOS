#pragma once
#include "../cabecalho/Individuo.h"

Individuo::Individuo()
{
    elementos.resize(qnt_cidades - 1);
}

Individuo::Individuo(const std::vector<int> &dados)
{
    elementos.resize(qnt_cidades - 1);
    for (int i = 0; i < qnt_cidades - 1; ++i)
    {
        this->elementos[i] = dados[i];
    }
}

void Individuo::set_elemento(int i, int valor)
{
    elementos[i] = valor;
}

void Individuo::imprimir_voos_ida() const
{
    for (int i = 0; i < qnt_cidades - 1; ++i)
    {
        std::cout << voos[voos_de_ida[i][this->elementos[i]]] << std::endl;
    }
}

void Individuo::imprimir_voos_volta() const
{
    for (int i = 0; i < qnt_cidades - 1; ++i)
    {
        std::cout << voos[voos_de_volta[i][this->elementos[i]]] << std::endl;
    }
}

void Individuo::operator=(const Individuo &ind)
{
    for (int i = 0; i < qnt_cidades - 1; ++i)
    {
        this->set_elemento(i, ind.elementos[i]);
    }
}

bool operator<(const Individuo &ind1, const Individuo &ind2)
{
    return fitness(ind1) < fitness(ind2);
}

bool operator>(const Individuo &ind1, const Individuo &ind2)
{
    return fitness(ind1) > fitness(ind2);
}

Individuo Individuo::gerar_individuo_aleatorio()
{
    Individuo ind;
    for (int i = 0; i < qnt_cidades - 1; ++i)
    {
        ind.set_elemento(i, Aleatorio::gera_elemento_aleatorio(i));
    }
    return ind;
}

Individuo crossover(Individuo &ind1, Individuo &ind2)
{
    Individuo novo;
    int local_corte = Aleatorio::gerar_aleatorio_em_intervalo(1, qnt_cidades - 2);
    for (int i = 0; i < local_corte; ++i)
    {
        novo.set_elemento(i, ind1.elementos[i]);
    }
    for (int i = local_corte; i < qnt_cidades - 1; ++i)
    {
        novo.set_elemento(i, ind2.elementos[i]);
    }
    return novo;
}

Individuo mutation(Individuo &ind1)
{
    Individuo novo;
    for (int i = 0; i < qnt_cidades - 1; ++i)
    {
        novo.set_elemento(i, ind1.elementos[i]);
    }
    for (int i = 0; i < QNT_IDX_MUTADOS; ++i)
    {
        int local_mutacao = Aleatorio::gerar_aleatorio_em_intervalo(0, qnt_cidades - 2);
        novo.set_elemento(local_mutacao, Aleatorio::gera_elemento_aleatorio(local_mutacao));
    }
    return novo;
}

double fitness(const Individuo &ind)
{
    int custo_em_dinheiro =  calcula_custo_em_dinheiro_voos_volta(ind);
    int custo_em_tempo = calcula_custo_em_tempo_voos_volta(ind);
    return PESO_CUSTO * custo_em_dinheiro + PESO_TEMPO * custo_em_tempo;
}

int calcula_custo_em_dinheiro_voos_ida(const Individuo &ind)
{
    int custo_em_dinheiro = 0;
    for (int i = 0; i < qnt_cidades - 1; ++i)
    {
        custo_em_dinheiro += voos[voos_de_ida[i][ind.elementos[i]]].custo;
    }
    return custo_em_dinheiro;
}

int calcula_custo_em_dinheiro_voos_volta(const Individuo &ind)
{
    int custo_em_dinheiro = 0;
    for (int i = 0; i < qnt_cidades - 1; ++i)
    {
        custo_em_dinheiro += voos[voos_de_volta[i][ind.elementos[i]]].custo;
    }
    return custo_em_dinheiro;
}

int calcula_custo_em_tempo_voos_ida(const Individuo &ind)
{
    int indice_do_tempo_maximo = 0;
    int indice_do_tempo_minimo = 0;
    for (int i = 1; i < qnt_cidades - 1; ++i)
    {
        if (voos[voos_de_ida[i][ind.elementos[i]]].horario_fim > voos[voos_de_ida[indice_do_tempo_maximo][ind.elementos[indice_do_tempo_maximo]]].horario_fim)
        {
            indice_do_tempo_maximo = i;
        }
        else if (voos[voos_de_ida[i][ind.elementos[i]]].horario_fim < voos[voos_de_ida[indice_do_tempo_minimo][ind.elementos[indice_do_tempo_minimo]]].horario_fim)
        {
            indice_do_tempo_minimo = i;
        }
    }
    return voos[voos_de_ida[indice_do_tempo_maximo][ind.elementos[indice_do_tempo_maximo]]].horario_fim -
           voos[voos_de_ida[indice_do_tempo_minimo][ind.elementos[indice_do_tempo_minimo]]].horario_fim;
}

int calcula_custo_em_tempo_voos_volta(const Individuo &ind)
{
    int indice_do_tempo_maximo = 0;
    int indice_do_tempo_minimo = 0;
    for (int i = 1; i < qnt_cidades - 1; ++i)
    {
        if (voos[voos_de_volta[i][ind.elementos[i]]].horario_fim > voos[voos_de_volta[indice_do_tempo_maximo][ind.elementos[indice_do_tempo_maximo]]].horario_fim)
        {
            indice_do_tempo_maximo = i;
        }
        else if (voos[voos_de_volta[i][ind.elementos[i]]].horario_fim < voos[voos_de_volta[indice_do_tempo_minimo][ind.elementos[indice_do_tempo_minimo]]].horario_fim)
        {
            indice_do_tempo_minimo = i;
        }
    }
    return voos[voos_de_volta[indice_do_tempo_maximo][ind.elementos[indice_do_tempo_maximo]]].horario_fim -
           voos[voos_de_volta[indice_do_tempo_minimo][ind.elementos[indice_do_tempo_minimo]]].horario_fim;
}

std::ostream &operator<<(std::ostream &s, const Individuo &ind)
{
    std::string esp = "";
    for (int i = 0; i < qnt_cidades - 1; ++i)
    {
        s << esp << ind.elementos[i];
        esp = " ";
    }
    return s;
}
