#include <stdio.h>
#include <omp.h>

#define TOTAL 2000000

int main(int argc, char *argv[])
{
    int pass = 0, cur;

    double start = omp_get_wtime();
    
    ///////////////////// MODIFIER A PARTIR D'ICI UNIQUEMENT /////////////////////
    
    #pragma omp parallel for private(cur)// reduction(+:pass)
    for(int i = 0; i < TOTAL; i++) {
        cur = i;
        while (cur > 1)
            cur = cur % 2 ? 3 * cur + 1 : cur / 2;
        pass++;
    }
    
    ///////////////////// MODIFIER JUSQU'ICI UNIQUEMENT /////////////////////
    
    double end = omp_get_wtime();
    
    printf("%d out of %d ! (delta= %d)\n", pass, TOTAL, TOTAL - pass);
    printf("time: %lf ms\n", (end - start) * 1000);
    printf("Number of threads: %d\n", omp_get_max_threads());
    
    return 0;
}