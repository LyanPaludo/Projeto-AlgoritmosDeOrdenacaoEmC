#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "sorts.h"
#include "generators.h"
#include "utils.h"

#define SEMENTE 42
#define REPETICOES 10
#define LIMIAR_MEDICAO_MS 0.05
#define TAMANHO_LOTE 100

typedef struct {
    const char *nome;
    void (*fn)(int *, int);
    int limite_n;            /* 0 = sem limite; evita O(n^2)/pilha muito grandes */
} Algoritmo;

typedef struct {
    const char *nome;
    void (*gera)(int *, int);
} TipoLista;

int main(int argc, char **argv) {
    const char *saida = argc > 1 ? argv[1] : "resultados/resultados.csv";

    Algoritmo algs[] = {
        {"bubble_basico",     bubble_basico,     20000},
        {"bubble_otimizado",  bubble_otimizado,  20000},
        {"insertion_basico",  insertion_basico,  20000},
        {"insertion_binario", insertion_binario, 20000},
        /* O limite controla o custo O(n²) e a profundidade da recursão. */
        {"quick_basico",      quick_basico,      20000},
        {"quick_otimizado",   quick_otimizado,   0},
        {"merge_basico",      merge_basico,      0},
        {"merge_otimizado",   merge_otimizado,   0},
    };
    TipoLista tipos[] = {
        {"aleatoria",      gerar_aleatoria},
        {"ordenada",       gerar_ordenada},
        {"inversa",        gerar_inversa},
        {"quase_ordenada", gerar_quase_ordenada},
        {"repetidos",      gerar_repetidos},
    };
    int tamanhos[] = {1000, 5000, 10000, 20000, 50000, 100000};

    int na = sizeof algs / sizeof algs[0];
    int nt = sizeof tipos / sizeof tipos[0];
    int ns = sizeof tamanhos / sizeof tamanhos[0];

    FILE *f = fopen(saida, "w");
    if (!f) { perror("nao foi possivel abrir o CSV"); return 1; }
    fprintf(f, "algoritmo,tipo_lista,tamanho,tempo_ms,comparacoes,movimentacoes,desvio_ms\n");
    fflush(f);

    for (int t = 0; t < nt; t++)
        for (int s = 0; s < ns; s++) {
            int n = tamanhos[s];
            int *original = malloc((size_t)n * sizeof(int));
            if (!original) {
                fprintf(stderr, "ERRO: memoria insuficiente para o vetor original\n");
                fclose(f);
                return 1;
            }
            semear(SEMENTE);                        /* mesma entrada sempre */
            tipos[t].gera(original, n);

            for (int a = 0; a < na; a++) {
                if (algs[a].limite_n && n > algs[a].limite_n) continue;

                double tempos[REPETICOES];
                long long comp = 0, movimentacoes = 0;
                int *aquecimento = copiar_vetor(original, n);
                if (!aquecimento) {
                    fprintf(stderr, "ERRO: memoria insuficiente no aquecimento\n");
                    free(original);
                    fclose(f);
                    return 1;
                }
                algs[a].fn(aquecimento, n);
                free(aquecimento);
                for (int r = 0; r < REPETICOES; r++) {
                    int *copia = copiar_vetor(original, n);   /* mesma entrada p/ todos */
                    if (!copia) {
                        fprintf(stderr, "ERRO: memoria insuficiente para uma copia\n");
                        free(original);
                        fclose(f);
                        return 1;
                    }
                    zerar_contadores();
                    double ini = agora_ms();
                    algs[a].fn(copia, n);
                    double ms = agora_ms() - ini;
                    if (!esta_ordenado(copia, n) ||
                        soma_vetor(copia, n) != soma_vetor(original, n)) {
                        fprintf(stderr, "ERRO: %s nao ordenou (%s, n=%d)\n",
                                algs[a].nome, tipos[t].nome, n);
                        free(copia);
                        free(original);
                        fclose(f);
                        return 1;
                    }
                    comp = g_comparacoes;
                    movimentacoes = g_movimentacoes;
                    if (ms < LIMIAR_MEDICAO_MS) {
                        int *lote[TAMANHO_LOTE];
                        for (int i = 0; i < TAMANHO_LOTE; i++) {
                            lote[i] = copiar_vetor(original, n);
                            if (!lote[i]) {
                                fprintf(stderr, "ERRO: memoria insuficiente no lote\n");
                                for (int j = 0; j < i; j++) free(lote[j]);
                                free(copia);
                                free(original);
                                fclose(f);
                                return 1;
                            }
                        }
                        ini = agora_ms();
                        for (int i = 0; i < TAMANHO_LOTE; i++) algs[a].fn(lote[i], n);
                        ms = (agora_ms() - ini) / TAMANHO_LOTE;
                        for (int i = 0; i < TAMANHO_LOTE; i++) {
                            if (!esta_ordenado(lote[i], n) ||
                                soma_vetor(lote[i], n) != soma_vetor(original, n)) {
                                fprintf(stderr, "ERRO: %s nao ordenou (%s, n=%d)\n",
                                        algs[a].nome, tipos[t].nome, n);
                                for (int j = 0; j < TAMANHO_LOTE; j++) free(lote[j]);
                                free(copia);
                                free(original);
                                fclose(f);
                                return 1;
                            }
                            free(lote[i]);
                        }
                    }
                    tempos[r] = ms;
                    free(copia);
                }
                double media = 0, desvio = 0;
                for (int r = 0; r < REPETICOES; r++) media += tempos[r];
                media /= REPETICOES;
                for (int r = 0; r < REPETICOES; r++) {
                    double diferenca = tempos[r] - media;
                    desvio += diferenca * diferenca;
                }
                desvio = sqrt(desvio / (REPETICOES - 1));
                fprintf(f, "%s,%s,%d,%.4f,%lld,%lld,%.4f\n", algs[a].nome,
                        tipos[t].nome, n, media, comp, movimentacoes, desvio);
                fflush(f);
                printf("%-18s %-15s n=%-7d %10.3f ms\n", algs[a].nome,
                       tipos[t].nome, n, media);
            }
            free(original);
        }
    fclose(f);
    printf("\nResultados salvos em %s\n", saida);
    return 0;
}
