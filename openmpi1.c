#include <stdio.h>
#include <omp.h>

int main() {
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int total_threads = omp_get_num_threads();
        
        printf("Thread %d of %d threads : yeeee\n", 
               thread_id, total_threads);
        
        #pragma omp master
        {
            printf("\nMaster Thread %d says: Hello World\n\n", 
                   thread_id);
        }
    }
    return 0;
}