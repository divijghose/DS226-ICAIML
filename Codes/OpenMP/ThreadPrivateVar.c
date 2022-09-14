#include <omp.h> // OpenMP Header file
#include <stdio.h>
int t_id;
#pragma omp threadprivate(t_id)
int main()
{
    int t_num;
#pragma omp parallel // Specify the block to be executed in parallel
    {
        t_id = omp_get_thread_num(); // Get ID of thread being executed
        if (t_id == 0)
            t_num = omp_get_num_threads(); // Get total number of OMP threads
    }

#pragma omp parallel // Specify the block to be executed in parallel
    printf("Hello World from thread %d of %d\n", t_id, t_num);

    return 0;
}