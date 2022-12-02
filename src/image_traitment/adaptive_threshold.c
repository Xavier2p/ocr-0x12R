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

#define ADAPTIVETHRESHOLDING_RANGE 40
#define ADAPTIVETHRESHOLING_C 10

// Compute adaptive threshold on the image
void adaptative_treshold(Image *image)
{
    int w = image->width;
    int h = image->height;
    Pixel **pixels = image->pixels;

    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
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
            if ((int)pixels[i][j].r < mean - ADAPTIVETHRESHOLING_C)
            {
                pixels[i][j].r = 0;
                pixels[i][j].g = 0;
                pixels[i][j].b = 0;
            }
            else
            {
                pixels[i][j].r = 255;
                pixels[i][j].g = 255;
                pixels[i][j].b = 255;
            }
        }
    }
}

// int compute_threshold(Image *image, int x, int y, int range)
//{
//     int w = image->width;
//     int h = image->height;
//     Pixel **pixels = image->pixels;
//
//     int res = 0;
//     int c = ADAPTIVETHRESHOLING_C;
//     int nb_pixels = 0;
//     double sum = 0;
//
//     for (int dy = -range; dy < range; ++dy)
//     {
//         if (y + dy >= 0 && y + dy < h)
//         {
//             for (int dx = -range; dx < range; ++dx)
//             {
//                 if (x + dx >= 0 && x + dx < w)
//                 {
//                     sum += pixels[y + dy][x + dx].r;
//                     nb_pixels++;
//                 }
//             }
//         }
//     }
//
//     if (nb_pixels > 0)
//         res = sum / nb_pixels;
//
//     res -= c;
//
//     return res;
// }
//
// void adaptive_threshold(Image *image)
//{
//     int w = image->width;
//     int h = image->height;
//     Pixel **pixels = image->pixels;
//
//     int range = ADAPTIVETHRESHOLDING_RANGE;
//     Image c_image = copy_image(image);
//
//     for (int x = 0; x < w; ++x)
//     {
//         for (int y = 0; y < h; ++y)
//         {
//             int thresh = compute_threshold(&c_image, x, y, range);
//             if (pixels[y][x].r > (unsigned int)thresh)
//                 set_all_pixel(image, y, x, 255);
//             else
//                 set_all_pixel(image, y, x, 0);
//         }
//     }
//
//     free_image(&c_image);
// }
