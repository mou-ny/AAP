#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int *table = NULL;
    int total = 12;
    int N = total / size;
    float t;

    int *local = malloc(N * sizeof(int));
    float *local_norm = malloc(N * sizeof(float));
    float *table_norm = NULL;

    if (rank == 0) {
        table_norm = malloc(total * sizeof(float));
        table = malloc(total * sizeof(int));
        srand(time(NULL));
        for (int i = 0; i < total; i++) {
            table[i] = rand() % 10;
            printf("%d ", table[i]);
            if ((i+1) % N == 0) printf("\n");
        }
        printf("\n");
    }

    MPI_Scatter(table, N, MPI_INT, local, N, MPI_INT, 0, MPI_COMM_WORLD);

    int local_sum = 0;
    for (int i = 0; i < N; i++)
     {
        local_sum += local[i]*local[i];
     }
    int global_sum = 0;

    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    sleep(1 * rank);

    if (rank == 0) {
        printf("Total sum of squares = %d\n", global_sum);
        t=sqrt(global_sum);
        printf("Global sum sqrt: %f \n",t);
    }
      MPI_Bcast(&t, 1, MPI_FLOAT, 0, MPI_COMM_WORLD);
      for (int i = 0; i < N; i++)
     {
        local_norm[i]=local[i]/t;
     }
     MPI_Gather(local_norm, N, MPI_INT, table_norm, N, MPI_INT, 0, MPI_COMM_WORLD);
     if (rank==0)
     {
        for (int i = 0; i < total; i++) {

            printf("%f ", table_norm[i]);
            if ((i+1) % N == 0) printf("\n");
        }
     }

    printf("\n");
    free(local);
    if (rank == 0) 
    { 
        free(table);free(table_norm);
    }

    MPI_Finalize();
    return 0;
}