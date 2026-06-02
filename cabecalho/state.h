#ifndef STATE_H
#define STATE_H

#include <unordered_map>
#include <vector>

#define PESO_CUSTO 1
#define PESO_TEMPO 1
#define TAM_POPULACAO 200
#define NUM_GERACOES 700
#define PROB_MIN_CRUZAMENTO 80
#define PROB_MIN_MUTACAO 10
#define MEMBROS_TORNEIO 5
#define PARAM_TORNEIO 80
#define QNT_IDX_MUTADOS 3

class Voo;

std::unordered_map<std::string, int> indice_cidade;
std::vector<std::string> nome_cidade;
std::vector<std::string> sigla_cidade;
std::vector<Voo> voos;
std::vector<std::vector<int>> voos_de_ida;
std::vector<std::vector<int>> voos_de_volta;
int qnt_cidades;

#endif