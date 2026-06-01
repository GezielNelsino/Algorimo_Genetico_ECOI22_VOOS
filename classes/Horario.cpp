#pragma once
#include "../cabecalho/Horario.h"

Horario::Horario() {}

Horario::Horario(const std::string &palavra)
{
    std::stringstream k(palavra);
    char lixo;
    k >> this->hora;
    k >> lixo;
    k >> this->minuto;
}

bool Horario::verificar_se_eh_horario_invalido(const std::string &palavra)
{
    std::stringstream k(palavra);
    int tmp;
    char lixo;
    k >> tmp;
    if (k.fail() || tmp > 23 || tmp < 0)
    {
        return true;
    }
    k >> lixo;
    if (k.fail())
    {
        return true;
    }
    k >> tmp;
    if (k.fail() || tmp > 59 || tmp < 0)
    {
        return true;
    }
    return false;
}

std::ostream &operator<<(std::ostream &s, const Horario &horario)
{
    if (horario.hora < 10)
    {
        s << "0";
    }
    s << horario.hora;
    s << ":";
    if (horario.minuto < 10)
    {
        s << "0";
    }
    s << horario.minuto;
    return s;
}

bool operator>(const Horario &horario1, const Horario &horario2)
{
    if (horario1.hora < horario2.hora)
    {
        return false;
    }
    if (horario1.hora == horario2.hora)
    {
        return horario1.minuto > horario2.minuto;
    }
    return true;
}

bool operator<(const Horario &horario1, const Horario &horario2)
{
    if (horario1.hora > horario2.hora)
    {
        return false;
    }
    if (horario1.hora == horario2.hora)
    {
        return horario1.minuto < horario2.minuto;
    }
    return true;
}

int operator-(const Horario &horario1, const Horario &horario2)
{
    int tempo1 = horario1.hora * 60 + horario1.minuto;
    int tempo2 = horario2.hora * 60 + horario2.minuto;
    return tempo1 - tempo2;
}
