#pragma once
#include "../cabecalho/Individuo.h"

namespace
{
    void validar_direcao(std::string &direcao)
    {
        if (direcao != "ida" && direcao != "volta")
        {
            throw std::invalid_argument("Direcao deve ser ida ou volta");
        }
    }

    void validar_indice(int indice)
    {
        if (indice < 0 || indice >= qnt_cidades - 1)
        {
            throw std::out_of_range("Indice fora de faixa");
        }
    }
}

Individuo::Individuo()
{
    voos_de_ida_escolhidos.resize(qnt_cidades - 1, -1);
    voos_de_volta_escolhidos.resize(qnt_cidades - 1, -1);
}

Individuo::Individuo(std::string direcao, std::vector<int> &dados) : Individuo()
{
    validar_direcao(direcao);
    if (direcao == "ida")
    {
        voos_de_ida_escolhidos = dados;
    }
    else
    {
        voos_de_volta_escolhidos = dados;
    }
}

Individuo::Individuo(std::vector<int> &dados_ida, std::vector<int> &dados_volta)
{
    voos_de_ida_escolhidos = dados_ida;
    voos_de_volta_escolhidos = dados_volta;
}

void Individuo::set_elemento(int valor, int i, std::string direcao)
{
    validar_direcao(direcao);

    validar_indice(i);

    if (direcao == "ida")
    {
        voos_de_ida_escolhidos[i] = valor;
    }
    else
    {
        voos_de_volta_escolhidos[i] = valor;
    }
}

int Individuo::get_elemento(int i, std::string direcao) const
{
    validar_direcao(direcao);
    validar_indice(i);
    return direcao == "ida" ? voos_de_ida_escolhidos[i] : voos_de_volta_escolhidos[i];
}

void Individuo::imprimir_voos_ida()
{
    std::cout << std::endl;
    for (int i = 0; i < qnt_cidades - 1; ++i)
    {
        std::cout << voos[voos_de_ida[i][this->voos_de_ida_escolhidos[i]]] << std::endl;
    }
    std::cout << std::endl;
}

void Individuo::imprimir_voos_volta()
{
    std::cout << std::endl;
    for (int i = 0; i < qnt_cidades - 1; ++i)
    {
        std::cout << voos[voos_de_volta[i][this->voos_de_volta_escolhidos[i]]] << std::endl;
    }
    std::cout << std::endl;
}

bool operator<(const Individuo &ind1, const Individuo &ind2)
{
    return fitness(ind1) < fitness(ind2);
}

bool operator>(const Individuo &ind1, const Individuo &ind2)
{
    return fitness(ind1) > fitness(ind2);
}

Individuo Individuo::gerar_individuo_aleatorio(std::string direcao)
{
    validar_direcao(direcao);
    Individuo ind;
    for (int i = 0; i < qnt_cidades - 1; ++i)
    {
        ind.set_elemento(Aleatorio::gera_elemento_aleatorio(i, direcao), i, direcao);
    }
    return ind;
}

Individuo crossover(const Individuo &ind1, const Individuo &ind2, std::string direcao)
{
    validar_direcao(direcao);
    Individuo novo;

    const int local_corte = Aleatorio::gerar_aleatorio_em_intervalo(1, qnt_cidades - 2);

    for (int i = 0; i < local_corte; ++i)
    {
        novo.set_elemento(ind1.get_elemento(i, direcao), i, direcao);
    }
    for (int i = local_corte; i < qnt_cidades - 1; ++i)
    {
        novo.set_elemento(ind2.get_elemento(i, direcao), i, direcao);
    }
    return novo;
}

Individuo mutation(const Individuo &ind1, std::string direcao)
{
    validar_direcao(direcao);
    Individuo novo = ind1;
    for (int i = 0; i < QNT_IDX_MUTADOS; ++i)
    {
        int local_mutacao = Aleatorio::gerar_aleatorio_em_intervalo(0, qnt_cidades - 2);
        novo.set_elemento(Aleatorio::gera_elemento_aleatorio(local_mutacao, direcao), local_mutacao, direcao);
    }
    return novo;
}

double fitness(const Individuo &ind)
{
    return fitness(ind, "ida") + fitness(ind, "volta");
}

double fitness(const Individuo &ind, std::string direcao)
{
    int custo_em_dinheiro = calcula_custo_em_dinheiro(ind, direcao);
    int custo_em_tempo = calcula_custo_em_tempo(ind, direcao);
    return PESO_CUSTO * custo_em_dinheiro + PESO_TEMPO * custo_em_tempo;
}

int calcula_custo_em_dinheiro(const Individuo &ind, std::string direcao)
{
    validar_direcao(direcao);
    const auto &indices_voos = direcao == "ida" ? voos_de_ida : voos_de_volta;
    const auto &voos_escolhidos = direcao == "ida" ? ind.voos_de_ida_escolhidos : ind.voos_de_volta_escolhidos;
    int custo_em_dinheiro = 0;
    for (int i = 0; i < qnt_cidades - 1; ++i)
    {
        custo_em_dinheiro += voos[indices_voos[i][voos_escolhidos[i]]].custo;
    }
    return custo_em_dinheiro;
}

int calcula_custo_em_dinheiro(const Individuo &ind)
{
    const auto &indices_voos_ida = voos_de_ida;
    const auto &indices_voos_volta = voos_de_volta;

    const auto &voo_ida = ind.voos_de_ida_escolhidos;
    const auto &voo_volta = ind.voos_de_volta_escolhidos;

    int custo_em_dinheiro_ida = 0;
    int custo_em_dinheiro_volta = 0;
    for (int i = 0; i < qnt_cidades - 1; ++i)
    {
        custo_em_dinheiro_ida += voos[indices_voos_ida[i][voo_ida[i]]].custo;

        custo_em_dinheiro_volta += voos[indices_voos_volta[i][voo_volta[i]]].custo;
    }

    return custo_em_dinheiro_ida + custo_em_dinheiro_volta;
}

int calcula_custo_em_tempo(const Individuo &ind, std::string direcao)
{
    validar_direcao(direcao);

    int indice_do_tempo_maximo = 0;
    int indice_do_tempo_minimo = 0;

    const auto &voos_direcao = direcao == "ida" ? voos_de_ida : voos_de_volta;
    const auto &voos_escolhidos = direcao == "ida" ? ind.voos_de_ida_escolhidos : ind.voos_de_volta_escolhidos;

    Horario horario_fim_voo_escolhido;
    Horario horario_fim_tempo_maximo;
    Horario horario_fim_tempo_minimo;

    for (int i = 1; i < qnt_cidades - 1; ++i)
    {
        horario_fim_voo_escolhido = voos[voos_direcao[i][voos_escolhidos[i]]].horario_fim;
        horario_fim_tempo_maximo = voos[voos_direcao[indice_do_tempo_maximo][voos_escolhidos[indice_do_tempo_maximo]]].horario_fim;
        horario_fim_tempo_minimo = voos[voos_direcao[indice_do_tempo_minimo][voos_escolhidos[indice_do_tempo_minimo]]].horario_fim;

        if (horario_fim_voo_escolhido > horario_fim_tempo_maximo)
        {
            indice_do_tempo_maximo = i;
        }
        if (horario_fim_voo_escolhido < horario_fim_tempo_minimo)
        {
            indice_do_tempo_minimo = i;
        }
    }

    return horario_fim_tempo_maximo - horario_fim_tempo_minimo;
}

int calcula_custo_em_tempo(const Individuo &ind)
{

    int indice_do_tempo_maximo_ida = 0;
    int indice_do_tempo_minimo_ida = 0;

    int indice_do_tempo_maximo_volta = 0;
    int indice_do_tempo_minimo_volta = 0;

    Horario horario_fim_voo_escolhido_ida;
    Horario horario_fim_tempo_maximo_ida;
    Horario horario_fim_tempo_minimo_ida;

    Horario horario_fim_voo_escolhido_volta;
    Horario horario_fim_tempo_maximo_volta;
    Horario horario_fim_tempo_minimo_volta;

    for (int i = 1; i < qnt_cidades - 1; ++i)
    {
        horario_fim_voo_escolhido_ida = voos[voos_de_ida[i][ind.voos_de_ida_escolhidos[i]]].horario_fim;
        horario_fim_tempo_maximo_ida = voos[voos_de_ida[indice_do_tempo_maximo_ida][ind.voos_de_ida_escolhidos[indice_do_tempo_maximo_ida]]].horario_fim;
        horario_fim_tempo_minimo_ida = voos[voos_de_ida[indice_do_tempo_minimo_ida][ind.voos_de_ida_escolhidos[indice_do_tempo_minimo_ida]]].horario_fim;

        horario_fim_voo_escolhido_volta = voos[voos_de_volta[i][ind.voos_de_volta_escolhidos[i]]].horario_fim;
        horario_fim_tempo_maximo_volta = voos[voos_de_volta[indice_do_tempo_maximo_volta][ind.voos_de_volta_escolhidos[indice_do_tempo_maximo_volta]]].horario_fim;
        horario_fim_tempo_minimo_volta = voos[voos_de_volta[indice_do_tempo_minimo_volta][ind.voos_de_volta_escolhidos[indice_do_tempo_minimo_volta]]].horario_fim;

        if (horario_fim_voo_escolhido_ida > horario_fim_tempo_maximo_ida)
        {
            indice_do_tempo_maximo_ida = i;
        }
        if (horario_fim_voo_escolhido_volta > horario_fim_tempo_maximo_volta)
        {
            indice_do_tempo_maximo_volta = i;
        }

        if (horario_fim_voo_escolhido_ida < horario_fim_tempo_minimo_ida)
        {
            indice_do_tempo_minimo_ida = i;
        }
        if (horario_fim_voo_escolhido_volta < horario_fim_tempo_minimo_volta)
        {
            indice_do_tempo_minimo_volta = i;
        }
    }

    return (horario_fim_tempo_maximo_ida - horario_fim_tempo_minimo_ida) + (horario_fim_tempo_maximo_volta - horario_fim_tempo_minimo_volta);
}

std::ostream &operator<<(std::ostream &s, const Individuo &ind)
{
    s << "ida: [";
    for (int i = 0; i < qnt_cidades - 1; ++i)
    {
        s << (i == 0 ? "" : " ") << ind.voos_de_ida_escolhidos[i];
    }
    s << "] volta: [";
    for (int i = 0; i < qnt_cidades - 1; ++i)
    {
        s << (i == 0 ? "" : " ") << ind.voos_de_volta_escolhidos[i];
    }
    s << "]";
    return s;
}
