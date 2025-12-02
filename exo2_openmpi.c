#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    printf("========== EXERCISE 2 ==========\n");
    
    const int N = 100000000; // 100 million elements
    double *A = (double*)malloc(N * sizeof(double));
    
    // Initialize array
    printf("Initializing array with %d elements...\n", N);
    for (int i = 0; i < N; i++) {
        A[i] = 1.0; // Simple value for easy verification
    }
    
    printf("Expected sum: %.2f\n\n", (double)N);
    
    // ========================================================================
    // Method 1: Using shared variable with critical section
    // ========================================================================
    printf("--- Method 1: Shared Variable with Critical Section ---\n");
    printf("%-10s %-15s %-20s\n", "Threads", "Sum", "Time (seconds)");
    printf("------------------------------------------------------\n");
    
    for (int num_threads = 1; num_threads <= 8; num_threads *= 2) {
        double sum_shared = 0.0;
        double start = omp_get_wtime();
        
        omp_set_num_threads(num_threads);
        #pragma omp parallel
        {
            double local_sum = 0.0;
            
            // Each thread computes partial sum
            #pragma omp for
            for (int i = 0; i < N; i++) {
                local_sum += A[i];
            }
            
            // Critical section to add to shared variable
            #pragma omp critical
            {
                sum_shared += local_sum;
            }
        }
        
        double end = omp_get_wtime();
        printf("%-10d %-15.2f %-20.6f\n", 
               num_threads, sum_shared, end - start);
    }
    
    // ========================================================================
    // Method 2: Using reduction clause
    // ========================================================================
    printf("\n--- Method 2: Reduction Clause ---\n");
    printf("%-10s %-15s %-20s\n", "Threads", "Sum", "Time (seconds)");
    printf("------------------------------------------------------\n");
    
    for (int num_threads = 1; num_threads <= 8; num_threads *= 2) {
        double sum_reduction = 0.0;
        double start = omp_get_wtime();
        
        omp_set_num_threads(num_threads);
        #pragma omp parallel for reduction(+:sum_reduction)
        for (int i = 0; i < N; i++) {
            sum_reduction += A[i];
        }
        
        double end = omp_get_wtime();
        printf("%-10d %-15.2f %-20.6f\n", 
               num_threads, sum_reduction, end - start);
    }
    
    printf("\n** Analysis **\n");
    printf("The reduction clause is typically faster because:\n");
    printf("- No critical section overhead\n");
    printf("- Compiler optimizations\n");
    printf("- Better cache performance\n");
    printf("- Cleaner and more readable code\n");
    
    free(A);
    
    printf("\n========== END OF EXERCISE 2 ==========\n");
    
    return 0;
}