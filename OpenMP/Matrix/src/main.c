#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define RUNS 10

void print_matrix(const char *name, int *matrix, int rows, int cols)
{
    printf("Matrix '%s': \n", name);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
            printf("%3d ", matrix[i * cols + j]);
        printf("\n");
    }
    printf("\n");
}

int dot_product_seq(int *row, int *col, int col_offset, int length)
{
    int result = 0;
    for (int i = 0; i < length; i++)
        result += row[i] * col[i * col_offset];

    return result;
}

void matrix_mul_seq(int *matrix_a, int *matrix_b, int rows, int cols)
{
    int *result = malloc((size_t)rows * (size_t)rows * sizeof(int));

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            // Para cada Row de A
            int *current_row = &matrix_a[i * cols];
            // Para cada Col de B
            int *current_col = &matrix_b[j];
            // Calcule el producto punto
            result[i * rows + j] = dot_product_seq(current_row, current_col, rows, cols);
        }
    }

    free(result);
}

int dot_product_parallel(int *row, int *col, int col_offset, int length)
{
    int result = 0;
    #pragma omp for
    for (int i = 0; i < length; i++)
        result += row[i] * col[i * col_offset];

    return result;
}

void matrix_mul_parallel(int *matrix_a, int *matrix_b, int rows, int cols)
{
    int *result = malloc((size_t)rows * (size_t)rows * sizeof(int));

    #pragma omp parallel for collapse(2)
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            // Para cada Row de A
            int *current_row = &matrix_a[i * cols];
            // Para cada Col de B
            int *current_col = &matrix_b[j];
            // Calcule el producto punto
            result[i * rows + j] = dot_product_parallel(current_row, current_col, rows, cols);
        }
    }

    free(result);
}

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        printf("Usage: %s <rows> <cols>\n", argv[0]);
        exit(-1);
    }

    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);

    if (rows < 2 || cols < 2)
    {
        printf("At least 2 rows and cols, got %d and %d", rows, cols);
        exit(-1);
    }

    srand((unsigned int)time(NULL));
    int *matrix_a = malloc((size_t)rows * (size_t)cols * sizeof(int));
    int *matrix_b = malloc((size_t)rows * (size_t)cols * sizeof(int));

    // Init random matrix A
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            matrix_a[i * cols + j] = rand() % 10;

    // Init random matrix B
    for (int i = 0; i < cols; i++)
        for (int j = 0; j < rows; j++)
            matrix_b[i * rows + j] = rand() % 10;

    double times_seq[RUNS], times_par[RUNS] = {};
    double start, end;
    double seq_avg = 0;
    double par_avg = 0;

    printf("\n>>> Ejecutando secuencial...\n");
    for (int i = 0; i < RUNS; i++)
    {
        start = omp_get_wtime();
        matrix_mul_seq(matrix_a, matrix_b, rows, cols);
        end = omp_get_wtime();
        times_seq[i] = end - start;
    }

    printf("\n>>> Ejecutando en paralelo...\n");
    for (int i = 0; i < RUNS; i++)
    {
        start = omp_get_wtime();
        matrix_mul_parallel(matrix_a, matrix_b, rows, cols);
        end = omp_get_wtime();
        times_par[i] = end - start;
    }

    for (int i = 0; i < RUNS; i++)
    {
        seq_avg += times_seq[i];
        par_avg += times_par[i];
    }
    seq_avg = seq_avg / RUNS;
    par_avg = par_avg / RUNS;

    printf("\n>>> Promedios de %d corridas:\n", RUNS);
    printf("    Secuencial: %.10f\n", seq_avg);
    printf("    Paralelo  : %.10f\n", par_avg);

    return 0;
}