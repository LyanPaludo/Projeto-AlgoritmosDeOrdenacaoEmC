#ifndef UTILS_H
#define UTILS_H

/* Contadores globais, zerados antes de cada execucao */
extern long long g_comparacoes;
/* Bubble/Quick contam trocas; Insertion deslocamentos; Merge cópias na intercalação. */
extern long long g_movimentacoes;

void zerar_contadores(void);
int *copiar_vetor(const int *v, int n);
int esta_ordenado(const int *v, int n);
long long soma_vetor(const int *v, int n);
double agora_ms(void);

#endif
