#include <stdio.h>
#include <omp.h>

int main() {
    int N = 10;
    double A[N];
    double sum = 0.0;
    
    for (int i = 0; i < N; i++) {
        A[i] = rand() % 100;
        printf("%.2f ", A[i]);
    }
    
   #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < N; i++) {
        sum += A[i];
    }
    
    printf("Sum = %.2f\n", sum);
    
    return 0;
}