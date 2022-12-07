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
#include "include/adaptative_threshold.h"
#include "include/utilis_image.h"

#define ADAPTIVETHRESHOLDING_RANGE 8
#define ADAPTIVETHRESHOLING_C 4
#define DILATAION_RANGE 3
#define EROSION_RANGE 3

// Compute adaptive threshold on the image
int find_adaptative_threshold(Image *image, int y, int x)
{
    int w = image->width;
    int h = image->height;
    Pixel **pixels = image->pixels;

    int sum = 0;
    int count = 0;

    for (int dy = -ADAPTIVETHRESHOLDING_RANGE; dy < ADAPTIVETHRESHOLDING_RANGE;
         dy++)
    {
        if (0 <= y + dy && y + dy < h)
        {
            for (int dx = -ADAPTIVETHRESHOLDING_RANGE;
                 dx < ADAPTIVETHRESHOLDING_RANGE; dx++)
            {
                if (0 <= x + dx && x + dx < w)
                {
                    sum += pixels[y + dy][x + dx].r;
                    count++;
                }
            }
        }
    }

    int mean = sum / count;

    if (count > 0)
        mean = sum / count;

    if (mean > ADAPTIVETHRESHOLING_C)
        mean -= ADAPTIVETHRESHOLING_C;
    else
        mean = 0;

    return mean;
}

void adaptative_threshold(Image *image)
{
    Image c_image = copy_image(image);
    int w = image->width;
    int h = image->height;

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            int mean = find_adaptative_threshold(&c_image, y, x);
            set_all_pixel(image, y, x,
                          mean > (int)image->pixels[y][x].r ? 255 : 0);
        }
    }

    free_image(&c_image);
}

void dilatation(Image *image)
{
    int w = image->width;
    int h = image->height;
    Image c_image = copy_image(image);
    Pixel **pixels = c_image.pixels;

    for (int i = 0; i < h; ++i)
    {
        for (int j = 0; j < w; ++j)
        {
            int count = 0;
            for (int k = i - DILATAION_RANGE;
                 k < i + DILATAION_RANGE && count != 1; k++)
            {
                for (int l = j - DILATAION_RANGE;
                     l < j + DILATAION_RANGE && count != 1; l++)
                {
                    if (k >= 0 && k < h && l >= 0 && l < w
                        && pixels[k][l].r == 88)
                    {
                        set_all_pixel(image, i, j, 88);
                        count = 1;
                    }
                }
            }
        }
    }

    free_image(&c_image);
}

void erosion(Image *image)
{
    int w = image->width;
    int h = image->height;
    Image c_image = copy_image(image);
    Pixel **pixels = c_image.pixels;

    for (int i = 0; i < h; ++i)
    {
        for (int j = 0; j < w; ++j)
        {
            int count = 0;
            for (int k = i - EROSION_RANGE; k < i + EROSION_RANGE && count != 0;
                 k++)
            {
                for (int l = j - EROSION_RANGE;
                     l < j + EROSION_RANGE && count != 0; l++)
                {
                    if (k >= 0 && k < h && l >= 0 && l < w
                        && pixels[k][l].r == 0)
                    {
                        set_all_pixel(image, i, j, 0);
                        count++;
                    }
                }
            }
        }
    }

    free_image(&c_image);
}
