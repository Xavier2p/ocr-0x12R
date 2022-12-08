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

int find_nbr_blob(Image *image)
{
    int nbr_blob = 0;
    Image c_image = copy_image(image);

    for (unsigned int i = 0; i < image->height; i++)
    {
        for (unsigned int j = 0; j < image->width; j++)
        {
            Dot start_dot = { .X = j, .Y = i };
            if (image->pixels[i][j].r == 255)
            {
                nbr_blob++;
                blob_detection(&c_image, start_dot, 255, 88);
            }
        }
    }

    free_image(&c_image);
    return nbr_blob;
}

void clean_image(Image *image)
{
    for (unsigned int i = 0; i < image->height; i++)
    {
        for (unsigned int j = 0; j < image->width; j++)
        {
            if (image->pixels[i][j].r == 88)
            {
                set_all_pixel(image, i, j, 0);
            }
        }
    }

    if (find_nbr_blob(image) >= 2)
    {
        Dot biggest_dot = find_biggest_blob(image);
        blob_detection(image, biggest_dot, 255, 88);
    }

    for (unsigned int i = 0; i < image->height; i++)
    {
        for (unsigned int j = 0; j < image->width; j++)
        {
            // Biggest blob is gray so put it in white
            if (image->pixels[i][j].r == 88)
            {
                set_all_pixel(image, i, j, 255);
            }
            // Other blob to earase
            else if (image->pixels[i][j].r == 255)
            {
                set_all_pixel(image, i, j, 0);
            }
        }
    }

    // Remove the top pixels touching the border
    for (unsigned int i = 0; i < 2; ++i)
    {
        for (unsigned int j = 0; j < image->width; ++j)
        {
            if (image->pixels[i][j].r == 255)
            {
                Dot start = { .X = j, .Y = i };
                blob_detection(image, start, 255, 0);
            }
        }
    }

    // Remove the bottom pixels touching the border
    for (unsigned int i = image->height - 3; i < image->height; ++i)
    {
        for (unsigned int j = 0; j < image->width; ++j)
        {
            if (image->pixels[i][j].r == 255)
            {
                Dot start = { .X = j, .Y = i };
                blob_detection(image, start, 255, 0);
            }
        }
    }

    // Remove the left pixels touching the border
    for (unsigned int i = 0; i < image->height; ++i)
    {
        for (unsigned int j = 0; j < 2; ++j)
        {
            if (image->pixels[i][j].r == 255)
            {
                Dot start = { .X = j, .Y = i };
                blob_detection(image, start, 255, 0);
            }
        }
    }

    // Remove the right pixels touching the border
    for (unsigned int i = 0; i < image->height; ++i)
    {
        for (unsigned int j = image->width - 3; j < image->width; ++j)
        {
            if (image->pixels[i][j].r == 255)
            {
                Dot start = { .X = j, .Y = i };
                blob_detection(image, start, 255, 0);
            }
        }
    }
}

double *create_square_image(Image *image, int i, int j, int size, int cordi,
                            int cordj)
{
    int w = image->width;
    int h = image->height;
    Image tmp = { .height = size, .width = size, .pixels = NULL, .path = NULL };

    tmp.path = calloc(8, sizeof(char));
    char ic = cordi + '0';
    char jc = cordj + '0';
    tmp.path[0] = ic;
    tmp.path[1] = jc;
    tmp.path[2] = '.';
    tmp.path[3] = 'j';
    tmp.path[4] = 'p';
    tmp.path[5] = 'e';
    tmp.path[6] = 'g';
    tmp.path[7] = '\0';

    tmp.pixels = (Pixel **)calloc(size, sizeof(Pixel *));
    for (int x = 0; x < size; ++x)
        tmp.pixels[x] = (Pixel *)calloc(size, sizeof(Pixel));

    for (int y = 0; y < size; ++y)
        for (int x = 0; x < size; ++x)
            set_all_pixel(&tmp, y, x, 0);

    for (int i2 = 0; i2 + i < h && i2 < size; ++i2)
    {
        for (int j2 = 0; j2 + j < w && j2 < size; ++j2)
        {
            Pixel tmp_pixel = {
                .r = image->pixels[i + i2][j + j2].r,
                .g = image->pixels[i + i2][j + j2].g,
                .b = image->pixels[i + i2][j + j2].b,
            };
            tmp.pixels[i2][j2] = tmp_pixel;
        }
    }

    resize_image(&tmp, SIZE_OF_NEURAL_INPUT);

    clean_image(&tmp);

    double *res =
        calloc(SIZE_OF_NEURAL_INPUT * SIZE_OF_NEURAL_INPUT, sizeof(double));

    for (int y = 0; y < SIZE_OF_NEURAL_INPUT; ++y)
        for (int x = 0; x < SIZE_OF_NEURAL_INPUT; ++x)
            res[y * SIZE_OF_NEURAL_INPUT + x] =
                tmp.pixels[y][x].r == 255 ? 1.0 : 0.0;

    free_image(&tmp);

    return res;
}

int **segmentation(Image *image, Network *n)
{
    int w = image->width;
    int h = image->height;
    int bloc_size = image->width / 9;

    int cordi = 0;
    int cordj = 0;

    int **grid = (int **)calloc(10, sizeof(int *));
    for (size_t i = 0; i < 10; ++i)
        grid[i] = (int *)calloc(10, sizeof(int));

    for (int i = 0; i <= h; i += bloc_size)
    {
        cordj = 0;
        for (int j = 0; j <= w; j += bloc_size)
        {
            double *tmp =
                create_square_image(image, i, j, bloc_size, cordi, cordj);

            int res;
            if (is_empty(tmp))
                res = 0;
            else
            {
                front_propagation(n, tmp, 0);
                res = fetch_result(n);
            }

            grid[cordi][cordj] = res;

            free(tmp);
            cordj++;
        }
        cordi++;
    }

    return grid;
}
