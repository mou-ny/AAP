#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int pid, nproc;
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    if (argc < 3) {
        if (pid == 0) 
            printf("Usage: mpirun -np <p> ./prog <root> <N>\n");
        MPI_Finalize();
        return 0;
    }

    int root = atoi(argv[1]);
    int N = atoi(argv[2]);

    /* ---------- Compute counts + displacements ---------- */
    int *counts = malloc(nproc * sizeof(int));
    int *displs = malloc(nproc * sizeof(int));

    int base = N / nproc;
    int rest = N % nproc;

    int offset = 0;
    for (int i = 0; i < nproc; i++) {
        counts[i] = base + (i < rest);
        displs[i] = offset;
        offset += counts[i];
    }

    /* ---------- Create and fill vector on root ---------- */
    int *buffer = NULL;
    if (pid == root) {
        buffer = malloc(N * sizeof(int));
        printf("Array: ");
        for (int i = 0; i < N; i++) {
            buffer[i] = rand() % 100;
            printf("%d ", buffer[i]);
        }
        printf("\n");
    }

    /* ---------- Local buffer ---------- */
    int n_loc = counts[pid];
    int *sub_buf = malloc(n_loc * sizeof(int));

    MPI_Scatterv(buffer, counts, displs, MPI_INT,
                 sub_buf, n_loc, MPI_INT, root, MPI_COMM_WORLD);

    /* ---------- Compute local sum of squares ---------- */
    int local_sum = 0;
    for (int i = 0; i < n_loc; i++)
        local_sum += sub_buf[i] * sub_buf[i];

    /* ---------- Compute global sum ---------- */
    int global_sum = 0;
    MPI_Allreduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    double norm = sqrt((double)global_sum);

    if (pid == root)
        printf("\nVector L2 norm = %.6f\n", norm);

    /* ---------- Normalize locally ---------- */
    double *sub_norm = malloc(n_loc * sizeof(double));
    for (int i = 0; i < n_loc; i++)
        sub_norm[i] = sub_buf[i] / norm;

    /* ---------- Gather normalized vector ---------- */
    double *T_norm = NULL;
    if (pid == root)
        T_norm = malloc(N * sizeof(double));

    MPI_Gatherv(sub_norm, n_loc, MPI_DOUBLE,
                T_norm, counts, displs, MPI_DOUBLE,
                root, MPI_COMM_WORLD);

    if (pid == root) {
        printf("\nNormalized vector:\n");
        for (int i = 0; i < N; i++)
            printf("%.4f ", T_norm[i]);
        printf("\n");
    }

    /* ---------- Cleanup ---------- */
    free(sub_buf);
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
