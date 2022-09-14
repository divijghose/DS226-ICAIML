#include <omp.h> // OpenMP Header file
#include <stdio.h>

int main()
{
    int t_num;
#pragma omp parallel // Specify the block to be executed in parallel
    {
        int t_id = omp_get_thread_num(); // Get ID of thread being executed
        if (t_id == 0)
            t_num = omp_get_num_threads(); // Get total number of OMP threads

#pragma omp barrier
        printf("Hello World from thread %d of %d\n", t_id, t_num);
    }
    return 0;
}