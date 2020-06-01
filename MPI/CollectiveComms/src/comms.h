#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>

/*
    Sends data from one process to all other processes in a communicator 
*/
void Scatter(
    const void *sendbuf,
    int sendcount,
    MPI_Datatype sendtype,
    void *recvbuf,
    int recvcount,
    MPI_Datatype recvtype,
    int root,
    MPI_Comm comm)
{
    int nprocs, rank;
    MPI_Comm_size(comm, &nprocs);
    MPI_Comm_rank(comm, &rank);

    int send_size;
    MPI_Type_size(sendtype, &send_size);

    if (rank == root)
    {
        for (int i = 0; i < nprocs; i++)
        {
            const void *buff = (sendbuf + (i * sendcount * send_size));
            if (i == root)
                memcpy(recvbuf, buff, (size_t)(sendcount * send_size));
            else
                MPI_Send(buff, sendcount, sendtype, i, 0, comm);
        }
    }
    else
    {
        MPI_Recv(recvbuf, recvcount, recvtype, root, 0, comm, MPI_STATUS_IGNORE);
    }
}

/*
    Broadcasts a message from the process with rank "root" to all other processes of the communicator 
*/
void Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm)
{
    int nprocs, rank;
    MPI_Comm_size(comm, &nprocs);
    MPI_Comm_rank(comm, &rank);

    if (rank == root)
    {
        for (int i = 0; i < nprocs; i++)
        {
            if (i == root)
                continue;
            MPI_Send(buffer, count, datatype, i, 0, comm);
        }
    }
    else
    {
        MPI_Recv(buffer, count, datatype, root, 0, comm, MPI_STATUS_IGNORE);
    }
}

/* 
    Blocks until all processes in the communicator have reached this routine.
*/
void Barrier(MPI_Comm comm)
{
    int barrier_token = (int)0x1111;
    Bcast(&barrier_token, 1, MPI_INT, 0, comm);
}

/*
    Gathers together values from a group of processes 
*/
void Gather(
    const void *sendbuf,
    int sendcount,
    MPI_Datatype sendtype,
    void *recvbuf,
    int recvcount,
    MPI_Datatype recvtype,
    int root,
    MPI_Comm comm)
{
    int nprocs, rank;
    MPI_Comm_size(comm, &nprocs);
    MPI_Comm_rank(comm, &rank);

    if (rank == root)
    {
        MPI_Status status;

        int recv_size;
        MPI_Type_size(recvtype, &recv_size);

        size_t buffer_size = (size_t)(recvcount * recv_size);
        void *buff = malloc(buffer_size);

        for (int i = 0; i < nprocs; i++)
        {
            if (i != root)
            {
                MPI_Recv(buff, recvcount, recvtype, MPI_ANY_SOURCE, MPI_ANY_TAG, comm, &status);
                memcpy(recvbuf + ((int)buffer_size * status.MPI_TAG), buff, buffer_size);
            }
            else
            {
                memcpy(recvbuf + ((int)buffer_size * root), sendbuf, buffer_size);
            }
        }
    }
    else
    {
        MPI_Send(sendbuf, sendcount, sendtype, root, rank, comm);
    }
}

