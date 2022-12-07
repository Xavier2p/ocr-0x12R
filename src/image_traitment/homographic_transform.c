/*
 * =====================================================================================
 *
 *       Filename:  homographic_transform.c
 *
 *    Description:  Perform homographic transform resizing / rotating and
 *    correction the perspective
 *
 *        Version:  1.0
 *        Created:  11/09/2022 08:25:13 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Paolo Wattebled, Mathieu Pastre, Xavier de Place
 *   Organization:  0x12R
 *
 * =====================================================================================
 */
#include "include/homographic_transform.h"
#include "include/utilis_image.h"

double *fill_matrix(int size, Square *square)
{
    double *mat = calloc(8 * 8, sizeof(double));
    // 1st ROW
    FILL(mat, 0, 2) = 1;
    FILL(mat, 1, 5) = 1;
    // 2nd ROW
    FILL(mat, 2, 0) = size;
    FILL(mat, 2, 2) = 1;
    FILL(mat, 2, 6) = -square->tr.X * size;
    FILL(mat, 3, 3) = size;
    FILL(mat, 3, 5) = 1;
    FILL(mat, 3, 6) = -square->tr.Y * size;
    // 3rd ROW
    FILL(mat, 4, 1) = size;
    FILL(mat, 4, 2) = 1;
    FILL(mat, 4, 7) = -square->bl.X * size;
    FILL(mat, 5, 4) = size;
    FILL(mat, 5, 5) = 1;
    FILL(mat, 5, 7) = -square->bl.Y * size;
    // 4th ROW
    FILL(mat, 6, 0) = size;
    FILL(mat, 6, 1) = size;
    FILL(mat, 6, 2) = 1;
    FILL(mat, 6, 6) = -square->br.X * size;
    FILL(mat, 6, 7) = -square->br.Y * size;
    FILL(mat, 7, 3) = size;
    FILL(mat, 7, 4) = size;
    FILL(mat, 7, 5) = 1;
    FILL(mat, 7, 6) = -square->br.X * size;
    FILL(mat, 7, 7) = -square->br.Y * size;

    double *res = calloc(8, sizeof(double));
    double b[] = { (double)square->tl.X, (double)square->tl.Y,
                   (double)square->tr.X, (double)square->tr.Y,
                   (double)square->bl.X, (double)square->bl.Y,
                   (double)square->br.X, (double)square->br.Y };

    double *mattranspose = calloc(8 * 8, sizeof(double));
    mat_transpose(mat, mattranspose, 8, 8);

    double *res_mult = calloc(8 * 8, sizeof(double));
    mat_mult(mattranspose, mat, 8, 8, 8, res_mult);

    free(mat);
    double *res_inv = calloc(8 * 8, sizeof(double));

    if (!inverse(res_mult, res_inv, 8))
        errx(EXIT_FAILURE, "Matrix is singular");

    free(res_mult);
    double *res_mult2 = calloc(8 * 8, sizeof(double));
    mat_mult(res_inv, mattranspose, 8, 8, 8, res_mult2);
    free(res_inv);
    free(mattranspose);
    mat_mult(res_mult2, b, 8, 8, 1, res);
    free(res_mult2);
    return res;
}

void homographic_transform(Image *image, Square *square, int size)
{
    double *mat = fill_matrix(size, square);

    Image new_image = {
        .height = size, .width = size, .pixels = NULL, .path = NULL
    };

    new_image.pixels = (Pixel **)calloc(size, sizeof(Pixel *));
    for (int x = 0; x < size; ++x)
    {
        new_image.pixels[x] = (Pixel *)calloc(size, sizeof(Pixel));
        if (new_image.pixels[x] == NULL)
            errx(EXIT_FAILURE,
                 "Error while allocating pixels pointers for the image");
    }
    if (image->path != NULL)
    {
        new_image.path = calloc(strlen(image->path) + 1, sizeof(char));
        strcpy(new_image.path, image->path);
    }

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            double x = (mat[0] * i + mat[1] * j + mat[2])
                / (mat[6] * i + mat[7] * j + 1);
            double y = (mat[3] * i + mat[4] * j + mat[5])
                / (mat[6] * i + mat[7] * j + 1);

            if (x >= 0 && x < image->width && y >= 0 && y < image->height)
            {
                new_image.pixels[j][i].r = image->pixels[(int)y][(int)x].r;
                new_image.pixels[j][i].g = image->pixels[(int)y][(int)x].g;
                new_image.pixels[j][i].b = image->pixels[(int)y][(int)x].b;
            }
        }
    }

    free(mat);

    free_image(image);
    *image = new_image;
}
