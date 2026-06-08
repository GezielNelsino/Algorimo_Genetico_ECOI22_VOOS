#ifndef INDIVIDUO_H
#define INDIVIDUO_H

#include <vector>
#include <iostream>

#include "./state.h"
#include "../cabecalho/Aleatorio.h"
#include "../cabecalho/Voo.h"

class Individuo
{
private:
    std::vector<int> elementos;

public:
    Individuo();

    Individuo(const std::vector<int> &dados);

    void set_elemento(int i, int valor);

    friend std::ostream &operator<<(std::ostream &s, const Individuo &ind);

    void imprimir_voos_ida() const;

    void imprimir_voos_volta() const;

    friend Individuo crossover(Individuo &ind1, Individuo &ind2);

    friend Individuo mutation(Individuo &ind1);

    friend double fitness(const Individuo &ind);

    void operator=(const Individuo &ind);

    friend int calcula_custo_em_dinheiro_voos_ida(const Individuo &ind);

    friend int calcula_custo_em_dinheiro_voos_volta(const Individuo &ind);

    friend int calcula_custo_em_tempo_voos_ida(const Individuo &ind);

    friend int calcula_custo_em_tempo_voos_volta(const Individuo &ind);

    static Individuo gerar_individuo_aleatorio();

    friend bool operator<(const Individuo &ind1, const Individuo &ind2);

    friend bool operator>(const Individuo &ind1, const Individuo &ind2);
};

#endif