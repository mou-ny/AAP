#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int pid,nproc;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);
    int N = atoi(argv[1]);

    int n_loc = N/ nproc;
    int buffer[N];
    int sub_buffer[n_loc];

        if (pid == 0) {
        printf("array: ");
        for (int i = 0; i < N; i++) {
            buffer[i] = i + 1;
            printf("%d ", buffer[i]);
        }
        printf("\n");
    }
    MPI_Scatter(buffer, n_loc, MPI_INT, sub_buffer, n_loc, MPI_INT, 0, MPI_COMM_WORLD);

    int sum = 0;
    for (int i = 0; i < n_loc; i++) {
        sum += sub_buffer[i];
    }
        printf("Process %d sum = %d\n", pid, sum);


    int total_sum = 0;

    MPI_Reduce(&sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (pid == 0)
        printf("prc %d total sum = %d\n",pid, total_sum);
  


    MPI_Finalize();
    return 0;
}

