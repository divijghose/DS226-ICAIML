#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <vectOps.h>
#include <vectHandling.h>

void l2NormErr(double *vect1, double *vect2, long n)
{

    double l2n = 0; // L2 norm error 

    /*
     *
     * Your Code Here
     *
     */

    printf("L2 Norm of the error vector: %lf\n", l2n);
    print1dvect(&l2n, 1, "l2error.txt");

    return;
}