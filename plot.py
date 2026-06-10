"""Analisa a convergencia do algoritmo genetico a partir do arquivo saida.txt."""

from __future__ import annotations

import argparse
import csv
from pathlib import Path
import re

import matplotlib.pyplot as plt
import numpy as np


COLUNAS = (
    "melhor_fitness",
    "pior_fitness",
    "fitness_medio",
    "variancia",
    "desvio_padrao",
)

PADRAO_SECAO = re.compile(r"^\s*SELECAO\s+DOS\s+VOOS\s+DE\s+(.+?)\s*$", re.IGNORECASE)


def ler_argumentos() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Gera graficos e um relatorio estatistico do algoritmo genetico."
    )
    parser.add_argument(
        "--entrada",
        type=Path,
        default=Path("saida.txt"),
        help="Arquivo com as cinco estatisticas por geracao (padrao: saida.txt).",
    )
    parser.add_argument(
        "--grafico",
        type=Path,
        default=Path("analise_convergencia.png"),
        help="Imagem de saida (padrao: analise_convergencia.png).",
    )
    parser.add_argument(
        "--relatorio",
        type=Path,
        default=Path("relatorio_fitness.csv"),
        help="Relatorio CSV de saida (padrao: relatorio_fitness.csv).",
    )
    parser.add_argument(
        "--mostrar",
        action="store_true",
        help="Tambem abre o grafico em uma janela.",
    )
    return parser.parse_args()


def carregar_dados(caminho: Path) -> dict[str, np.ndarray]:
    if not caminho.is_file():
        raise FileNotFoundError(f"Arquivo de entrada nao encontrado: {caminho}")

    secoes: dict[str, list[list[float]]] = {}
    secao_atual = "GERAL"
    linhas_ignoradas = 0

    with caminho.open(encoding="utf-8-sig", errors="replace") as arquivo:
        for numero_linha, linha in enumerate(arquivo, start=1):
            texto = linha.strip()
            if not texto:
                continue

            identificador_secao = PADRAO_SECAO.match(texto)
            if identificador_secao:
                secao_atual = identificador_secao.group(1).strip().upper()
                secoes.setdefault(secao_atual, [])
                continue

            partes = texto.replace(",", ".").split()
            if len(partes) != len(COLUNAS):
                linhas_ignoradas += 1
                continue

            try:
                valores = [float(parte) for parte in partes]
            except ValueError:
                linhas_ignoradas += 1
                continue

            if not np.isfinite(valores).all():
                raise ValueError(f"Valores invalidos na linha {numero_linha}: {texto}")
            if valores[3] < 0 or valores[4] < 0:
                raise ValueError(
                    f"Variancia ou desvio padrao negativo na linha {numero_linha}."
                )

            secoes.setdefault(secao_atual, []).append(valores)

    dados = {
        nome: np.asarray(valores, dtype=float)
        for nome, valores in secoes.items()
        if valores
    }
    if not dados:
        raise ValueError(
            f"Nenhuma linha com as {len(COLUNAS)} colunas numericas foi encontrada "
            f"em {caminho}."
        )

    print(
        f"Foram lidas {sum(len(secao) for secao in dados.values())} geracoes "
        f"em {len(dados)} secao(oes)."
    )
    if linhas_ignoradas:
        print(f"Linhas descritivas ignoradas: {linhas_ignoradas}")

    return dados


def salvar_relatorio(caminho: Path, secoes: dict[str, np.ndarray]) -> None:
    caminho.parent.mkdir(parents=True, exist_ok=True)

    with caminho.open("w", newline="", encoding="utf-8") as arquivo:
        escritor = csv.writer(arquivo)
        escritor.writerow(("secao", "geracao", *COLUNAS))

        for nome_secao, dados in secoes.items():
            for geracao, valores in enumerate(dados, start=1):
                escritor.writerow(
                    (nome_secao, geracao, *(f"{valor:.3f}" for valor in valores))
                )


def gerar_graficos(
    caminho: Path, secoes: dict[str, np.ndarray], mostrar: bool
) -> None:
    quantidade_secoes = len(secoes)
    figura, eixos = plt.subplots(
        3,
        quantidade_secoes,
        figsize=(7 * quantidade_secoes, 11),
        sharex="col",
        squeeze=False,
        gridspec_kw={"height_ratios": (2, 1, 1)},
    )

    for coluna, (nome_secao, dados) in enumerate(secoes.items()):
        geracoes = np.arange(1, len(dados) + 1)
        melhor, pior, media, variancia, desvio_padrao = dados.T
        eixo_fitness = eixos[0, coluna]

        eixo_fitness.plot(
            geracoes, melhor, label="Melhor fitness", color="#18864b", lw=2
        )
        eixo_fitness.plot(
            geracoes, media, label="Fitness medio", color="#2468b4", lw=2
        )
        eixo_fitness.plot(
            geracoes, pior, label="Pior fitness", color="#c43c35", lw=1.6
        )
        eixo_fitness.fill_between(
            geracoes,
            melhor,
            pior,
            color="#7d8da6",
            alpha=0.12,
            label="Amplitude da populacao",
        )
        eixo_fitness.set_title(f"Convergencia - voos de {nome_secao.lower()}")
        eixo_fitness.set_ylabel("Fitness")
        eixo_fitness.legend(loc="best", fontsize=9)

        eixos[1, coluna].plot(
            geracoes,
            desvio_padrao,
            color="#7b4ab5",
            lw=1.8,
            label="Desvio padrao",
        )
        eixos[1, coluna].set_title(f"Dispersao - {nome_secao.lower()}")
        eixos[1, coluna].set_ylabel("Desvio padrao")
        eixos[1, coluna].legend(loc="best")

        eixos[2, coluna].plot(
            geracoes, variancia, color="#d17a22", lw=1.8, label="Variancia"
        )
        eixos[2, coluna].set_title(f"Variancia - {nome_secao.lower()}")
        eixos[2, coluna].set_xlabel("Geracao")
        eixos[2, coluna].set_ylabel("Variancia")
        eixos[2, coluna].legend(loc="best")

        for linha in range(3):
            eixos[linha, coluna].grid(True, linestyle=":", alpha=0.55)
            eixos[linha, coluna].margins(x=0)

    figura.tight_layout()
    caminho.parent.mkdir(parents=True, exist_ok=True)
    figura.savefig(caminho, dpi=180, bbox_inches="tight")

    if mostrar:
        plt.show()
    else:
        plt.close(figura)


def imprimir_resumo(secoes: dict[str, np.ndarray]) -> None:
    for nome_secao, dados in secoes.items():
        melhor, pior, media, variancia, desvio_padrao = dados.T
        indice_melhor = int(np.argmin(melhor))
        ultima = dados[-1]

        print(f"\nRESUMO - VOOS DE {nome_secao}")
        print(f"Geracoes analisadas:   {len(dados)}")
        print(
            f"Melhor fitness global: {melhor[indice_melhor]:.3f} "
            f"(geracao {indice_melhor + 1})"
        )
        print(f"Fitness medio inicial: {media[0]:.3f}")
        print(f"Fitness medio final:   {ultima[2]:.3f}")
        print(f"Pior fitness final:    {ultima[1]:.3f}")
        print(f"Variancia final:       {ultima[3]:.3f}")
        print(f"Desvio padrao final:   {ultima[4]:.3f}")
        print("Medias entre todas as geracoes:")
        print(f"  Fitness medio:       {np.mean(media):.3f}")
        print(f"  Variancia:           {np.mean(variancia):.3f}")
        print(f"  Desvio padrao:       {np.mean(desvio_padrao):.3f}")


def main() -> None:
    argumentos = ler_argumentos()
    secoes = carregar_dados(argumentos.entrada)

    salvar_relatorio(argumentos.relatorio, secoes)
    gerar_graficos(argumentos.grafico, secoes, argumentos.mostrar)
    imprimir_resumo(secoes)

    print(f"\nGrafico salvo em:  {argumentos.grafico.resolve()}")
    print(f"Relatorio salvo em: {argumentos.relatorio.resolve()}")


if __name__ == "__main__":
    main()
