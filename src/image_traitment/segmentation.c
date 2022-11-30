/*
 * =====================================================================================
 *
 *       Filename:  segmentation.c
 *
 *    Description:  Cut the sudoku image to sent it to the neural network
 *
 *        Version:  1.0
 *        Created:  29/11/2022 08:05:45 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Paolo Wattebled
 *   Organization:  Ox12R
 *
 * =====================================================================================
 */
#include "include/segmentation.h"

double *create_square_image(Image *image, int i, int j, int size)
{
    int w = image->width;
    int h = image->height;
    Image tmp = { .height = size, .width = size, .pixels = NULL, .path = NULL };

    tmp.pixels = (Pixel **)calloc(size, sizeof(Pixel *));
    for (int x = 0; x < size; ++x)
        tmp.pixels[x] = (Pixel *)calloc(size, sizeof(Pixel));

    for (int y = 0; y < size; ++y)
        for (int x = 0; x < size; ++x)
            set_all_pixel(&tmp, y, x, 0);

    for (int i2 = i; i2 + i < h && i2 < i + size; ++i2)
    {
        for (int j2 = j; j2 + j < w && j2 < j + size; ++j2)
        {
            Pixel tmp_pixel = {
                .r = image->pixels[i + i2][j + j2].r,
                .g = image->pixels[i + i2][j + j2].g,
                .b = image->pixels[i + i2][j + j2].b,
            };
            tmp.pixels[i2 - i][j2 - j] = tmp_pixel;
        }
    }

    Image tmp_resized = resize_image(&tmp, SIZE_OF_NEURAL_INPUT);
    double *res =
        calloc(SIZE_OF_NEURAL_INPUT * SIZE_OF_NEURAL_INPUT, sizeof(double));

    for (int y = 0; y < SIZE_OF_NEURAL_INPUT; ++y)
        for (int x = 0; x < SIZE_OF_NEURAL_INPUT; ++x)
            res[x * size + y] = tmp_resized.pixels[y][x].r == 255 ? 1.0 : 0.0;

    free_image(&tmp);
    free_image(&tmp_resized);

    return res;
}

void print_array(double *arr, int size)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            printf("%d", (int)arr[j * size + i]);
        }
        printf("\n");
    }
    printf("\n");
    printf("\n");
}

void segmentation(Image *image)
{
    int w = image->width;
    int h = image->height;
    int bloc_size = image->width / 9;

    for (int i = 0; i < h; i += bloc_size)
    {
        for (int j = 0; j < w; j += bloc_size)
        {
            double *tmp = create_square_image(image, i, j, bloc_size);
            print_array(tmp, SIZE_OF_NEURAL_INPUT);
            free(tmp);
        }
    }
}
