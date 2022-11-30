/*
 * =====================================================================================
 *
 *       Filename:  utilis_matrix.c
 *
 *    Description:  Operations possbile to apply on a matrix. For example, you
 *    can inverse a matrix with the method of the determinant
 *
 *        Version:  1.0
 *        Created:  11/09/2022 08:30:24 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Paolo Wattebled
 *   Organization:  Ox12R
 *
 * =====================================================================================
 */
#include <stdlib.h>

void mat_mult(double *matA, double *matB, int heightA, int widthA, int widthB,
              double *result)
{
    for (int iHeightA = 0; iHeightA < heightA; iHeightA++)
    {
        for (int iWidthB = 0; iWidthB < widthB; iWidthB++)
        {
            double sum = 0;
            for (int iWidthA = 0; iWidthA < widthA; iWidthA++)
            {
                sum = sum
                    + matA[iHeightA * widthA + iWidthA]
                        * matB[iWidthA * widthB + iWidthB];
            }
            result[iHeightA * widthB + iWidthB] = sum;
        }
    }
}

void mat_sub(double *matA, double *matB, double *matC, int height, int width)
{
    for (int iHeight = 0; iHeight < height; ++iHeight)
    {
        for (int iWidth = 0; iWidth < width; ++iWidth)
        {
            matC[iHeight * width + iWidth] =
                matA[iHeight * width + iWidth] - matB[iHeight * width + iWidth];
        }
    }
}

void mat_add(double *matA, double *matB, int height, int width)
{
    for (int iHeight = 0; iHeight < height; ++iHeight)
    {
        for (int iWidth = 0; iWidth < width; ++iWidth)
        {
            matA[iHeight * width + iWidth] =
                matA[iHeight * width + iWidth] + matB[iHeight * width + iWidth];
        }
    }
}

void mat_transpose(double *mat, double *result, int height, int width)
{
    for (int iHeight = 0; iHeight < height * width; iHeight++)
    {
        int i = iHeight / height;
        int j = iHeight % height;
        result[iHeight] = mat[width * j + i];
    }
}

void hadamard_product(double *matA, double *matB, int height, int width)
{
    for (int iHeight = 0; iHeight < height; iHeight++)
    {
        for (int iWidth = 0; iWidth < width; iWidth++)
        {
            matA[iHeight * width + iWidth] =
                matA[iHeight * width + iWidth] * matB[iHeight * width + iWidth];
        }
    }
}

void get_cofactor(double *A, double *temp, int p, int q, int n, int cols)
{
    int i = 0, j = 0;

    // Looping for each element of the matrix
    for (int row = 0; row < n; row++)
    {
        for (int col = 0; col < n; col++)
        {
            //  Copying into temporary matrix only those element
            //  which are not in given row and column
            if (row != p && col != q)
            {
                temp[i * cols + j++] = A[row * cols + col];
                // Row is filled, so increase row index and
                // reset col index
                if (j == n - 1)
                {
                    j = 0;
                    i++;
                }
            }
        }
    }
}

double determinant(double *A, int n, int cols)
{
    double D = 0; // Initialize result

    //  Base case : if matrix contains single element
    if (n == 1)
        return A[0];

    double *temp =
        (double *)calloc(cols * cols, sizeof(double)); // To store cofactors

    double sign = 1; // To store sign multiplier

    // Iterate for each element of first row
    for (int f = 0; f < n; f++)
    {
        // Getting Cofactor of A[0][f]
        get_cofactor(A, temp, 0, f, n, cols);
        D += sign * A[f] * determinant(temp, n - 1, cols);

        // terms are to be added with alternate sign
        sign = -sign;
    }
    free(temp);
    return D;
}

void adjoint(double *A, double *adj, int N)
{
    if (N == 1)
    {
        adj[0] = 1;
        return;
    }

    // temp is used to store cofactors of A[][]
    double sign = 1;
    double *temp = (double *)calloc(N * N, sizeof(double));

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            // Get cofactor of A[i][j]
            get_cofactor(A, temp, i, j, N, N);

            // sign of adj[j][i] positive if sum of row
            // and column indexes is even.
            sign = ((i + j) % 2 == 0) ? 1 : -1;

            // Interchanging rows and columns to get the
            // transpose of the cofactor matrix
            adj[j * N + i] = (sign) * (determinant(temp, N - 1, N));
        }
    }
    free(temp);
}

int inverse(double *A, double *inverse, int N)
{
    // Find determinant of A[][]
    double det = determinant(A, N, N);
    if (det == 0)
    {
        return 0;
    }

    // Find adjoint
    double *adj = (double *)calloc(N * N, sizeof(double));
    adjoint(A, adj, N);

    // Find Inverse using formula "inverse(A) = adj(A)/det(A)"
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            inverse[i * N + j] = adj[i * N + j] / det;
    free(adj);
    return !0;
}
