#ifndef HORARIO_H
#define HORARIO_H  

#include <iostream>
#include <sstream>

class Horario
{
public:
    int hora;
    int minuto;
    Horario();
    Horario(const std::string &palavra);
    static bool verificar_se_eh_horario_invalido(const std::string &palavra);
    friend std::ostream &operator<<(std::ostream &s, const Horario &horario);
    friend bool operator>(const Horario &horario1, const Horario &horario2);
    friend bool operator<(const Horario &horario1, const Horario &horario2);
    friend int operator-(const Horario &horario1, const Horario &horario2);
};

#endif