#pragma once
#include "../cabecalho/Voo.h"

Voo::Voo(std::string &_sigla_origem, std::string &_sigla_destino, std::string &_horario_inicio, std::string &_horario_fim, int &_custo) : sigla_origem(_sigla_origem),
                                                                                                                      sigla_destino(_sigla_destino),
                                                                                                                      horario_inicio(Horario(_horario_inicio)),
                                                                                                                      horario_fim(Horario(_horario_fim)),
                                                                                                                      custo(_custo)
{
}

bool Voo::siglas_nao_sao_validas()
{
    return indice_cidade.find(this->sigla_origem) == indice_cidade.end() || indice_cidade.find(this->sigla_destino) == indice_cidade.end();
}

bool Voo::horario_nao_eh_valido()
{
    return this->horario_inicio > this->horario_fim;
}

std::ostream &operator<<(std::ostream &s, const Voo &voo)
{
    s << voo.custo << " " << voo.horario_inicio << " -> " << voo.horario_fim;
    return s;
}