#include "generators.h"
#include "sorts.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double measure_sort(void (*sort_fn)(int *, size_t), int *data, size_t n) {
    clock_t start = clock();
    sort_fn(data, n);
    clock_t end = clock();

    return ((double)(end - start) / (double)CLOCKS_PER_SEC) * 1000.0;
}

int main(void) {
    const size_t n = 1000;
    int *base = malloc(n * sizeof(int));
    int *work = malloc(n * sizeof(int));

    if (!base || !work) {
        fprintf(stderr, "Erro ao alocar memória.\n");
        free(base);
        free(work);
        return 1;
    }

    srand((unsigned int)time(NULL));
    generate_random_list(base, n, 0, 10000);

    copy_array(base, work, n);
    double bubble_ms = measure_sort(bubble_sort, work, n);

    copy_array(base, work, n);
    double insertion_ms = measure_sort(insertion_sort, work, n);

    copy_array(base, work, n);
    double selection_ms = measure_sort(selection_sort, work, n);

    printf("Benchmark (%zu elementos)\n", n);
    printf("Bubble Sort:    %.3f ms\n", bubble_ms);
    printf("Insertion Sort: %.3f ms\n", insertion_ms);
    printf("Selection Sort: %.3f ms\n", selection_ms);
    printf("Vetor final ordenado? %s\n", is_sorted(work, n) ? "sim" : "não");

    free(base);
    free(work);

    return 0;
}
