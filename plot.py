import numpy as np
import matplotlib.pyplot as plt

fitness_melhor, fitness_pior, media, variancia, desvio_padrao = np.loadtxt('saida.txt', unpack=True)

plt.figure(figsize=(10, 6))

geracoes = list(range(1, len(fitness_melhor) + 1))

# 2. Plota as três linhas com cores e estilos diferentes
plt.plot(geracoes, fitness_melhor, label="Melhor Fitness", color="green", linewidth=2)
plt.plot(geracoes, media, label="Fitness Médio", color="blue", linewidth=2)
plt.plot(geracoes, fitness_pior, label="Pior Fitness", color="red", linewidth=1.5)


# 3. Adiciona títulos e rótulos nos eixos
plt.title("Evolução do Fitness ao Longo das Gerações", fontsize=14, fontweight='bold')
plt.xlabel("Gerações", fontsize=12)
plt.ylabel("Valor de Fitness", fontsize=12)

# 4. Ativa a grade de fundo para facilitar a leitura dos valores
plt.grid(True, linestyle=":", alpha=0.6)

# 5. Adiciona a legenda para identificar cada linha
plt.legend(fontsize=11, loc="upper left")

# 6. Exibe o gráfico na tela
plt.show()

plt.plot(geracoes, variancia, label="Variancia", color="green", linewidth=1.5)

plt.title("Variância ao Longo das Gerações", fontsize=14, fontweight='bold')
plt.xlabel("Gerações", fontsize=12)
plt.ylabel("Variância", fontsize=12)

plt.grid(True, linestyle=":", alpha=0.6)

plt.legend(fontsize=11, loc="upper left")

plt.show()

plt.plot(geracoes, desvio_padrao, label="Desvio Padrão", color="blue", linewidth=1.5)

plt.title("Desvio Padrão ao Longo das Gerações", fontsize=14, fontweight='bold')
plt.xlabel("Gerações", fontsize=12)
plt.ylabel("Desvio Padrão", fontsize=12)

plt.grid(True, linestyle=":", alpha=0.6)

plt.legend(fontsize=11, loc="upper left")

plt.show()