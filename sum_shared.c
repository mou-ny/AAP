
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char **argv)
{
    int N = 10;         /* default small array */
    int nthreads = 4;   /* default threads */

    if (argc > 1) N = atoi(argv[1]);
    if (argc > 2) nthreads = atoi(argv[2]);

    if (N <= 0) N = 10;
    if (nthreads <= 0) nthreads = 1;

    omp_set_num_threads(nthreads);

    double *A = malloc(sizeof(double) * (size_t)N);
    if (!A) return 1;

    for (int i = 0; i < N; ++i) A[i] = 1.0; /* simple init */

    double sum = 0.0;

    /* simple parallel loop updating shared `sum` atomically */
    #pragma omp parallel for shared(sum, A, N)
    for (int i = 0; i < N; ++i) {
        #pragma omp atomic
        sum += A[i];
    }

    printf("sum = %f (expected = %d)\n", sum, N);

    free(A);
    return 0;
}
