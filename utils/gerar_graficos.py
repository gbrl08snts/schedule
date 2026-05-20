import pandas as pd
import matplotlib.pyplot as plt

# LEITURA DO CSV

df = pd.read_csv("/resultados_tme.csv")

# CÁLCULO DAS ESTATÍSTICAS

estatisticas = df.groupby(
    ["Algoritmo", "Quantum", "N_Processos"]
)["TME"].agg(["mean", "std"]).reset_index()

# LISTAS AUXILIARES

algoritmos = estatisticas["Algoritmo"].unique()
quantums = sorted(estatisticas["Quantum"].unique())


# GRÁFICOS INDIVIDUAIS
# Esses não foram apresentados no relatório. Quando estávamos escrevendo reparamos 
# a quantidade absuda de imagens que teriam então optmos por deixar apenas os gráficos comparativos.

for algoritmo in algoritmos:

    plt.figure(figsize=(10, 6))

    for q in quantums:

        dados = estatisticas[
            (estatisticas["Algoritmo"] == algoritmo) &
            (estatisticas["Quantum"] == q)
        ]

        plt.errorbar(
            dados["N_Processos"],
            dados["mean"],
            yerr=dados["std"],
            marker='o',
            capsize=5,
            linewidth=2,
            label=f'Quantum {q}'
        )

    plt.title(f"TME Médio - {algoritmo.upper()}")
    plt.xlabel("Quantidade de Processos")
    plt.ylabel("Média do TME (µs)")
    plt.grid(True)
    plt.legend()

    plt.tight_layout()

    # salva imagem
    plt.savefig(f"grafico_{algoritmo}.png")

    plt.show()

# GRÁFICOS COMPARATIVOS

for q in quantums:

    plt.figure(figsize=(12, 7))

    for algoritmo in algoritmos:

        dados = estatisticas[
            (estatisticas["Algoritmo"] == algoritmo) &
            (estatisticas["Quantum"] == q)
        ]

        plt.errorbar(
            dados["N_Processos"],
            dados["mean"],
            yerr=dados["std"],
            marker='o',
            capsize=5,
            linewidth=2,
            label=algoritmo
        )

    plt.title(f"Comparação entre Algoritmos (Quantum = {q})")
    plt.xlabel("Quantidade de Processos")
    plt.ylabel("Média do TME (µs)")
    plt.grid(True)
    plt.legend()

    plt.tight_layout()

    # salva imagem
    plt.savefig(f"grafico_comparativo_q{q}.png")

    plt.show()