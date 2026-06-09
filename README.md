# Algoritmo Genético — Combinações de Voos (ECOI22)

Implementação de algoritmo genético para identificar as melhores combinações de voos, avaliando custo em dinheiro e tempo máximo de espera.

## Compilação

```bash
g++ main.cpp -o main
```

## Execução

```bash
./main
```

Para salvar a saída em arquivo:

```bash
./main > saida.txt
```

## Configuração

**Pesos de custo e tempo**
Edite as macros em `cabecalho/state.h` para ajustar os pesos associados ao custo em dinheiro e em tempo de espera.

**Adicionar nova cidade**
1. Adicione a sigla e o nome ao arquivo `entrada/cidades.txt`
2. Insira pelo menos um voo de ida e um de volta entre a nova cidade e Roma

## Branches

| Branch | Descrição |
|---|---|
| `main` | Considera apenas voos de ida |
| `voos-de-volta` | Considera voos de volta |

## Restrições

- Todos os voos têm Roma como origem ou destino.
- Voos com horário de saída anterior ao horário de chegada não são considerados válidos.
