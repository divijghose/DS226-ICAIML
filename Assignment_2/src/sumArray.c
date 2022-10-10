#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <vectOps.h>

/**
 * @brief Routine to calculate sum of elements of a vector and 
 *          print it to file
 * @param vect Pointer to vector
 * @param vect_len Length of vector
 * @param outfile Name of output file
 */
void sumVect(double *vect, long vect_len, char *outfile)
{
    double sum_vect = 0;
    double t_start, t_end;
    t_start = omp_get_wtime();
#pragma omp parallel for default(shared) reduction(+ \
                                                   : sum_vect)
    for (int i = 0; i <= vect_len; i++)
        sum_vect += vect[i]; // Sum up the array

    t_end = omp_get_wtime();

    printf("Sum of array elements = %lf. Time required to execute = %f seconds\n", sum_vect, t_end - t_start);
}

