#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

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



         int sum = 0;
    for (int i = 0; i < n_loc; i++) {

        sum = sum + (sub_buffer[i]*sub_buffer[i]);
    }
        printf("Process %d sum = %d\n", pid, sum);


    int total_sum = 0;
    int i;
    int T_norm[i];

    MPI_Allreduce(&sum, &total_sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  
    N=sqrt(total_sum);

    for (int i = 0; i < n_loc; i++) {

        T_norm[i] = sub_buffer[i]/N;
    }



    MPI_Gatherv(sub_buffer, counts, displs, MPI_INT,
                 T_norm, i, MPI_INT, root,
                 MPI_COMM_WORLD);


    if (pid == 0) {
        printf("\n ecart_type = %d\n",);
    }

    MPI_Finalize();
    return 0;
}