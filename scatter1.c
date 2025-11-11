#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int rank, size;
    int N;  // size of the table

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Root process asks the user for N
    if (rank == 0) {
        printf("Enter the size of the table (N): ");
        scanf("%d", &N);

        // Check if N divisible by number of processes
        if (N % size != 0) {
            printf("Error: N (%d) must be divisible by number of processes (%d)\n", N, size);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    // Broadcast N to all processes
    MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int elements_per_proc = N / size;
    int *global_array = NULL;
    int *local_array = (int*) malloc(elements_per_proc * sizeof(int));

    // Root initializes the full array
    if (rank == 0) {
        global_array = (int*) malloc(N * sizeof(int));

        printf("Enter %d elements:\n", N);
        for (int i = 0; i < N; i++) {
            scanf("%d", &global_array[i]);
        }
    }

    // Scatter the data to all processes
    MPI_Scatter(global_array, elements_per_proc, MPI_INT,
                local_array, elements_per_proc, MPI_INT,
                0, MPI_COMM_WORLD);

    // Compute local sum
    int local_sum = 0;
    for (int i = 0; i < elements_per_proc; i++) {
        local_sum += local_array[i];
    }
    printf("Process %d local sum = %d\n", rank, local_sum);

    // Reduce to root
    int global_sum = 0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("\nFinal global sum = %d\n", global_sum);
        free(global_array);
    }

    free(local_array);
    MPI_Finalize();
    return 0;
}
