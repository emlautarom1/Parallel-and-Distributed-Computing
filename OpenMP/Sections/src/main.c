#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define LENGTH 1000000
#define RUNS 50

/*
Realice un programa OpenMP el cual disponga de un conjunto de funciones capaces
de calcular el mínimo, máximo, la multiplicación y desviación estándar de un conjunto
de un millón de elementos contenidos en un arreglo.

Cada una de estas funciones deberá recibir por parámetro los datos a procesar y
deberá devolver el valor resultante.

Se pide que realice un programa OpenMP que sea capaz de realizar en paralelo todas
estas operaciones. El programa además deberá informar por pantalla que thread ha
realizado que operación.

Finalmente se pide que calcule el speedup de esta aplicación. Para realizar este
cálculo debe trabajar con los valores promedios obtenidos de 50 ejecuciones tanto
para la solución paralela como para la solución secuencial.
 */

int min(int *arr)
{
    int min = INT_MAX;
    for (int i = 0; i < LENGTH; i++)
    {
        if (arr[i] < min)
            min = arr[i];
    }
    return min;
}

int max(int *arr)
{
    int max = INT_MIN;
    for (int i = 0; i < LENGTH; i++)
    {
        if (arr[i] > max)
            max = arr[i];
    }
    return max;
}

int mult(int *arr)
{
    int result = 1;
    for (int i = 0; i < LENGTH; i++)
    {
        result *= arr[i];
    }
    return result;
}

double deviation(int *arr)
{
    double avg = 0;
    for (int i = 0; i < LENGTH; i++)
        avg += arr[i] / LENGTH;

    double variance = 0;
    for (int i = 0; i < LENGTH; i++)
        variance += pow(arr[i] - avg, 2) / LENGTH;

    return sqrt(variance);
}

void run_seq(int *arr)
{
    min(arr);
    max(arr);
    mult(arr);
    deviation(arr);
}

void run_par(int *arr)
{
    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                min(arr);
            }
            #pragma omp section
            {
               max(arr);
            }
            #pragma omp section
            {
                mult(arr);
            }
            #pragma omp section
            {
                deviation(arr);
            }
        }
    }
}

int main(void)
{
    srand((unsigned int)time(NULL));
    int *arr = malloc((size_t)LENGTH * sizeof(int));
    for (int i = 0; i < LENGTH; i++)
        arr[i] = rand() % 10;

    double times_seq[RUNS], times_par[RUNS] = {};
    double start, end;
    double seq_avg = 0;
    double par_avg = 0;

    printf("\n>>> Ejecutando secuencial...\n");
    for (int i = 0; i < RUNS; i++)
    {
        start = omp_get_wtime();
        run_seq(arr);
        end = omp_get_wtime();
        times_seq[i] = end - start;
    }

    printf("\n>>> Ejecutando en paralelo...\n");
    for (int i = 0; i < RUNS; i++)
    {
        start = omp_get_wtime();
        run_par(arr);
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

    free(arr);

    return 0;
}