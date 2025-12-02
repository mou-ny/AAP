#include <stdio.h>
#include <omp.h>

int main() {
    printf("========== EXERCISE 1 ==========\n");
    printf("Maximum threads available: %d\n\n", omp_get_max_threads());
    
    // Method 1: Using omp_set_num_threads()
    printf("--- Method 1: Using omp_set_num_threads(4) ---\n");
    omp_set_num_threads(4);
    
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int total_threads = omp_get_num_threads();
        
        printf("Thread %d of %d threads: Hello from parallel region!\n", 
               thread_id, total_threads);
        
        // Master thread displays "Hello World"
        #pragma omp master
        {
            printf("\n*** Master Thread %d says: Hello World! ***\n\n", 
                   thread_id);
        }
    }
    
    // Method 2: Using num_threads clause
    printf("\n--- Method 2: Using num_threads(6) clause ---\n");
    #pragma omp parallel num_threads(6)
    {
        int thread_id = omp_get_thread_num();
        int total_threads = omp_get_num_threads();
        
        printf("Thread %d of %d threads: Hello from parallel region!\n", 
               thread_id, total_threads);
        
        #pragma omp master
        {
            printf("\n*** Master Thread %d says: Hello World! ***\n\n", 
                   thread_id);
        }
    }
    
    // Method 3: Using environment variable
    printf("\n--- Method 3: Using OMP_NUM_THREADS environment variable ---\n");
    printf("(Set with: export OMP_NUM_THREADS=8 before running)\n");
    
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        int total_threads = omp_get_num_threads();
        
        // Using critical to avoid mixed output
        #pragma omp critical
        {
            printf("Thread %d of %d threads: Hello!\n", 
                   thread_id, total_threads);
        }
        
        #pragma omp master
        {
            printf("\n*** Master Thread says: Hello World! ***\n");
        }
    }
    
    printf("\n========== END OF EXERCISE 1 ==========\n");
    
    return 0;
}