/*
 * =====================================================================================
 *
 *       Filename:  adaptive_threshold.c
 *
 *    Description:  Compute adaptive threshold on the image
 *
 *        Version:  1.0
 *        Created:  11/26/2022 12:06:25 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Paolo Wattebled
 *   Organization:  Ox12R
 *
 * =====================================================================================
 */
#include "../../include/image_traitment/adaptive_treshold.h"

#define ADAPTIVETHRESHOLDING_RANGE 5
#define ADAPTIVETHRESHOLING_C 3

int compute_threshold(Image *image, int x, int y, int range, double **kernel)
{
    int w = image->width;
    int h = image->height;
    Pixel **pixels = image->pixels;

    int res = 0;
    int c = ADAPTIVETHRESHOLING_C;
    int nb_pixels = 0;
    double sum = 0;

    for (int dy = -range; dy < range; ++dy)
    {
        if (y + dy >= 0 && y + dy < h)
        {
            for (int dx = -range; dx < range; ++dx)
            {
                if (x + dx >= 0 && x + dx < w)
                {
                    sum += (double)pixels[y + dy][x + dx].r
                        * kernel[dy + range][dx + range];
                    nb_pixels++;
                }
            }
        }
    }

    if (nb_pixels > 0)
        res = sum / nb_pixels;

    if (res > c)
        res -= c;
    else
        res = 0;

    return res;
}

void adaptive_threshold(Image *image)
{
    int w = image->width;
    int h = image->height;
    Pixel **pixels = image->pixels;

    int range = ADAPTIVETHRESHOLDING_RANGE;
    double **kernel = build_gaussian_kernel(range);
    Image c_image = copy_image(image);

    for (int x = 0; x < w; ++x)
    {
        for (int y = 0; y < h; ++y)
        {
            int thresh = compute_threshold(&c_image, x, y, range, kernel);
            if (pixels[y][x].r > thresh)
                set_all_pixel(image, y, x, 0);
            else
                set_all_pixel(image, y, x, 255);
        }
    }

    free_image(&c_image);
    free(kernel);
}
