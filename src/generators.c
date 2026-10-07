#include "generators.h"

#include <stdlib.h>

void generate_random_list(int *arr, size_t n, int min_value, int max_value) {
    if (!arr || n == 0 || min_value > max_value) {
        return;
    }

    int range = max_value - min_value + 1;
    for (size_t i = 0; i < n; i++) {
        arr[i] = min_value + (rand() % range);
    }
}

void generate_sorted_list(int *arr, size_t n) {
    if (!arr) {
        return;
    }

    for (size_t i = 0; i < n; i++) {
        arr[i] = (int)i;
    }
}

void generate_reverse_sorted_list(int *arr, size_t n) {
    if (!arr) {
        return;
    }

    for (size_t i = 0; i < n; i++) {
        arr[i] = (int)(n - 1 - i);
    }
}
