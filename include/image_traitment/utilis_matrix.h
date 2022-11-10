#ifndef UTILIS_MATRIX_H
#define UTILIS_MATRIX_H

void mat_mult(double *matA, double *matB, int heightA, int widthA, int widthB,
              double *result);
void mat_sub(double *matA, double *matB, double *matC, int height, int width);
void mat_add(double *matA, double *matB, int height, int width);
int inverse(double *matrix, double *inverse, int cols);
void mat_transpose(double *mat, double *result, int height, int width);
void hadamard_product(double *matA, double *matB, int height, int width);

#endif
