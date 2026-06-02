#include <iostream>
#include <fstream>
#include <vector>

#include "./cabecalho/state.h"
#include "./classes/Voo.cpp"
#include "./classes/StatusLeituraDeDados.cpp"
#include "./classes/Aleatorio.cpp"
#include "./classes/Individuo.cpp"
#include "./classes/SelecaoGenetica.cpp"

using namespace std;

/* Leitura e validacao de dados */

void criar_cidade(string &sigla, string &nome)
{
    indice_cidade[sigla] = indice_cidade.size();
    nome_cidade.push_back(nome);
    sigla_cidade.push_back(sigla);
}

void inicializar_vetores_de_voos()
{
    qnt_cidades = (int)indice_cidade.size();
    voos_de_ida = vector<vector<int>>(qnt_cidades - 1);
    voos_de_volta = vector<vector<int>>(qnt_cidades - 1);
}

void inserir_na_lista_de_voos(Voo &voo, int &indice)
{
    if (voo.sigla_origem != "FCO")
    {
        voos_de_ida[indice_cidade[voo.sigla_origem]].push_back(indice);
    }
    else
    {
        voos_de_volta[indice_cidade[voo.sigla_destino]].push_back(indice);
    }
}

StatusLeituraDeDados ler_arquivo_de_cidades()
{
    ifstream arq_cidades("entrada/cidades.txt");
    if (arq_cidades.is_open())
    {
        int indice = 0;
        string cidade_e_sigla;
        while (getline(arq_cidades, cidade_e_sigla))
        {
            string cidade;
            string sigla;
            stringstream k(cidade_e_sigla);
            k >> sigla;
            k >> cidade;
            if (sigla.empty())
            {
                return StatusLeituraDeDados(407, "Falha ao extrair sigla no arquivo de Cidades");
            }
            if (cidade.empty())
            {
                return StatusLeituraDeDados(408, "Falha ao extrair nome no arquivo de Cidades");
            }
            if (sigla == "FCO")
            {
                continue;
            }
            criar_cidade(sigla, cidade);
        }
        arq_cidades.close();
    }
    else
    {
        return StatusLeituraDeDados(405, "Falha ao abrir o arquivo de Cidades");
    }
    if (indice_cidade.empty())
    {
        return StatusLeituraDeDados(406, "Nenhuma cidade cadastrada alem de Roma");
    }
    return StatusLeituraDeDados(200, "Ok");
}

StatusLeituraDeDados ler_arquivo_de_voos()
{
    ifstream arq_voos("entrada/flights.txt");
    if (arq_voos.is_open())
    {
        int indice = 0;
        string str_voo;
        while (getline(arq_voos, str_voo))
        {
            stringstream k(str_voo);
            string sigla_origem, sigla_destino;
            string horario_inicio, horario_fim;
            int custo;
            getline(k, sigla_origem, ',');
            getline(k, sigla_destino, ',');
            getline(k, horario_inicio, ',');
            getline(k, horario_fim, ',');

            if (!(k >> custo))
            {
                return StatusLeituraDeDados(409, "Erro ao extrair dados de algum voo");
            }
            if (Horario::verificar_se_eh_horario_invalido(horario_inicio))
            {
                return StatusLeituraDeDados(410, "Horario de inicio do voo " + to_string(indice) + " em formato invalido");
            }
            if (Horario::verificar_se_eh_horario_invalido(horario_fim))
            {
                return StatusLeituraDeDados(411, "Horario de fim do voo " + to_string(indice) + " em formato invalido");
            }
            Voo voo(sigla_origem, sigla_destino, horario_inicio, horario_fim, custo);
            if (voo.siglas_nao_sao_validas())
            {
                return StatusLeituraDeDados(401, "A sigla do voo " + to_string(indice) + " nao eh valida");
            }
            if (voo.horario_nao_eh_valido())
            {
                return StatusLeituraDeDados(402, "O horario do voo " + to_string(indice) + " nao eh valido");
            }
            voos.push_back(voo);
            inserir_na_lista_de_voos(voo, indice);
            ++indice;
        }
        arq_voos.close();
    }
    else
    {
        return StatusLeituraDeDados(403, "Falha ao abrir o arquivo de Voos");
    }

    for (int i = 0; i < qnt_cidades; i++)
    {
        if (sigla_cidade[i] == "FCO")
        {
            continue;
        }
        if (voos_de_volta[i].empty() || voos_de_ida[i].empty())
        {
            return StatusLeituraDeDados(404, "Alguma(s) cidade(s) nao tem nenhum voo para roma");
        }
    }
    return StatusLeituraDeDados(200, "Ok");
}

StatusLeituraDeDados validar_entrada_de_dados()
{
    StatusLeituraDeDados status_leitura_cidades = ler_arquivo_de_cidades();
    if (status_leitura_cidades.eh_codigo_de_erro())
    {
        return StatusLeituraDeDados(status_leitura_cidades.codigo, status_leitura_cidades.mensagem);
    }
    criar_cidade(*(new string("FCO")), *(new string("Rome")));
    inicializar_vetores_de_voos();
    StatusLeituraDeDados status_leitura_voos = ler_arquivo_de_voos();
    if (status_leitura_voos.eh_codigo_de_erro())
    {
        return StatusLeituraDeDados(status_leitura_voos.codigo, status_leitura_voos.mensagem);
    }
    return StatusLeituraDeDados(200, "Dados Lidos Com Sucesso");
}

/* Selecao de melhor individuo com brute force (Apenas para comparativo) */

void permutar_e_analisar(int p, double &x, vector<int> &dados, Individuo &bind)
{
    if (p == qnt_cidades - 1)
    {
        Individuo ind(dados);
        double fit = fitness(ind);
        if (fit < x)
        {
            x = fit;
            bind = ind;
        }
        return;
    }
    for (int i = 0; i < (int)voos_de_ida[p].size(); ++i)
    {
        dados[p] = i;
        permutar_e_analisar(p + 1, x, dados, bind);
    }
}

void gerar_melhor_individuo_brute_force()
{
    vector<int> dados(qnt_cidades - 1);
    vector<int> inicial(qnt_cidades - 1, 0);
    Individuo best_ind(inicial);
    double best_fitness = fitness(best_ind);
    permutar_e_analisar(0, best_fitness, dados, best_ind);
    cout << best_ind << endl;
    cout << best_fitness << endl;
    best_ind.imprimir_voos();
}

double gerar_melhor_fitness_brute_force()
{
    vector<int> dados(qnt_cidades - 1);
    vector<int> inicial(qnt_cidades - 1, 0);
    Individuo best_ind(inicial);
    double best_fitness = fitness(best_ind);
    permutar_e_analisar(0, best_fitness, dados, best_ind);
    return best_fitness;
}

/* Area de teste */

void imprimir_voos_de_ida_cidades()
{
    for (int i = 0; i < qnt_cidades - 1; i++)
    {
        cout << nome_cidade[i] << " -> " << "Roma" << endl;
        for (auto &j : voos_de_ida[i])
        {
            cout << voos[j] << endl;
        }
    }
}

void gerar_e_cruzar_dois_individuos_aleatorios()
{
    Individuo ind1 = Individuo::gerar_individuo_aleatorio();
    Individuo ind2 = Individuo::gerar_individuo_aleatorio();
    Individuo ind3 = crossover(ind1, ind2);
    cout << ind1 << endl;
    cout << ind2 << endl;
    cout << ind3 << endl;
    cout << fitness(ind1) << endl;
    cout << fitness(ind2) << endl;
    cout << fitness(ind3) << endl;
}

void executar_algoritmo_genetico()
{
    SelecaoGenetica selecao;
    selecao.executar();
}

/* Main */

int main()
{
    StatusLeituraDeDados status_entrada_de_informacoes = validar_entrada_de_dados();
    if (status_entrada_de_informacoes.eh_codigo_de_erro())
    {
        cout << status_entrada_de_informacoes << endl;
        return -1;
    }
    executar_algoritmo_genetico();
    return 0;
}