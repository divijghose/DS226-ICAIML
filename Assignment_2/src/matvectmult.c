#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <vectHandling.h>
#include <vectOps.h>

void MatVectMult(double *matrix, double *vect, long height, long width, char *filename)
{
    double *output = make1dvect(height);
    /*
     *
     * Your Code Here
     *
     */

    print1dvect(output, height, filename);
}