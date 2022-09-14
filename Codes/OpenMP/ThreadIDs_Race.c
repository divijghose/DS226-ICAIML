#include <omp.h> // OpenMP Header file
#include <stdio.h>

int main()
{
    int j;
    int t_num, t_id; //Declaring t_num and t_id leads to a race condition
#pragma omp parallel // Specify the block to be executed in parallel
    {
        t_num = omp_get_num_threads(); // Get total number of OMP threads
        t_id = omp_get_thread_num();   // Get ID of thread being executed
        // for(j=0;j<10000000;j++); // Introduce time delay to make code fail
        printf("Hello World from thread %d of %d\n", t_id, t_num);
    }

    return 0;
}