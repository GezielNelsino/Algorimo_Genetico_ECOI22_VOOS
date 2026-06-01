#ifndef STATE_H
#define STATE_H

#define PESO_CUSTO 1
#define PESO_TEMPO 0.1
#define TAM_POPULACAO 20
#define NUM_GERACOES 100
#define PROB_MIN_CRUZAMENTO 70
#define PROB_MIN_MUTACAO 5
#define MEMBROS_TORNEIO 10
#define PARAM_TORNEIO 80
#define QNT_IDX_MUTADOS 1

class Voo;

std::unordered_map<std::string, int> indice_cidade;
std::vector<std::string> nome_cidade;
std::vector<std::string> sigla_cidade;
std::vector<Voo> voos;
std::vector<std::vector<int>> voos_de_ida;
std::vector<std::vector<int>> voos_de_volta;
int qnt_cidades;

#endif