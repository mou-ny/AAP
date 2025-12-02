#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <limits.h>
#include <time.h>

int main() {
    printf("========== EXERCISE 3 ==========\n");
    
    const int N = 50000000; // 50 million elements
    int *vector = (int*)malloc(N * sizeof(int));
    
    // Initialize vector with random values
    printf("Initializing vector with %d elements...\n", N);
    srand(time(NULL));
    for (int i = 0; i < N; i++) {
        vector[i] = rand() % 10000;
    }
    
    // Place known min and max for verification
    vector[N/2] = -100;     // Known minimum
    vector[N/4] = 99999;    // Known maximum
    
    printf("Expected: Min = -100, Max = 99999\n\n");
    
    // ========================================================================
    // Sequential Version (for comparison)
    // ========================================================================
    printf("--- Sequential Version ---\n");
    double start_seq = omp_get_wtime();
    
    int min_seq = INT_MAX;
    int max_seq = INT_MIN;
    
    for (int i = 0; i < N; i++) {
        if (vector[i] < min_seq) min_seq = vector[i];
        if (vector[i] > max_seq) max_seq = vector[i];
    }
    
    double end_seq = omp_get_wtime();
    double time_seq = end_seq - start_seq;
    
    printf("Min: %d | Max: %d | Time: %.6f seconds\n", 
           min_seq, max_seq, time_seq);
    
    // ========================================================================
    // Parallel Version with Reduction
    // ========================================================================
    printf("\n--- Parallel Version with Reduction ---\n");
    printf("%-10s %-10s %-10s %-20s %-10s\n", 
           "Threads", "Min", "Max", "Time (seconds)", "Speedup");
    printf("----------------------------------------------------------------\n");
    
    for (int num_threads = 1; num_threads <= 8; num_threads *= 2) {
        int min_val = INT_MAX;
        int max_val = INT_MIN;
        
        double start = omp_get_wtime();
        
        omp_set_num_threads(num_threads);
        #pragma omp parallel for reduction(min:min_val) reduction(max:max_val)
        for (int i = 0; i < N; i++) {
            if (vector[i] < min_val) min_val = vector[i];
            if (vector[i] > max_val) max_val = vector[i];
        }
        
        double end = omp_get_wtime();
        double time_parallel = end - start;
        double speedup = time_seq / time_parallel;
        
        printf("%-10d %-10d %-10d %-20.6f %-10.2fx\n", 
               num_threads, min_val, max_val, time_parallel, speedup);
    }
    
    printf("\n** Analysis **\n");
    printf("- Reduction clauses work for min/max operations\n");
    printf("- Speedup increases with more threads (up to a point)\n");
    printf("- Overhead becomes significant with too many threads\n");
    
    free(vector);
    
    printf("\n========== END OF EXERCISE 3 ==========\n");
    
    return 0;
}