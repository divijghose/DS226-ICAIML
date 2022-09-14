#include <omp.h> // OpenMP Header file
#include <stdio.h>

int main()
{

#pragma omp parallel // Specify the block to be executed in parallel
    {
        printf("Hello World!\n");
    }

    return 0;
}