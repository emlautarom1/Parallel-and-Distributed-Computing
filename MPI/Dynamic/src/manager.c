#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>

#define MASTER_RANK 0
#define CHILD_N 5

/*
    Reference:
    https://etutorials.org/Linux+systems/cluster+computing+with+linux/Part+II+Parallel+Programming/Chapter+9+Advanced+Topics+in+MPI+Programming/

    MPI_Comm_spawn:
    https://stackoverflow.com/questions/21497605/creating-child-process-with-mpi-comm-spawn
    
    MPI_Comm_spawn is a collective call and it does not spawn n additional processes per rank but rather it spawns a child MPI job with n processes, therefore adding n to the total number of processes. When called with n = 2, it spawn a child  job with 2 processes and that's exactly what you observe in the output.
    
    To spawn n processes from one parent, you have to use MPI_COMM_SELF.
*/

void error(const char *string)
{
    printf("%s", string);
    exit(-1);
}

int main(int argc, char *argv[])
{
    /*
        Intercommunicator:
        
        Binds two groups of processes, the local group and remote groups.
        Ranks in intercommunicator always refer to the processes in the remote group.
        The local group contains the parents and the remote the children.
    */
    MPI_Comm workercomm;
    int world_size;
    char workerProgram[] = "./build/worker";

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    printf("Manager: MPI_COMM_WORLD size = %d\n", world_size);

    if (world_size != 1)
        error("Top heavy with management\n");

    printf("Manager: Running...\n");

    // Spawn Workers
    MPI_Comm_spawn(
        workerProgram,
        MPI_ARGV_NULL,
        CHILD_N,
        MPI_INFO_NULL,
        MASTER_RANK,
        MPI_COMM_SELF,
        &workercomm,
        MPI_ERRCODES_IGNORE);

    //Finalize Process Root
    MPI_Finalize();

    return 0;
}
