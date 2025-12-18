#include <stdio.h>
#include <omp.h>
#include <math.h>

int main() {
    int n = 10000000;
    double sum = 0.0;
    double step = 1.0 / n;
    
    printf("Sequential Version\n");
    double start = omp_get_wtime();
    
    for (int i = 0; i < n; i++) {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }
    
    double pi = sum * step;
    double end = omp_get_wtime();
    
    printf("Pi = %.10f\n", pi);
    printf("Time: %.6f seconds\n\n", end - start);
    
    
    printf("Parallel Version\n");
    sum = 0.0;
    start = omp_get_wtime();
    
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < n; i++) {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }
    
    pi = sum * step;
    end = omp_get_wtime();
    
    printf("Pi = %.10f\n", pi);
    printf("Time: %.6f seconds\n\n", end - start);

    

    printf("Parallel Version using critical section\n");
    sum = 0.0;
    start = omp_get_wtime();
    
    #pragma omp parallel 
    {
        double loc_sum = 0.0;
        #pragma omp for 
        for (int i = 0; i < n; i++) {
            double x = (i + 0.5) * step;
            loc_sum += 4.0 / (1.0 + x * x);
        }
        #pragma omp critical
        {
            sum += loc_sum;
        }
    }
    
    pi = sum * step;
    end = omp_get_wtime();
    
    printf("Pi = %.10f\n", pi);
    printf("Time: %.6f seconds\n", end - start);

    return 0;
}