#include <stdio.h>
#include <omp.h>

#define TOTAL 1000000

int main(int argc, char *argv[])
{
    int max_steps = 0;
    int max_number = 0;
    int steps, cur;

    double start = omp_get_wtime();
    
    ///////////////////// MODIFIER A PARTIR D'ICI UNIQUEMENT /////////////////////
    
    #pragma omp parallel for private(steps, cur) reduction(max:max_steps)
    for(int i = 1; i < TOTAL; i++) {
        cur = i;
        steps = 0;
        
        while (cur > 1) {
            cur = cur % 2 ? 3 * cur + 1 : cur / 2;
            steps++;
        }
        
        // Update maximum steps
        if (steps > max_steps) {
            max_steps = steps;
            #pragma omp critical
            {
                if (steps > max_steps) {
                    max_number = i;
                }
            }
        }
    }
    
    ///////////////////// MODIFIER JUSQU'ICI UNIQUEMENT /////////////////////
    
    double end = omp_get_wtime();
    
    printf("Number with maximum steps: %d\n", max_number);
    printf("Maximum steps: %d\n", max_steps);
    printf("Time: %lf ms\n", (end - start) * 1000);
    printf("Number of threads: %d\n", omp_get_max_threads());
    
    return 0;
}