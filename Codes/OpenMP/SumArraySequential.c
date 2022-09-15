#include <omp.h>
#include <stdio.h>

#define ARRAY_SIZE 10000
int arr[ARRAY_SIZE];

int main()
{
    int i, sum_arr = 0;
    double t_start, t_end;

    for (i = 0; i < ARRAY_SIZE; i++) // Initialize array
        arr[i] = 1;

    t_start = omp_get_wtime();
    for (i = 0; i < ARRAY_SIZE; i++) // Sum up the array
        sum_arr += arr[i];
    t_end = omp_get_wtime();

    printf("Sum of array elements = %d. Time required to execute = %f seconds\n", sum_arr, t_end - t_start);

    return 0;
}