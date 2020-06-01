#include <mpi.h>
#include <string.h>
#include <stdio.h>

#define MASTER_PROC 3

void master_routine(int nprocs);
void master_ordered_routine(int nprocs);
void slave_routine(int rank);

int main(void)
{
    int nprocs, myrank;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    if (myrank == MASTER_PROC)
    {
        master_ordered_routine(nprocs);
    }
    else
    {
        slave_routine(myrank);
    }
    MPI_Finalize();
}

void master_routine(int nprocs)
{
    char buffer[BUFSIZ];
    MPI_Status status;
    printf("Hola, soy el proceso maestro %d (hay %d procesos) y recibo:\n", MASTER_PROC, nprocs);

    int remaining_messages = nprocs - 1;
    while (remaining_messages != 0)
    {
        MPI_Recv(buffer, BUFSIZ, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        printf(">> %s\n", buffer);
        remaining_messages--;
    }
}

void master_ordered_routine(int nprocs)
{
    char buffer[BUFSIZ];
    MPI_Status status;
    printf("Hola, soy el proceso maestro %d (hay %d procesos) y recibo:\n", MASTER_PROC, nprocs);
    for (int slave_id = 0; slave_id < nprocs; slave_id++)
    {
        if (slave_id == MASTER_PROC)
            continue;

        MPI_Recv(buffer, BUFSIZ, MPI_CHAR, slave_id, 0, MPI_COMM_WORLD, &status);
        printf(">> %s\n", buffer);
    }
}

void slave_routine(int rank)
{
    char message[BUFSIZ];
    sprintf(message, "Hola desde el proceso %d", rank);
    MPI_Send(message, BUFSIZ, MPI_CHAR, MASTER_PROC, 0, MPI_COMM_WORLD);
}