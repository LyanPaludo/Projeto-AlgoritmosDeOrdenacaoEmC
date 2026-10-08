#define _POSIX_C_SOURCE 199309L

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"

long long g_comparacoes = 0;
long long g_movimentacoes = 0;

void zerar_contadores(void) {
    g_comparacoes = 0;
    g_movimentacoes = 0;
}

int *copiar_vetor(const int *v, int n) {
    int *c = malloc((size_t)n * sizeof(int));
    if (c) memcpy(c, v, (size_t)n * sizeof(int));
    return c;
}

int esta_ordenado(const int *v, int n) {
    for (int i = 1; i < n; i++)
        if (v[i - 1] > v[i]) return 0;
    return 1;
}

long long soma_vetor(const int *v, int n) {
    long long soma = 0;
    for (int i = 0; i < n; i++) soma += v[i];
    return soma;
}

double agora_ms(void) {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec * 1000.0 + t.tv_nsec / 1e6;
}
