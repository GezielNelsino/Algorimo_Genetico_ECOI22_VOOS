#include "../cabecalho/SelecaoGenetica.h"

SelecaoGenetica::SelecaoGenetica()
{
    individuos.resize(TAM_POPULACAO);
    for (int i = 0; i < TAM_POPULACAO; ++i)
    {
        Individuo novo = Individuo::gerar_individuo_aleatorio();
        individuos[i] = novo;
    }
}
Individuo SelecaoGenetica::selecionar_individuo_por_torneio()
{
    std::vector<Individuo> membros(MEMBROS_TORNEIO);
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

Individuo SelecaoGenetica::selecionar_melhor_individuo()
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

int SelecaoGenetica::selecionar_indice_pior_individuo()
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

std::vector<Individuo> SelecaoGenetica::gerar_nova_populacao()
{
    Individuo melhor = selecionar_melhor_individuo();
    std::vector<Individuo> nova_populacao(TAM_POPULACAO);
    nova_populacao[0] = melhor;
    for (int i = 1; i < TAM_POPULACAO; ++i)
    {
        Individuo selecionado1 = selecionar_individuo_por_torneio();
        Individuo selecionado2 = selecionar_individuo_por_torneio();
        int prob_cruzamento = Aleatorio::gerar_probabilidade();
        if (prob_cruzamento < PROB_MIN_CRUZAMENTO)
        {
            selecionado1 = crossover(selecionado1, selecionado2);
        }
        int prob_mutacao = Aleatorio::gerar_probabilidade();
        if (prob_mutacao < PROB_MIN_MUTACAO)
        {
            selecionado1 = mutation(selecionado1);
        }
        nova_populacao[i] = selecionado1;
    }
    return nova_populacao;
}

void SelecaoGenetica::imprimir_individuos()
{
    for (auto &i : individuos)
    {
        std::cout << i << std::endl;
    }
}

void SelecaoGenetica::imprimir_melhor_da_geracao(int indice_geracao)
{
    Individuo melhor_individuo_geracao = selecionar_melhor_individuo();
    auto resultados_da_geracao = selecionar_melhor_pior_media_variancia_fitness_geracao();
    std::cout << "Melhor individuo da geracao " << indice_geracao << "\t\t: " << melhor_individuo_geracao << std::endl;
    std::cout << "Fitness do melhor individuo " << "\t\t: " << std::get<0>(resultados_da_geracao) << std::endl;
    std::cout << "Fitness do pior individuo " << "\t\t\t: " << std::get<1>(resultados_da_geracao) << std::endl;
    std::cout << "Fitness medio dos individuos " << "\t\t: " << std::get<2>(resultados_da_geracao) << std::endl;
    std::cout << "Variancia do fitness dos individuos " << ": " << std::get<3>(resultados_da_geracao) << std::endl;
    std::cout << std::endl;
}

void SelecaoGenetica::imprimir_melhor_da_geracao_em_colunas(int indice_geracao)
{
    auto resultados_da_geracao = selecionar_melhor_pior_media_variancia_fitness_geracao();
    std::cout << std::fixed << std::setprecision(3);
    std::cout << std::get<0>(resultados_da_geracao) << " \t";
    std::cout << std::get<1>(resultados_da_geracao) << " \t";
    std::cout << std::get<2>(resultados_da_geracao) << " \t";
    std::cout << std::get<3>(resultados_da_geracao) << " \t";
    std::cout << std::endl;
}

void SelecaoGenetica::executar()
{
    for (int i = 0; i < NUM_GERACOES; ++i)
    {
        individuos = gerar_nova_populacao();
        imprimir_melhor_da_geracao_em_colunas(i);
    }
}

void SelecaoGenetica::executar_algoritmo_ate_encontrar_otimo(double otimo)
{
    int i = 0;
    double resultado = selecionar_melhor_fitness();
    while (resultado != otimo)
    {
        individuos = gerar_nova_populacao();
        resultado = selecionar_melhor_fitness();
        ++i;
        std::cout << resultado << std::endl;
    }
    std::cout << "Demorou " << i << " iteracoes" << std::endl;
    std::cout << selecionar_melhor_individuo() << std::endl;
}

double SelecaoGenetica::selecionar_melhor_fitness()
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

double SelecaoGenetica::selecionar_pior_fitness()
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

double SelecaoGenetica::selecionar_fitness_medio()
{
    double total = 0;
    for (int i = 0; i < TAM_POPULACAO; ++i)
    {
        total += fitness(individuos[i]);
    }
    return total / TAM_POPULACAO;
}

double SelecaoGenetica::selecionar_variancia_populacional_fitness()
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

std::tuple<double, double, double, double> SelecaoGenetica::selecionar_melhor_pior_media_variancia_fitness_geracao()
{
    int ind_melhor, ind_pior;
    double numerador_media, numerador_variancia;
    ind_melhor = ind_pior = 0;
    numerador_media = fitness(individuos[0]);
    numerador_variancia = 0;
    for (int i = 1; i < TAM_POPULACAO; ++i)
    {
        if (fitness(individuos[i]) < fitness(individuos[ind_melhor]))
        {
            ind_melhor = i;
        }
        if (fitness(individuos[i]) > fitness(individuos[ind_pior]))
        {
            ind_pior = i;
        }
        numerador_media += fitness(individuos[i]);
    }
    double media_populacao = numerador_media / TAM_POPULACAO;
    for (int i = 0; i < TAM_POPULACAO; ++i)
    {
        double atual = (fitness(individuos[i]) - media_populacao);
        numerador_variancia += atual * atual;
    }
    double variancia = sqrt(numerador_variancia / TAM_POPULACAO);
    return std::make_tuple(fitness(individuos[ind_melhor]), fitness(individuos[ind_pior]), media_populacao, variancia > 1e-12 ? variancia : 0);
}