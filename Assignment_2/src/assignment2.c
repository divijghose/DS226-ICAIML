#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#include <vectHandling.h>
#include <vectOps.h>

int main()
{
    double *vector_a, *vector_b, *matx;
    char *vect1filename = (char *)malloc(256);
    char  *vect2filename = (char *)malloc(256);
    char  *matxfilename = (char *)malloc(256);
    long vectlen, matxrow, matxcol = 0;
    readParams("param.txt", vect1filename, vect2filename, &vectlen, matxfilename, &matxrow, &matxcol);

    vector_a = make1dvect(vectlen);
    vector_b = make1dvect(vectlen);

    read1dvect(vector_a, vectlen, vect1filename);
    read1dvect(vector_b, vectlen, vect2filename);

    l2NormErr(vector_a, vector_b, vectlen);

    matx = make1dvect(matxrow*matxcol);
    read1dvect(matx,matxrow*matxcol,matxfilename);
    MatVectMult(matx, vector_a, matxrow, matxcol, "matvectmult.txt");

    free1dvect(vector_a);
    free1dvect(vector_b);

    return 0;
}