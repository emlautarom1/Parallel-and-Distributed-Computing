#include <stdio.h>
#include <omp.h>

#define N 1024

void compare_arrays(int a[], int b[])
{
    int err = 0;
    for (int i = 0; i < N && err != 0; i++)
    {
        if (a[i] != b[i])
        {
            printf("Resultados distintos en %d:\n  a[i] = %d\n  b[i] = %d\n", i, a[i], b[i]);
            err = 1;
        }
    }
    if (err == 0)
        printf("Ok\n");
}

void example_1(void)
{
    // Initialization
    int a[N], b[N] = {};
    int x = 32;
    for (int i = 0; i < N; i++)
    {
        a[i] = i;
        b[i] = i;
    }

    // Sequential
    for (int i = 0; i < N - 1; i++)
        a[i] = a[i + 1] + x;

#pragma omp for
    for (int i = 0; i < N - 1; i++)
        b[i] = b[i + 1] + x;

    compare_arrays(a, b);
}

void example_2(void)
{
    // Initialization
    int a[N], b[N], x[N] = {};
    for (int i = 0; i < N; i++)
    {
        a[i] = i;
        b[i] = i;
        x[i] = i;
    }

    // Sequential
    for (int i = 0; i < N - 1; i++)
        a[i] = a[i] + x[i];

#pragma omp for
    for (int i = 0; i < N - 1; i++)
        b[i] = b[i] + x[i];

    compare_arrays(a, b);
}

void example_3(void)
{
    int stride = -1;

    // Initialization
    int a[N], b[N], x[N] = {};
    for (int i = 0; i < N; i++)
    {
        a[i] = i;
        b[i] = i;
        x[i] = (N - i) % 7;
    }

    // Sequential
    int ix_a = N;
    for (int i = 0; i < N; i++)
    {
        a[ix_a] = a[ix_a] - x[i];
        ix_a += stride;
    }

    int ix_b = N;
#pragma omp for
    for (int i = 0; i < N; i++)
    {
        b[ix_b] = b[ix_b] - x[i];
        ix_b += stride;
    }

    compare_arrays(a, b);
}

void example_4(void)
{
    // Initialization
    int a[N], b[N], x[N] = {};
    for (int i = 0; i < N; i++)
    {
        a[i] = i;
        b[i] = i;
        x[i] = (N - i) % 7;
    }

    // Sequential
    for (int i = 1; i < N; i++)
        a[i] = (x[i] - x[i - 1]) * 0.5;

#pragma omp for
    for (int i = 1; i < N; i++)
        b[i] = (x[i] - x[i - 1]) * 0.5;

    compare_arrays(a, b);
}

int main(void)
{
    printf("Ejecutando ejemplo 1... ");
    example_1();
    printf("Ejecutando ejemplo 2... ");
    example_2();
    printf("Ejecutando ejemplo 3... ");
    example_3();
    printf("Ejecutando ejemplo 4... ");
    example_4();
}