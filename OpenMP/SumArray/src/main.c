#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define ARRAY_SIZE 1000000000

void sum_arrays_parallel(void)
{
    int *array_a = (int *)malloc((size_t)ARRAY_SIZE * sizeof(int));
    int *array_b = (int *)malloc((size_t)ARRAY_SIZE * sizeof(int));
    int *result = (int *)malloc((size_t)ARRAY_SIZE * sizeof(int));
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        array_a[i] = i;
        array_b[i] = i;
    }

    #pragma omp parallel for
    for (int i = 0; i < ARRAY_SIZE; i++)
        result[i] = array_a[i] + array_b[i];

    free(array_a);
    free(array_b);
    free(result);
}

void sum_arrays_seq(void)
{
    int *array_a = (int *)malloc((size_t)ARRAY_SIZE * sizeof(int));
    int *array_b = (int *)malloc((size_t)ARRAY_SIZE * sizeof(int));
    int *result = (int *)malloc((size_t)ARRAY_SIZE * sizeof(int));
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        array_a[i] = i;
        array_b[i] = i;
    }

    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        result[i] = array_a[i] + array_b[i];
    }

    free(array_a);
    free(array_b);
    free(result);
}

int main(void)
{
    double start, end;
    start = omp_get_wtime();
    sum_arrays_seq();
    end = omp_get_wtime();
    printf("\n>>> Tiempo (seq): %.10f\n", end - start);
    start = omp_get_wtime();
    sum_arrays_parallel();
    end = omp_get_wtime();
    printf("\n>>> Tiempo (par): %.10f\n", end - start);
    return 0;
}
