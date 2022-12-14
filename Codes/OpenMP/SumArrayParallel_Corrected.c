#include <omp.h>
#include <stdio.h>

#define ARRAY_SIZE 1000000000
int arr[ARRAY_SIZE];

int main()
{
    int i, t_id, t_num, sum_arr = 0;
    double t_start, t_end;

    for (i = 0; i < ARRAY_SIZE; i++) // Initialize array
        arr[i] = 1;

    t_start = omp_get_wtime();
#pragma omp parallel default(shared)
    {
        int psum = 0;
#pragma omp for
        for (i = 0; i <= ARRAY_SIZE; i++)
            psum += arr[i]; // Sum up the array

#pragma omp critical
        sum_arr += psum;
    }
    t_end = omp_get_wtime();

    printf("Sum of array elements = %d. Time required to execute = %f seconds\n", sum_arr, t_end - t_start);

    return 0;
}