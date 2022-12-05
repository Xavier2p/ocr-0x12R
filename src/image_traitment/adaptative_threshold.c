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

#define ADAPTIVETHRESHOLDING_RANGE 7
#define ADAPTIVETHRESHOLING_C 4
#define DILATAION_RANGE 2

// Compute adaptive threshold on the image
void adaptative_threshold(Image *image)
{
    int w = image->width;
    int h = image->height;
    Image c_image = copy_image(image);
    Pixel **pixels = c_image.pixels;

    for (int i = 0; i < h; ++i)
    {
        for (int j = 0; j < w; ++j)
        {
            int sum = 0;
            int count = 0;
            for (int k = i - ADAPTIVETHRESHOLDING_RANGE;
                 k < i + ADAPTIVETHRESHOLDING_RANGE; k++)
            {
                for (int l = j - ADAPTIVETHRESHOLDING_RANGE;
                     l < j + ADAPTIVETHRESHOLDING_RANGE; l++)
                {
                    if (k >= 0 && k < h && l >= 0 && l < w)
                    {
                        sum += pixels[k][l].r;
                        count++;
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

            set_all_pixel(image, i, j,
                          (int)image->pixels[i][j].r > mean ? 0 : 255);
        }
    }
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
                        && pixels[k][l].r == 255)
                    {
                        set_all_pixel(image, i, j, 255);
                        count = 1;
                    }
                }
            }
        }
    }
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
            for (int k = i - DILATAION_RANGE;
                 k < i + DILATAION_RANGE && count != 0; k++)
            {
                for (int l = j - DILATAION_RANGE;
                     l < j + DILATAION_RANGE && count != 0; l++)
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
}
