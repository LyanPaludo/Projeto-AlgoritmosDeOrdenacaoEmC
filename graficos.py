"""
Gera os gráficos do projeto a partir de resultados/resultados.csv

Uso:
    pip install pandas matplotlib numpy
    python graficos.py                              # usa resultados/resultados.csv
    python graficos.py outro.csv --n-ref 20000      # CSV e tamanho de referência

Saída: PNGs (300 dpi) na pasta graficos/
"""
import argparse
import os

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from matplotlib.colors import TwoSlopeNorm
from matplotlib.lines import Line2D
from matplotlib.patches import Patch
from matplotlib.ticker import FuncFormatter

# ---------------------------------------------------------------- configuração
# Paleta Okabe-Ito (distinguível por daltônicos e boa em projetor).
# Uma cor por família de algoritmo, mantida em TODOS os gráficos.
COR = {
    "bubble": "#D55E00",     # vermelho-alaranjado
    "insertion": "#0072B2",  # azul
    "quick": "#009E73",      # verde
    "merge": "#CC79A7",      # rosa
}
NOME_FAMILIA = {"bubble": "Bubble", "insertion": "Insertion",
                "quick": "Quick", "merge": "Merge"}

# Qual versão é "básica" e qual é "otimizada" em cada família
PAR = {
    "bubble": ("bubble_basico", "bubble_otimizado"),
    "insertion": ("insertion_basico", "insertion_binario"),
    "quick": ("quick_basico", "quick_otimizado"),
    "merge": ("merge_basico", "merge_otimizado"),
}

NOME_TIPO = {
    "aleatoria": "Aleatória",
    "ordenada": "Ordenada",
    "inversa": "Inversa",
    "quase_ordenada": "Quase ordenada",
    "repetidos": "Muitos repetidos",
}

plt.rcParams.update({
    "font.size": 12,
    "axes.titlesize": 14,
    "axes.titleweight": "bold",
    "axes.labelsize": 12,
    "axes.spines.top": False,
    "axes.spines.right": False,
    "axes.grid": True,
    "grid.alpha": 0.25,
    "figure.facecolor": "white",
    "savefig.facecolor": "white",
    "savefig.bbox": "tight",
})

TEMPO_MIN = 1e-3  # evita zero na escala log (resolução do relógio)


def familia(alg):
    return alg.split("_")[0]


def eh_otimizado(alg):
    return alg == PAR[familia(alg)][1]


def fmt_n(x, _):
    return f"{int(x/1000)}k"


def salvar(fig, pasta, nome):
    caminho = os.path.join(pasta, nome)
    fig.savefig(caminho, dpi=300)
    plt.close(fig)
    print("salvo:", caminho)


# ------------------------------------------------------------------- gráfico 1
def grafico_tempo_por_tamanho(df, pasta):
    """Linhas: tempo x tamanho, um painel por tipo de lista (log-log)."""
    tipos = [t for t in NOME_TIPO if t in df.tipo_lista.unique()]
    fig, axes = plt.subplots(2, 3, figsize=(15, 8.5), sharey=True)
    axes = axes.ravel()

    for ax, tipo in zip(axes, tipos):
        sub = df[df.tipo_lista == tipo]
        for alg, g in sub.groupby("algoritmo"):
            g = g.sort_values("tamanho")
            otim = eh_otimizado(alg)
            ax.plot(g.tamanho, g.tempo_ms.clip(lower=TEMPO_MIN),
                    color=COR[familia(alg)],
                    linestyle="-" if otim else "--",
                    marker="s" if otim else "o",
                    markerfacecolor=COR[familia(alg)] if otim else "white",
                    linewidth=2, markersize=6)
        ax.set_xscale("log")
        ax.set_yscale("log")
        ax.set_title(NOME_TIPO[tipo])
        ax.set_xticks(sorted(sub.tamanho.unique()))
        ax.xaxis.set_major_formatter(FuncFormatter(fmt_n))
        ax.minorticks_off()
        ax.tick_params(axis="x", rotation=45)
        ax.set_xlabel("Tamanho da lista (n)")
    for ax in axes[::3]:
        ax.set_ylabel("Tempo médio (ms, escala log)")

    # painel livre vira legenda
    leg = axes[len(tipos)]
    leg.axis("off")
    handles = [Patch(color=COR[f], label=NOME_FAMILIA[f]) for f in COR]
    handles += [
        Line2D([0], [0], color="black", ls="--", marker="o",
               markerfacecolor="white", label="versão básica"),
        Line2D([0], [0], color="black", ls="-", marker="s", label="versão otimizada"),
    ]
    leg.legend(handles=handles, loc="center", frameon=False, fontsize=13)
    for ax in axes[len(tipos) + 1:]:
        ax.axis("off")

    fig.suptitle("Tempo de ordenação por tamanho e tipo de lista",
                 fontsize=17, fontweight="bold", y=1.01)
    fig.tight_layout()
    salvar(fig, pasta, "1_tempo_por_tamanho.png")


# ------------------------------------------------------------------- gráfico 2
def grafico_basico_vs_otimizado(df, pasta, n_ref):
    """Barras agrupadas: básico x otimizado, por família, um painel por tipo."""
    tipos = [t for t in NOME_TIPO if t in df.tipo_lista.unique()]
    sub = df[df.tamanho == n_ref]
    fig, axes = plt.subplots(1, len(tipos), figsize=(3.4 * len(tipos), 5),
                             sharey=True)
    larg = 0.38
    x = np.arange(len(PAR))

    for ax, tipo in zip(np.atleast_1d(axes), tipos):
        s = sub[sub.tipo_lista == tipo].set_index("algoritmo")
        for i, (fam, (basico, otim)) in enumerate(PAR.items()):
            tb = max(s.loc[basico, "tempo_ms"], TEMPO_MIN)
            to = max(s.loc[otim, "tempo_ms"], TEMPO_MIN)
            ax.bar(i - larg / 2, tb, larg, color=COR[fam], alpha=0.40,
                   hatch="//", edgecolor=COR[fam])
            ax.bar(i + larg / 2, to, larg, color=COR[fam])
        ax.set_yscale("log")
        ax.set_xticks(x)
        ax.set_xticklabels([NOME_FAMILIA[f] for f in PAR], rotation=30)
        ax.set_title(NOME_TIPO[tipo], fontsize=12)
        ax.grid(axis="x", visible=False)
    np.atleast_1d(axes)[0].set_ylabel("Tempo médio (ms, escala log)")

    handles = [
        Patch(facecolor="lightgray", hatch="//", edgecolor="gray", label="básica"),
        Patch(facecolor="gray", label="otimizada"),
    ]
    fig.legend(handles=handles, loc="upper right", frameon=False,
               bbox_to_anchor=(0.99, 1.0))
    fig.suptitle(f"Versão básica x otimizada (n = {n_ref:,})".replace(",", "."),
                 fontsize=16, fontweight="bold", y=1.02, x=0.45)
    fig.tight_layout()
    salvar(fig, pasta, "2_basico_vs_otimizado.png")


# ------------------------------------------------------------------- gráfico 3
def grafico_speedup(df, pasta, n_ref):
    """Mapa de calor do ganho (tempo básico / tempo otimizado)."""
    tipos = [t for t in NOME_TIPO if t in df.tipo_lista.unique()]
    sub = df[df.tamanho == n_ref]
    ganho = np.zeros((len(PAR), len(tipos)))
    for i, (fam, (basico, otim)) in enumerate(PAR.items()):
        for j, tipo in enumerate(tipos):
            s = sub[sub.tipo_lista == tipo].set_index("algoritmo").tempo_ms
            ganho[i, j] = max(s[basico], TEMPO_MIN) / max(s[otim], TEMPO_MIN)

    # cor na escala log (os ganhos vão de ~0,2x a milhares de x); texto mostra o valor real
    lg = np.log10(ganho)
    vmin, vmax = min(lg.min(), -1), max(lg.max(), 1)
    fig, ax = plt.subplots(figsize=(10.5, 4.8))
    im = ax.imshow(lg, cmap="RdYlGn", norm=TwoSlopeNorm(vcenter=0, vmin=vmin, vmax=vmax),
                   aspect="auto")
    ax.set_xticks(range(len(tipos)))
    ax.set_xticklabels([NOME_TIPO[t].replace(" ", "\n") for t in tipos])
    ax.set_yticks(range(len(PAR)))
    ax.set_yticklabels([NOME_FAMILIA[f] for f in PAR])
    ax.grid(False)
    for i in range(ganho.shape[0]):
        for j in range(ganho.shape[1]):
            v = ganho[i, j]
            ax.text(j, i, f"{v:.1f}x" if v < 100 else f"{v:.0f}x",
                    ha="center", va="center", fontsize=13, fontweight="bold")
    cb = fig.colorbar(im, ax=ax)
    ticks = [t for t in range(-3, 5) if vmin <= t <= vmax]
    cb.set_ticks(ticks)
    cb.set_ticklabels([f"{10.0 ** t:g}x" for t in ticks])
    cb.set_label("Ganho (escala log)")
    ax.set_title(f"Quantas vezes a versão otimizada é mais rápida (n = {n_ref:,})"
                 .replace(",", "."))
    fig.tight_layout()
    salvar(fig, pasta, "3_ganho_otimizacao.png")


# ------------------------------------------------------------------- gráfico 4
def grafico_mapa_calor(df, pasta, n_ref):
    """Mapa de calor com o tempo de todos os algoritmos x tipos de lista."""
    from matplotlib.colors import LogNorm
    tipos = [t for t in NOME_TIPO if t in df.tipo_lista.unique()]
    algs = [a for par in PAR.values() for a in par]
    sub = df[df.tamanho == n_ref]
    m = np.full((len(algs), len(tipos)), np.nan)
    for i, a in enumerate(algs):
        for j, t in enumerate(tipos):
            v = sub[(sub.algoritmo == a) & (sub.tipo_lista == t)].tempo_ms
            if len(v):
                m[i, j] = max(v.iloc[0], TEMPO_MIN)

    fig, ax = plt.subplots(figsize=(10, 6))
    im = ax.imshow(m, cmap="YlOrRd", norm=LogNorm(), aspect="auto")
    ax.set_xticks(range(len(tipos)))
    ax.set_xticklabels([NOME_TIPO[t] for t in tipos])
    ax.set_yticks(range(len(algs)))
    ax.set_yticklabels(algs)
    ax.grid(False)
    for i in range(m.shape[0]):
        for j in range(m.shape[1]):
            if not np.isnan(m[i, j]):
                cor = "white" if m[i, j] > np.nanmax(m) ** 0.6 else "black"
                ax.text(j, i, f"{m[i, j]:.2f}", ha="center", va="center",
                        fontsize=10.5, color=cor)
    fig.colorbar(im, ax=ax).set_label("Tempo (ms, escala log)")
    ax.set_title(f"Tempo de cada algoritmo por tipo de lista (n = {n_ref:,})"
                 .replace(",", "."))
    fig.tight_layout()
    salvar(fig, pasta, "4_mapa_de_calor.png")


# ------------------------------------------------------------------- gráfico 5
def grafico_comparacoes(df, pasta, n_ref):
    """Barras de comparações (contagem, não tempo) para lista aleatória."""
    sub = df[(df.tamanho == n_ref) & (df.tipo_lista == "aleatoria")]
    algs = [a for par in PAR.values() for a in par]
    fig, ax = plt.subplots(figsize=(10, 5))
    vals = [sub[sub.algoritmo == a].comparacoes.iloc[0] for a in algs]
    cores = [COR[familia(a)] for a in algs]
    barras = ax.bar(range(len(algs)), vals, color=cores)
    for b, a in zip(barras, algs):
        if not eh_otimizado(a):
            b.set_alpha(0.45)
            b.set_hatch("//")
            b.set_edgecolor(COR[familia(a)])
    ax.set_yscale("log")
    ax.set_xticks(range(len(algs)))
    ax.set_xticklabels(algs, rotation=35, ha="right")
    ax.set_ylabel("Nº de comparações (escala log)")
    ax.grid(axis="x", visible=False)
    ax.set_title(f"Comparações em lista aleatória (n = {n_ref:,})"
                 .replace(",", "."))
    fig.tight_layout()
    salvar(fig, pasta, "5_comparacoes.png")


# ------------------------------------------------------------------------ main
def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("csv", nargs="?", default="resultados/resultados.csv")
    ap.add_argument("--n-ref", type=int, default=20000,
                    help="tamanho usado nos gráficos de barras/mapas (todos os "
                         "algoritmos precisam ter rodado nele)")
    ap.add_argument("--saida", default="graficos")
    args = ap.parse_args()

    df = pd.read_csv(args.csv)
    os.makedirs(args.saida, exist_ok=True)

    grafico_tempo_por_tamanho(df, args.saida)
    grafico_basico_vs_otimizado(df, args.saida, args.n_ref)
    grafico_speedup(df, args.saida, args.n_ref)
    grafico_mapa_calor(df, args.saida, args.n_ref)
    grafico_comparacoes(df, args.saida, args.n_ref)


if __name__ == "__main__":
    main()
