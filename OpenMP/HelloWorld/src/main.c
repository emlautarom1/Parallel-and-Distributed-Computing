#include <stdio.h>
#include <omp.h>

int main(void)
{
    int nthreads, tid; // tid es una variable privada a cada thread
    #pragma omp parallel private(tid) // Sincronización implicita -> FORK
    {
        // Obtenemos el id de cada thread
        tid = omp_get_thread_num();
        printf("Hola mundo desde el thread = % d\n", tid);
        if (tid == 0) // Solo el thread maestro
        {
            nthreads = omp_get_num_threads();
            printf("Numero total de threads = % d\n", nthreads);
        }
    } // sincronización implicita -> JOIN
    return 0;
}