#include <stdlib.h>
#include <stdio.h>
#include <vectHandling.h>
#include <string.h>
/**
 * @brief Allocate memory for a 1D Vector
 *
 * @param n Length of vector
 * @return double* Pointer to vector
 */
double *make1dvect(long n)
{
    double *vect = NULL;
    vect = (double *)malloc(n * sizeof(double));
    return vect;
}

/**
 * @brief Print vector to a text file
 *
 * @param vect Pointer to vector
 * @param n Length of vector
 * @param filename Name of text file
 */
void print1dvect(double *vect, long n, char *filename)
{
    FILE *out_file = fopen(filename, "w");
    if (out_file == NULL)
    {
        printf("Error, could not open output file\n");
        exit(0);
    }
    for (long i = 0; i < n; i++)
    {
        fprintf(out_file, "%lf\n", vect[i]);
    }
    printf("Vector has succesfully been written to file: %s\n", filename);
}

void read1dvect(double *vect, long n, char *filename)
{
    FILE *in_file = fopen(filename, "r");
    if (in_file == NULL)
    {
        printf("Error, could not open output file: %s\n", filename);
        exit(0);
    }
    for (long i = 0; i < n; i++)
    {
        fscanf(in_file, "%lf", &vect[i]);
    }
    printf("Vector has succesfully been read from the file: %s\n", filename);
}

/**
 * @brief Free the memory allocated to vector
 *
 * @param vect Pointer to vector
 */
void free1dvect(double *vect)
{
    free(vect);
}

void readParams(char *paramFilename, char *vect1Filename, char *vect2Filename, long *vectLen, char *matxFilename, long *height, long *width)
{
    char line[256];
    FILE *param_file = fopen(paramFilename, "r");
    if (param_file == NULL)
    {
        printf("Error, could not open output file: %s\n", paramFilename);
        exit(0);
    }

    while (fgets(line, 256, param_file))
    {
        if (!strcmp(line, "Vector_1_Source:\n"))
        {
            fgets(line, 256, param_file);

            line[strcspn(line, "\n")] = 0;

            strcpy(vect1Filename, line);
        }
        if (!strcmp(line, "Vector_2_Source:\n"))
        {
            fgets(line, 256, param_file);
            line[strcspn(line, "\n")] = 0;
            strcpy(vect2Filename, line);
        }
        if (!strcmp(line, "Matrix_Source:\n"))
        {
            fgets(line, 256, param_file);
            line[strcspn(line, "\n")] = 0;
            strcpy(matxFilename, line);
        }
        if (!strcmp(line, "Vector_Length:\n"))
        {
            fgets(line, 256, param_file);
            line[strcspn(line, "\n")] = 0;
            char *ptr;
            *vectLen = strtol(line, &ptr, 10);
        }
        if (!strcmp(line, "Matrix_Rows:\n"))
        {
            fgets(line, 256, param_file);
            line[strcspn(line, "\n")] = 0;
            char *ptr;
            *height = strtol(line, &ptr, 10);
        }
        if (!strcmp(line, "Matrix_Columns:\n"))
        {
            fgets(line, 256, param_file);
            line[strcspn(line, "\n")] = 0;
            char *ptr;
            *width = strtol(line, &ptr, 10);
        }
    }

    printf("Parameters have succesfully been read from the file: %s\n", paramFilename);
}