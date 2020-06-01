#include <mpi.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MASTER_RANK 0

void print_matrix(const char *name, int *matrix, int rows, int cols);

int main(int argc, char const *argv[])
{
    int nprocs, rank;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (argc < 3)
    {
        printf("Usage: %s <rows> <cols>", argv[0]);
        MPI_Abort(MPI_COMM_WORLD, -1);
    }

    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);

    if (rows < 2 || cols < 2)
    {
        printf("At least 2 rows and cols, got %d and %d", rows, cols);
        MPI_Abort(MPI_COMM_WORLD, -1);
    }

    // Helper alias
    int vector_length = cols;

    // For master rank
    int *matrix;
    int *op_result;
    int *cells_per_proc;
    int *rows_per_proc;
    int *send_displacements;
    int *recv_displacements;

    // For each process
    int *vector = (int *)malloc((size_t)vector_length * (int)sizeof(int));
    int my_cell_count, my_row_count;
    int *my_cells, *my_results;

    if (rank == MASTER_RANK)
    {
        // Init random number generation
        srand((unsigned int)time(NULL));

        matrix = (int *)malloc((size_t)(rows * cols * (int)sizeof(int)));
        op_result = (int *)malloc((size_t)rows * (int)sizeof(int));

        // Init random matrix
        for (int i = 0; i < rows; i++)
            for (int j = 0; j < cols; j++)
                matrix[i * cols + j] = rand() % 10;

        // Init random vector
        for (int i = 0; i < vector_length; i++)
            vector[i] = rand() % 10;

        print_matrix("Initial", matrix, rows, cols);
        print_matrix("Vector", vector, vector_length, 1);

        // Calculate rows & cells to process per slave proc
        cells_per_proc = (int *)malloc((size_t)nprocs * (int)sizeof(int));
        rows_per_proc = (int *)malloc((size_t)nprocs * (int)sizeof(int));
        for (int i = 0; i < nprocs; i++)
        {
            rows_per_proc[i] = (rows / nprocs + (i < rows % nprocs));
            cells_per_proc[i] = rows_per_proc[i] * cols;
        }

        // Calculate the displacements
        send_displacements = (int *)malloc((size_t)nprocs * (int)sizeof(int));
        recv_displacements = (int *)malloc((size_t)nprocs * (int)sizeof(int));
        for (int i = 0; i < nprocs; i++)
        {
            int displacement = 0;
            for (int j = 0; j < i; j++)
            {
                displacement += rows_per_proc[j];
            }
            recv_displacements[i] = displacement;
            send_displacements[i] = displacement * cols;
        }
    }

    // Broadcast the vector
    MPI_Bcast(vector, vector_length, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);

    // Send the number of cells assigned to each process
    MPI_Scatter(cells_per_proc, 1, MPI_INT, &my_cell_count, 1, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);
    my_row_count = my_cell_count / cols;

    // Allocate enough for the assigned celss
    my_cells = (int *)malloc((size_t)(my_cell_count * (int)sizeof(int)));

    // Send cells to each process
    MPI_Scatterv(matrix, cells_per_proc, send_displacements, MPI_INT, my_cells, my_cell_count, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);

    // Process the multiplication
    my_results = (int *)calloc((size_t)my_row_count, sizeof(int));
    for (int i = 0; i < my_row_count; i++)
        for (int j = 0; j < cols; j++)
            my_results[i] += my_cells[i * cols + j] * vector[j];

    // Recieve results from each process and build the final result
    MPI_Gatherv(my_results, my_row_count, MPI_INT, op_result, rows_per_proc, recv_displacements, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);

    if (rank == MASTER_RANK)
        print_matrix("Result", op_result, rows, 1);

    MPI_Finalize();
}

void print_matrix(const char *name, int *matrix, int rows, int cols)
{
    printf("Matrix '%s': \n", name);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
            printf("%2d ", matrix[i * cols + j]);
        printf("\n");
    }
    printf("\n");
}
