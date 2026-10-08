#ifndef GENERATORS_H
#define GENERATORS_H

/* Os geradores usam um xorshift reproduzível; chame semear antes dos testes. */
void semear(unsigned long long s);
void gerar_aleatoria(int *v, int n);
void gerar_ordenada(int *v, int n);
void gerar_inversa(int *v, int n);
void gerar_quase_ordenada(int *v, int n);   /* ~1% dos elementos fora do lugar */
void gerar_repetidos(int *v, int n);        /* apenas 10 valores distintos */

#endif
