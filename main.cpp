#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

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
class Horario;
class Aleatorio;

unordered_map<string, int> indice_cidade;
vector<string> nome_cidade;
vector<string> sigla_cidade;
vector<Voo> voos;
vector<vector<int>> voos_de_ida;
vector<vector<int>> voos_de_volta;
int qnt_cidades;

class StatusLeituraDeDados
{
public:
    int codigo;
    string mensagem;
    StatusLeituraDeDados(int _codigo, string _mensagem) : codigo(_codigo), mensagem(_mensagem) {}
    friend ostream &operator<<(ostream &s, const StatusLeituraDeDados &status_leitura_de_dados)
    {
        s << status_leitura_de_dados.mensagem;
        return s;
    }
    bool eh_codigo_de_sucesso()
    {
        return codigo == 200;
    }
    bool eh_codigo_de_erro()
    {
        return codigo != 200;
    }
};

class Aleatorio
{
public:
    static std::random_device rd;
    static std::mt19937 gen;
    static int gerar_aleatorio_em_intervalo(int inicio, int fim)
    {
        std::uniform_int_distribution<> distrib(inicio, fim);
        return distrib(gen);
    }
    static int gera_elemento_aleatorio(int i)
    {
        return gerar_aleatorio_em_intervalo(0, (int)voos_de_ida[i].size() - 1);
    }
    static int gerar_probabilidade()
    {
        std::uniform_int_distribution<> distrib(0, 100);
        return distrib(gen);
    }
};

std::random_device Aleatorio::rd;
std::mt19937 Aleatorio::gen(Aleatorio::rd());

class Horario
{
public:
    int hora;
    int minuto;

    Horario() {}

    Horario(const string &palavra)
    {
        stringstream k(palavra);
        char lixo;
        k >> this->hora;
        k >> lixo;
        k >> this->minuto;
    }

    static bool verificar_se_eh_horario_invalido(const string &palavra)
    {
        stringstream k(palavra);
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

    friend ostream &operator<<(ostream &s, const Horario &horario)
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

    friend bool operator>(const Horario &horario1, const Horario &horario2)
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
    friend bool operator<(const Horario &horario1, const Horario &horario2)
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
    friend int operator-(const Horario &horario1, const Horario &horario2)
    {
        int tempo1 = horario1.hora * 60 + horario1.minuto;
        int tempo2 = horario2.hora * 60 + horario2.minuto;
        return tempo1 - tempo2;
    }
};

class Voo
{
public:
    string sigla_origem;
    string sigla_destino;
    Horario horario_inicio;
    Horario horario_fim;
    int custo;

    Voo(string &_sigla_origem, string &_sigla_destino, string &_horario_inicio, string &_horario_fim, int &_custo) : sigla_origem(_sigla_origem),
                                                                                                                     sigla_destino(_sigla_destino),
                                                                                                                     horario_inicio(Horario(_horario_inicio)),
                                                                                                                     horario_fim(Horario(_horario_fim)),
                                                                                                                     custo(_custo)
    {
    }

    bool siglas_nao_sao_validas()
    {
        return indice_cidade.find(this->sigla_origem) == indice_cidade.end() || indice_cidade.find(this->sigla_destino) == indice_cidade.end();
    }

    bool horario_nao_eh_valido()
    {
        return this->horario_inicio > this->horario_fim;
    }

    friend ostream &operator<<(ostream &s, const Voo &voo)
    {
        s << voo.custo << " " << voo.horario_inicio << " -> " << voo.horario_fim;
        return s;
    }
};

class Individuo
{
private:
    vector<int> elementos;

public:
    Individuo()
    {
        elementos.resize(qnt_cidades - 1);
    }

    Individuo(const vector<int> &dados)
    {
        elementos.resize(qnt_cidades - 1);
        for (int i = 0; i < qnt_cidades - 1; ++i)
        {
            this->elementos[i] = dados[i];
        }
    }

    void set_elemento(int i, int valor)
    {
        elementos[i] = valor;
    }

    friend ostream &operator<<(ostream &s, const Individuo &ind)
    {
        string esp = "";
        for (int i = 0; i < qnt_cidades - 1; ++i)
        {
            s << esp << ind.elementos[i];
            esp = " ";
        }
        return s;
    }

    void imprimir_voos() const
    {
        for (int i = 0; i < qnt_cidades - 1; ++i)
        {
            cout << voos[voos_de_ida[i][this->elementos[i]]] << endl;
        }
    }

    friend Individuo crossover(Individuo &ind1, Individuo &ind2)
    {
        Individuo novo;
        int local_corte = Aleatorio::gerar_aleatorio_em_intervalo(1, qnt_cidades - 2);
        for (int i = 0; i < local_corte; ++i)
        {
            novo.set_elemento(i, ind1.elementos[i]);
        }
        for (int i = local_corte; i < qnt_cidades - 1; ++i)
        {
            novo.set_elemento(i, ind2.elementos[i]);
        }
        return novo;
    }

    friend Individuo mutation(Individuo &ind1)
    {
        Individuo novo;
        for (int i = 0; i < qnt_cidades - 1; ++i)
        {
            novo.set_elemento(i, ind1.elementos[i]);
        }
        for(int i = 0; i < QNT_IDX_MUTADOS; ++i){
            int local_mutacao = Aleatorio::gerar_aleatorio_em_intervalo(0, qnt_cidades - 2);
            novo.set_elemento(local_mutacao, Aleatorio::gera_elemento_aleatorio(local_mutacao));
        }
        return novo;
    }

    friend double fitness(const Individuo &ind)
    {
        int custo_em_dinheiro = calcula_custo_em_dinheiro(ind);
        int custo_em_tempo = calcula_custo_em_tempo(ind);
        return PESO_CUSTO * custo_em_dinheiro + PESO_TEMPO * custo_em_tempo;
    }

    void operator=(const Individuo &ind)
    {
        for (int i = 0; i < qnt_cidades - 1; ++i)
        {
            this->set_elemento(i, ind.elementos[i]);
        }
    }

    friend int calcula_custo_em_dinheiro(const Individuo &ind)
    {
        int custo_em_dinheiro = 0;
        for (int i = 0; i < qnt_cidades - 1; ++i)
        {
            custo_em_dinheiro += voos[voos_de_ida[i][ind.elementos[i]]].custo;
        }
        return custo_em_dinheiro;
    }

    friend int calcula_custo_em_tempo(const Individuo &ind)
    {
        int indice_do_tempo_maximo = 0;
        int indice_do_tempo_minimo = 0;
        for (int i = 1; i < qnt_cidades - 1; ++i)
        {
            if (voos[voos_de_ida[i][ind.elementos[i]]].horario_fim > voos[voos_de_ida[indice_do_tempo_maximo][ind.elementos[indice_do_tempo_maximo]]].horario_fim)
            {
                indice_do_tempo_maximo = i;
            }
            if (voos[voos_de_ida[i][ind.elementos[i]]].horario_fim < voos[voos_de_ida[indice_do_tempo_minimo][ind.elementos[indice_do_tempo_minimo]]].horario_fim)
            {
                indice_do_tempo_minimo = i;
            }
        }
        return voos[voos_de_ida[indice_do_tempo_maximo][ind.elementos[indice_do_tempo_maximo]]].horario_fim -
               voos[voos_de_ida[indice_do_tempo_minimo][ind.elementos[indice_do_tempo_minimo]]].horario_fim;
    }

    static Individuo gerar_individuo_aleatorio()
    {
        Individuo ind;
        for (int i = 0; i < qnt_cidades - 1; ++i)
        {
            ind.set_elemento(i, Aleatorio::gera_elemento_aleatorio(i));
        }
        return ind;
    }

    friend bool operator<(const Individuo &ind1, const Individuo &ind2)
    {
        return fitness(ind1) < fitness(ind2);
    }

    friend bool operator>(const Individuo &ind1, const Individuo &ind2)
    {
        return fitness(ind1) > fitness(ind2);
    }
};

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

/* Algoritmo Genetico */

class SelecaoGenetica
{
private:
    vector<Individuo> individuos;

public:
    SelecaoGenetica()
    {
        individuos.resize(TAM_POPULACAO);
        for (int i = 0; i < TAM_POPULACAO; ++i)
        {
            Individuo novo = Individuo::gerar_individuo_aleatorio();
            individuos[i] = novo;
        }
    }
    Individuo selecionar_individuo_por_torneio()
    {
        vector<Individuo> membros(MEMBROS_TORNEIO);
        for (int i = 0; i < MEMBROS_TORNEIO; ++i)
        {
            Individuo novo = individuos[Aleatorio::gerar_aleatorio_em_intervalo(0, TAM_POPULACAO - 1)];
            membros[i] = novo;
        }
        int probabilidade_selecao = Aleatorio::gerar_probabilidade();
        if (probabilidade_selecao < PARAM_TORNEIO)
        {
            Individuo melhor = membros[0];
            for (int i = 1; i < MEMBROS_TORNEIO; ++i)
            {
                if (fitness(membros[i]) < fitness(melhor))
                {
                    melhor = membros[i];
                }
            }
            return melhor;
        }
        Individuo pior = membros[0];
            for (int i = 1; i < MEMBROS_TORNEIO; ++i)
            {
                if (fitness(membros[i]) < fitness(pior))
                {
                    pior = membros[i];
                }
            }
            return pior;
    }

    Individuo selecionar_melhor_individuo()
    {
        Individuo melhor = individuos[0];
        for (int i = 1; i < TAM_POPULACAO; ++i)
        {
            if (fitness(individuos[i]) < fitness(melhor))
            {
                melhor = individuos[i];
            }
        }
        return melhor;
    }

    int selecionar_indice_pior_individuo()
    {
        int indice_pior = 0;
        for (int i = 1; i < TAM_POPULACAO; ++i)
        {
            if (fitness(individuos[i]) < fitness(individuos[indice_pior]))
            {
                indice_pior = i;
            }
        }
        return indice_pior;
    }

    vector<Individuo> gerar_nova_populacao(){
        Individuo melhor = selecionar_melhor_individuo();
        vector<Individuo> nova_populacao(TAM_POPULACAO);
        nova_populacao[0] = melhor;
        for(int i = 1; i < TAM_POPULACAO; ++i){
            Individuo selecionado1 = selecionar_individuo_por_torneio();
            Individuo selecionado2 = selecionar_individuo_por_torneio();
            int prob_cruzamento = Aleatorio::gerar_probabilidade();
            if(prob_cruzamento < PROB_MIN_CRUZAMENTO){
                selecionado1 = crossover(selecionado1, selecionado2);
            }
            int prob_mutacao = Aleatorio::gerar_probabilidade();
            if(prob_mutacao < PROB_MIN_MUTACAO){
                selecionado1 = mutation(selecionado1);
            }
            nova_populacao[i] = selecionado1;
        }
        return nova_populacao;
    }

    void imprimir_individuos()
    {
        for (auto &i : individuos)
        {
            cout << i << endl;
        }
    }

    void executar_algoritmo()
    {
        for (int i = 0; i < NUM_GERACOES; ++i)
        {
            individuos = gerar_nova_populacao();
            cout << selecionar_melhor_individuo() << " -> " << fitness(selecionar_melhor_individuo() )<< endl;
        }
    }

    void executar_algoritmo_ate_encontrar_otimo()
    {
        int i = 0;
        double otimo = gerar_melhor_fitness_brute_force();
        double resultado = selecionar_melhor_fitness();
        while(resultado != otimo){
            individuos = gerar_nova_populacao();
            resultado = selecionar_melhor_fitness();
            ++i;
        }
        cout << "Demorou " << i << " iteracoes" << endl;
        cout << selecionar_melhor_individuo() << endl;
    }

    double selecionar_melhor_fitness()
    {
        Individuo melhor = individuos[0];
        for (int i = 1; i < TAM_POPULACAO; ++i)
        {
            if (fitness(individuos[i]) < fitness(melhor))
            {
                melhor = individuos[i];
            }
        }
        return fitness(melhor);
    }

    double selecionar_pior_fitness()
    {
        Individuo pior = individuos[0];
        for (int i = 1; i < TAM_POPULACAO; ++i)
        {
            if (fitness(individuos[i]) > fitness(pior))
            {
                pior = individuos[i];
            }
        }
        return fitness(pior);
    }

    double selecionar_fitness_medio()
    {
        double total = 0;
        for (int i = 0; i < TAM_POPULACAO; ++i)
        {
            total += fitness(individuos[i]);
        }
        return total / TAM_POPULACAO;
    }

    double selecionar_variancia_populacional_fitness()
    {
        double numerador = 0;
        double media_populacao = selecionar_fitness_medio();
        for (int i = 0; i < TAM_POPULACAO; ++i)
        {
            double atual = (fitness(individuos[i]) - media_populacao);
            numerador += atual * atual;
        }
        return sqrt(numerador / TAM_POPULACAO);
    }

};

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

void teste_selecao()
{
    SelecaoGenetica selecaco;
    // selecaco.executar_algoritmo();
    selecaco.executar_algoritmo_ate_encontrar_otimo();
}

void teste()
{
    // gerar_e_cruzar_dois_individuos_aleatorios();
    teste_selecao();
    // gerar_melhor_individuo_brute_force();
}

/* Main */

int main()
{
    StatusLeituraDeDados status_entrada_de_informacoes = validar_entrada_de_dados();
    cout << status_entrada_de_informacoes << endl;
    if (status_entrada_de_informacoes.eh_codigo_de_erro())
    {
        return -1;
    }
    teste();
    return 0;
}