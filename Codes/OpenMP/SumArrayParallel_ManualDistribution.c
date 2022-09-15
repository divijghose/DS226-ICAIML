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
#pragma omp parallel default(shared) private(i, t_id)
    {
        int arr_start, arr_end;
        t_id = omp_get_thread_num();
        t_num = omp_get_num_threads();
        // Define the start and end points for the sub-array
        arr_start = t_id * (ARRAY_SIZE / t_num);
        arr_end = (t_id + 1)*(ARRAY_SIZE / t_num) - 1;
        if (t_id == t_num) // for the last subarray
            arr_end = ARRAY_SIZE - 1;
        // printf("Thread %d of %d starts at %d and ends at %d\n",t_id,t_num,arr_start,arr_end);

        for (i = arr_start; i <= arr_end; i++) // Sum up the array
            sum_arr += arr[i];
    }
    t_end = omp_get_wtime();

    printf("Sum of array elements = %d. Time required to execute = %f seconds\n", sum_arr, t_end - t_start);

    return 0;
}