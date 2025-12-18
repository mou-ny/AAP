/*
 * Exercise 3: Find Min and Max using Reduction Clause
 * 
 * Compilation: gcc -fopenmp exercise3.c -o exercise3
 * Execution: ./exercise3
 */

#include <stdio.h>
#include <omp.h>
#include <limits.h>

int main() {
    int N = 10;
    int vector[N];
    int min_val = INT_MAX;
    int max_val = INT_MIN;
    
    for (int i = 0; i < N; i++) {
        vector[i] = rand() % 10;
        printf("%d ", vector[i]);
    }

    ""
    #pragma omp parallel for reduction(min:min_val) reduction(max:max_val)
    for (int i = 0; i < N; i++) {
        if (vector[i] < min_val) min_val = vector[i];
        if (vector[i] > max_val) max_val = vector[i];
    }
    
    printf("Min = %d\n", min_val);
    printf("Max = %d\n", max_val);
    
    return 0;
}