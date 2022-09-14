#include <omp.h> // OpenMP Header file
#include <stdio.h>

int main()
{
    int t_num, t_id; //Declaring t_num and t_id leads to a race condition
#pragma omp parallel private(t_id) shared(t_num) // Specify the block to be executed in parallel
    {
        t_num = omp_get_num_threads(); // Get total number of OMP threads
        t_id = omp_get_thread_num();   // Get ID of thread being executed
        printf("Hello World from thread %d of %d\n", t_id, t_num);
    }

    return 0;
}