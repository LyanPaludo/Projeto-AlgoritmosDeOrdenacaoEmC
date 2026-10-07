#ifndef GENERATORS_H
#define GENERATORS_H

#include <stddef.h>

void generate_random_list(int *arr, size_t n, int min_value, int max_value);
void generate_sorted_list(int *arr, size_t n);
void generate_reverse_sorted_list(int *arr, size_t n);

#endif
