#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

void error(const char *string)
{
    printf("%s", string);
    exit(-1);
}

int main(int argc, char *argv[])
{

    // Parent Communicator
    MPI_Comm parent;
    // Parent communication group size
    int parent_size, children_size, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_get_parent(&parent);
    if (parent == MPI_COMM_NULL)
        error("No Parent!\n");
    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &children_size);
    MPI_Comm_remote_size(parent, &parent_size);
    if (parent_size != 1)
        error("Something's wrong with the parent");

    printf("Worker : Running (%d/%d)...\n", rank, children_size - 1);

    MPI_Finalize();
    return 0;
}
