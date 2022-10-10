/*
Include files for operations on vectors
*/

void sumVect(double *vect, long vect_len, char *outfile);

double dotProduct(double *vect1, double *vect2, long vect_len);

void Daxpy(double *vect1, double *vect2, double alpha, long vect_len, char *filename);

void l2NormErr(double *vect1, double *vect2, long n);

void MatVectMult(double *matrix, double *vect, long height, long width, char *filename);
