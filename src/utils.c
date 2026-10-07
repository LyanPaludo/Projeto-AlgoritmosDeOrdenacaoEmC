#include "utils.h"

#include <string.h>

void copy_array(const int *src, int *dst, size_t n) {
    if (!src || !dst || n == 0) {
        return;
    }

    memcpy(dst, src, n * sizeof(int));
}

int is_sorted(const int *arr, size_t n) {
    if (!arr || n < 2) {
        return 1;
    }

    for (size_t i = 1; i < n; i++) {
        if (arr[i - 1] > arr[i]) {
            return 0;
        }
    }

    return 1;
}
