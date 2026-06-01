#ifndef VOO_H
#define VOO_H

#include "../classes/Horario.cpp"
#include<unordered_map>


class Voo
{
public:
    std::string sigla_origem;
    std::string sigla_destino;
    Horario horario_inicio;
    Horario horario_fim;
    int custo;
    Voo(std::string &_sigla_origem, std::string &_sigla_destino, std::string &_horario_inicio, std::string &_horario_fim, int &_custo);
    bool siglas_nao_sao_validas(std::unordered_map<std::string, int> &indice_cidade);
    bool horario_nao_eh_valido();
    friend std::ostream &operator<<(std::ostream &s, const Voo &voo);
};

#endif