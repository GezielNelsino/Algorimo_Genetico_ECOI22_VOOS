#pragma once
#include "../cabecalho/StatusLeituraDeDados.h"

StatusLeituraDeDados::StatusLeituraDeDados(int _codigo, std::string _mensagem) : codigo(_codigo), mensagem(_mensagem) {}

std::ostream &operator<<(std::ostream &s, const StatusLeituraDeDados &status_leitura_de_dados)
{
    s << status_leitura_de_dados.mensagem;
    return s;
}

bool StatusLeituraDeDados::eh_codigo_de_erro()
{
    return codigo != 200;
}
