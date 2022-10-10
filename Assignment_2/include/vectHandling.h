/*
Include file for rotuines to handle vectors
*/


double *make1dvect(long n);

void print1dvect(double *vect, long n, char *filename);

void read1dvect(double *vect, long n, char *filename);

void free1dvect(double *vect);

void readParams(char *paramFilename, char *vect1Filename, char *vect2Filename, long *vectLen, char *matxFilename, long *height, long *width);


