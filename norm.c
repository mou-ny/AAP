#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

int main(int argc, char** argv) {
    int pid, nproc;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    if (argc < 3) {
        if (pid == 0) 
            printf("Usage: mpirun -np <p> ./prog <root> <N>\n");
        MPI_Finalize();
        return 1;
    }

    int root = atoi(argv[1]);
    int N = atoi(argv[2]);

    int *buffer = NULL;
    int *counts = malloc(nproc * sizeof(int));
    int *displs = malloc(nproc * sizeof(int));

    int base = N / nproc;
    int rest = N % nproc;

    // Compute counts and displacements
    int offset = 0;
    for (int i = 0; i < nproc; i++) {
        counts[i] = base + (i < rest ? 1 : 0);
        displs[i] = offset;
        offset += counts[i];
    }

    // Root initializes the full buffer
    if (pid == root) {
        buffer = malloc(N * sizeof(int));
        printf("Array: ");
        for (int i = 0; i < N; i++) {
            buffer[i] = rand() % 100;      // random values
            printf("%d ", buffer[i]);
        }
        printf("\n");
    }

    // Allocate sub-buffer for each process
    int n_loc = counts[pid];
    int *sub_buffer = malloc(n_loc * sizeof(int));

    // Scatterv to distribute segments
    MPI_Scatterv(buffer, counts, displs, MPI_INT,
                 sub_buffer, n_loc, MPI_INT,
                 root, MPI_COMM_WORLD);

    // Compute local sum of squares
    int local_sum = 0;
    for (int i = 0; i < n_loc; i++) {
        local_sum += sub_buffer[i] * sub_buffer[i];
    }

    printf("Process %d local sum of squares = %d\n", pid, local_sum);

    // Global sum of squares
    int global_sum = 0;
    MPI_Allreduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    // Compute Euclidean norm
    double norm = sqrt((double)global_sum);

    if (pid == root)
        printf("\nVector L2 norm = %.6f\n", norm);

    // Normalize local elements
    double *sub_norm = malloc(n_loc * sizeof(double));

    for (int i = 0; i < n_loc; i++) {
        sub_norm[i] = sub_buffer[i] / norm;
    }

    // Prepare gather for normalized vector
    double *T_norm = NULL;
    if (pid == root)
        T_norm = malloc(N * sizeof(double));

    MPI_Gatherv(sub_norm, n_loc, MPI_DOUBLE,
                T_norm, counts, displs, MPI_DOUBLE,
                root, MPI_COMM_WORLD);

    // Root prints the normalized vector
    if (pid == root) {
        printf("\nNormalized vector:\n");
        for (int i = 0; i < N; i++)
            printf("%.4f ", T_norm[i]);
        printf("\n");
    }

    // Free memory
    free(sub_buffer);
    free(sub_norm);
    free(counts);
    free(displs);
    if (pid == root) {
        free(buffer);
        free(T_norm);
    }

    MPI_Finalize();
    return 0;
}
