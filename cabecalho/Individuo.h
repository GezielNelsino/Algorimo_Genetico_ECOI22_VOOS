#ifndef INDIVIDUO_H
#define INDIVIDUO_H

#include <iostream>
#include <string>
#include <vector>

#include "./state.h"
#include "../cabecalho/Aleatorio.h"
#include "../cabecalho/Voo.h"

class Individuo
{
private:
    std::vector<int> voos_de_ida_escolhidos;
    std::vector<int> voos_de_volta_escolhidos;

public:
    Individuo();

    Individuo(std::string direcao, std::vector<int> &dados);

    Individuo(std::vector<int> &dados_ida, std::vector<int> &dados_volta);

    void set_elemento(int valor, int i, std::string direcao);

    int get_elemento(int i, std::string direcao) const;

    friend std::ostream &operator<<(std::ostream &s, const Individuo &ind);

    void imprimir_voos_ida();

    void imprimir_voos_volta();

    friend Individuo crossover(const Individuo &ind1, const Individuo &ind2, std::string direcao);

    friend Individuo mutation(const Individuo &ind1, std::string direcao);

    friend double fitness(const Individuo &ind);

    friend double fitness(const Individuo &ind, std::string direcao);

    friend int calcula_custo_em_dinheiro(const Individuo &ind, std::string direcao);

    friend int calcula_custo_em_dinheiro(const Individuo &ind);

    friend int calcula_custo_em_tempo(const Individuo &ind, std::string direcao);

    friend int calcula_custo_em_tempo(const Individuo &ind);

    static Individuo gerar_individuo_aleatorio(std::string direcao);

    friend bool operator<(const Individuo &ind1, const Individuo &ind2);

    friend bool operator>(const Individuo &ind1, const Individuo &ind2);
};

#endif
