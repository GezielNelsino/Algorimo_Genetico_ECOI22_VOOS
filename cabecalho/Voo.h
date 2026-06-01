#ifndef VOO_H
#define VOO_H

#include<unordered_map>

#include "../classes/Horario.cpp"

class Voo
{

public:

    std::string sigla_origem;

    std::string sigla_destino;

    Horario horario_inicio;

    Horario horario_fim;

    int custo;

    Voo(std::string &_sigla_origem, std::string &_sigla_destino, std::string &_horario_inicio, std::string &_horario_fim, int &_custo);

    bool siglas_nao_sao_validas();

    bool horario_nao_eh_valido();

    friend std::ostream &operator<<(std::ostream &s, const Voo &voo);

};

#endif