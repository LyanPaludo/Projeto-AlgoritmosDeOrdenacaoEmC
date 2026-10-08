#ifndef SORTS_H
#define SORTS_H

/* Mesma assinatura para todos: void sort(int *v, int n) */
void bubble_basico(int *v, int n);
void bubble_otimizado(int *v, int n);     /* flag de parada + intervalo reduzido */

void insertion_basico(int *v, int n);
void insertion_binario(int *v, int n);    /* busca binaria da posicao */

void quick_basico(int *v, int n);         /* pivo = ultimo elemento */
void quick_otimizado(int *v, int n);      /* mediana de 3 + Insertion em subvetores pequenos */

void merge_basico(int *v, int n);         /* aloca auxiliar a cada chamada */
void merge_otimizado(int *v, int n);      /* auxiliar unico + Insertion em partes pequenas */

#endif
