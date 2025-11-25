#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int pid, nproc;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);


    int root = atoi(argv[1]);
    int N = atoi(argv[2]);
    int buffer[N];
    int counts[nproc];
    int displs[nproc];
    int n_loc = N / nproc;
    int rest = N % nproc;

    if (pid == root) {
        printf("Array: ");
        for (int i = 0; i < N; i++) buffer[i] = rand() % 100;
        for (int i = 0; i < N; i++) printf("%d ", buffer[i]);
        
        printf("\n");


        int d=0;
        for (int i = 0; i < nproc; i++) {
          if ( i < rest ) {
            counts[i] = n_loc + 1;
          }
          else { 
            counts[i] = n_loc;
          }
            displs[i] = d;
            d = d+counts[i];
        }

    }

    if(pid<rest) n_loc = n_loc + 1;
    int sub_buffer[n_loc];  
    MPI_Scatterv(buffer, counts, displs, MPI_INT,
                 sub_buffer, n_loc, MPI_INT, root,
                 MPI_COMM_WORLD);

    int local_max = sub_buffer[0];
    for (int i = 1; i < n_loc; i++) {
        if (sub_buffer[i] > local_max)
            local_max = sub_buffer[i];
    }

    printf("Process %d local max = %d\n", pid, local_max);

    int global_max = 0;
    MPI_Reduce(&local_max, &global_max, 1, MPI_INT, MPI_MAX, root, MPI_COMM_WORLD);


    if (pid == 0) {
        printf("\nGlobal maximum = %d\n", global_max);
    }

    MPI_Finalize();
    return 0;
}
