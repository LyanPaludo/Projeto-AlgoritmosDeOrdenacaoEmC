#include <stdlib.h>
#include "generators.h"

static unsigned long long estado = 1;

void semear(unsigned long long s) {
    estado = s ? s : 1;
}

static unsigned long long proximo_aleatorio(void) {
    estado ^= estado << 13;
    estado ^= estado >> 7;
    estado ^= estado << 17;
    return estado & 0x7fffffffULL;
}

void gerar_aleatoria(int *v, int n) {
    for (int i = 0; i < n; i++)
        v[i] = (int)(proximo_aleatorio() % 1000000ULL);
}

void gerar_ordenada(int *v, int n) {
    for (int i = 0; i < n; i++) v[i] = i;
}

void gerar_inversa(int *v, int n) {
    for (int i = 0; i < n; i++) v[i] = n - i;
}

void gerar_quase_ordenada(int *v, int n) {
    gerar_ordenada(v, n);
    int trocas = n / 200 > 0 ? n / 200 : 1;
    for (int k = 0; k < trocas; k++) {
        int a = (int)(proximo_aleatorio() % (unsigned long long)n);
        int b = (int)(proximo_aleatorio() % (unsigned long long)n);
        int t = v[a]; v[a] = v[b]; v[b] = t;
    }
}

void gerar_repetidos(int *v, int n) {
    for (int i = 0; i < n; i++)
        v[i] = (int)(proximo_aleatorio() % 10ULL);
}
