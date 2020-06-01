#include <mpi.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    int nprocs, rank;
    MPI_Status status;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (argc < 2)
    {
        printf("Uso: %s <nro-vueltas>\n", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, -1);
    }

    int lap_count = atoi(argv[1]);
    int target_proc = ((rank + 1) % nprocs);
    int listen_proc = ((rank - 1 + nprocs) % nprocs);

    for (int lap = 0; lap < lap_count; lap++)
    {
        char message[BUFSIZ];

        if (rank == 0)
        {
            printf("Iniciando vuelta %d\n", lap);

            sprintf(message, "0");
            MPI_Send(message, BUFSIZ, MPI_CHAR, target_proc, 0, MPI_COMM_WORLD);
            MPI_Recv(message, BUFSIZ, MPI_CHAR, listen_proc, 0, MPI_COMM_WORLD, &status);

            printf("Mensaje final de vuelta %d:\n  %s\n", lap, message);
            printf("---\n");
        }
        else
        {
            MPI_Recv(message, BUFSIZ, MPI_CHAR, listen_proc, 0, MPI_COMM_WORLD, &status);

            char rank_msg[8];
            sprintf(rank_msg, " < %d", rank);
            strcat(message, rank_msg);

            MPI_Send(message, BUFSIZ, MPI_CHAR, target_proc, 0, MPI_COMM_WORLD);
        }
    }
    MPI_Finalize();
}
