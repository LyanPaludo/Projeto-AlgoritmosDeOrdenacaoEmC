#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sorts.h"
#include "utils.h"

#define CORTE 16   /* tamanho abaixo do qual Quick/Merge usam Insertion Sort */

static void troca(int *a, int *b) {
    int t = *a; *a = *b; *b = t;
    g_movimentacoes++;
}

/* Deslocamentos contam como movimentações para manter a métrica histórica. */
static void insertion_intervalo(int *v, int lo, int hi) {
    for (int i = lo + 1; i <= hi; i++) {
        int chave = v[i];
        int j = i - 1;
        while (j >= lo) {
            g_comparacoes++;
            if (v[j] > chave) { v[j + 1] = v[j]; g_movimentacoes++; j--; }
            else break;
        }
        v[j + 1] = chave;
    }
}

/* ---------- Bubble ---------- */
void bubble_basico(int *v, int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - 1 - i; j++) {
            g_comparacoes++;
            if (v[j] > v[j + 1]) troca(&v[j], &v[j + 1]);
        }
}

void bubble_otimizado(int *v, int n) {
    int fim = n - 1, trocou;
    do {
        trocou = 0;
        int ultimo = 0;
        for (int j = 0; j < fim; j++) {
            g_comparacoes++;
            if (v[j] > v[j + 1]) {
                troca(&v[j], &v[j + 1]);
                trocou = 1;
                ultimo = j;          /* depois de 'ultimo' ja esta ordenado */
            }
        }
        fim = ultimo;
    } while (trocou);
}

/* ---------- Insertion ---------- */
void insertion_basico(int *v, int n) {
    insertion_intervalo(v, 0, n - 1);
}

void insertion_binario(int *v, int n) {
    for (int i = 1; i < n; i++) {
        int chave = v[i], ini = 0, fim = i;
        while (ini < fim) {
            int meio = (ini + fim) / 2;
            g_comparacoes++;
            if (chave < v[meio]) fim = meio;
            else ini = meio + 1;
        }
        for (int j = i; j > ini; j--) { v[j] = v[j - 1]; g_movimentacoes++; }
        v[ini] = chave;
    }
}

/* ---------- Quick ---------- */
static void quick_b(int *v, int lo, int hi) {
    if (lo >= hi) return;
    int p = v[hi], i = lo - 1;
    for (int j = lo; j < hi; j++) {
        g_comparacoes++;
        if (v[j] <= p) { i++; troca(&v[i], &v[j]); }
    }
    troca(&v[i + 1], &v[hi]);
    quick_b(v, lo, i);
    quick_b(v, i + 2, hi);
}

void quick_basico(int *v, int n) {
    quick_b(v, 0, n - 1);
}

static void quick_o(int *v, int lo, int hi) {
    while (hi - lo + 1 > CORTE) {
        int meio = lo + (hi - lo) / 2;
        /* mediana de tres: ordena v[lo], v[meio], v[hi] */
        g_comparacoes++; if (v[meio] < v[lo]) troca(&v[meio], &v[lo]);
        g_comparacoes++; if (v[hi]   < v[lo]) troca(&v[hi],   &v[lo]);
        g_comparacoes++; if (v[hi] < v[meio]) troca(&v[hi], &v[meio]);
        troca(&v[meio], &v[hi - 1]);          /* pivo (mediana) em hi-1 */
        int p = v[hi - 1], i = lo, j = hi - 1;
        for (;;) {
            do { i++; g_comparacoes++; } while (v[i] < p);
            do { j--; g_comparacoes++; } while (v[j] > p);
            if (i >= j) break;
            troca(&v[i], &v[j]);
        }
        troca(&v[i], &v[hi - 1]);             /* pivo na posicao final */
        /* recursao na menor parte; laco na maior (limita a pilha a O(log n)) */
        if (i - lo < hi - i) { quick_o(v, lo, i - 1); lo = i + 1; }
        else                 { quick_o(v, i + 1, hi); hi = i - 1; }
    }
    insertion_intervalo(v, lo, hi);
}

void quick_otimizado(int *v, int n) {
    quick_o(v, 0, n - 1);
}

/* ---------- Merge ---------- */
static void merge_b(int *v, int n) {
    if (n < 2) return;
    int m = n / 2;
    merge_b(v, m);
    merge_b(v + m, n - m);
    int *aux = malloc((size_t)n * sizeof(int));   /* alocacao a cada chamada */
    if (!aux) {
        fprintf(stderr, "ERRO: memoria insuficiente no merge_basico\n");
        exit(EXIT_FAILURE);
    }
    int i = 0, j = m, k = 0;
    while (i < m && j < n) {
        g_comparacoes++;
        aux[k++] = (v[i] <= v[j]) ? v[i++] : v[j++];
    }
    while (i < m) aux[k++] = v[i++];
    while (j < n) aux[k++] = v[j++];
    memcpy(v, aux, (size_t)n * sizeof(int));
    g_movimentacoes += n;
    free(aux);
}

void merge_basico(int *v, int n) {
    merge_b(v, n);
}

static void merge_o(int *v, int *aux, int lo, int hi) {
    if (hi - lo + 1 <= CORTE) { insertion_intervalo(v, lo, hi); return; }
    int m = lo + (hi - lo) / 2;
    merge_o(v, aux, lo, m);
    merge_o(v, aux, m + 1, hi);
    int i = lo, j = m + 1, k = lo;
    while (i <= m && j <= hi) {
        g_comparacoes++;
        aux[k++] = (v[i] <= v[j]) ? v[i++] : v[j++];
    }
    while (i <= m) aux[k++] = v[i++];
    while (j <= hi) aux[k++] = v[j++];
    for (k = lo; k <= hi; k++) v[k] = aux[k];
    g_movimentacoes += hi - lo + 1;
}

void merge_otimizado(int *v, int n) {
    int *aux = malloc((size_t)n * sizeof(int));   /* alocado uma unica vez */
    if (!aux) {
        fprintf(stderr, "ERRO: memoria insuficiente no merge_otimizado\n");
        exit(EXIT_FAILURE);
    }
    merge_o(v, aux, 0, n - 1);
    free(aux);
}
