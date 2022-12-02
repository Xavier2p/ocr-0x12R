/*
 * =====================================================================================
 *
 *    Filename:  gaussian_filter.c *
 *
 *    Description: Perform gaussian filter on an image
 *
 *        Version:  1.0
 *        Created:  09/28/2022 07:40:29 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Paolo.Wattebled
 *   Organization: Fach'OCR
 *
 * =====================================================================================
 */
#include "include/gaussian_filter.h"

// float *gaussian_kernel(int sigma, int n)
//{
//     float w_ideal = sqrt((12 * sigma * sigma / n) + 1);
//     int wl = floor(w_ideal);
//     if (wl % 2 == 0)
//         wl--;
//     int wu = wl + 2;
//
//     float m_ideal = (12 * sigma * sigma - n * wl * wl - 4 * n * wl - 3 * n) /
//     (-4 * wl - 4); int m = round(m_ideal);
//
//     float *kernel = malloc(n * sizeof(float));
//
//     for (int i = 0; i < n; i++)
//         kernel[i] = (i < m ? wl : wu);
//
//     return kernel;
// }
//
//
// void box_blur_T_4(Image *image_src, Image *image_dst, int w, int h, int r)
//{
//     Pixel **src = image_src->pixels;
//     Pixel **dst = image_dst->pixels;
//
//     int iarr = 1 / (r + r + 1);
//     for (int i = 0; i < w; i++)
//     {
//         int ti = i, li = ti, ri = ti + r * w;
//         int fv = src[ti][i].r, lv = src[ti + w * (h - 1)][i].r, val = (r + 1)
//         * fv; for (int j = 0; j < r; j++)
//             val += src[ti + j * w][i].r;
//         for (int j = 0; j <= r; j++)
//         {
//             val += src[ri][i].r - fv;
//             set_all_pixel(image_dst, ti, i, round(val * iarr));
//             ri += w;
//             ti += w;
//         }
//         for (int j = r + 1; j < w - r; j++)
//         {
//             val += src[ri][i].r - dst[li][i].r;
//             set_all_pixel(image_dst, ti, i, round(val * iarr));
//             li += w;
//             ri += w;
//             ti += w;
//         }
//         for (int j = w - r; j < w; j++)
//         {
//             val += lv - src[li][i].r;
//             set_all_pixel(image_dst, ti, i, round(val * iarr));
//             li += w;
//             ti += w;
//         }
//     }
// }
//
//
//
// void box_blur_H_4(Image *image_src, Image *image_dst, int w, int h, int r)
//{
//     Pixel **src = image_src->pixels;
//
//     float iarr = 1 / (r + r + 1);
//     for (int i = 0; i < h; ++i)
//     {
//         int ti = i * w, li = ti, ri = ti + r;
//         int fv = src[i][ti].r, lv = src[i][ti + w - 1].r, val = (r + 1) * fv;
//         for (int j = 0; j < r; ++j)
//             val += src[i][ti + j].r;
//         for (int j = 0; j <= r; ++j)
//         {
//             val += src[i][ri++].r - fv;
//             set_all_pixel(image_dst, i, ti++, round(val * iarr));
//         }
//         for (int j = r + 1; j < w - r; ++j)
//         {
//             val += src[i][ri++].r - src[i][li++].r;
//             set_all_pixel(image_dst, i, ti++, round(val * iarr));
//         }
//         for (int j = w - r; j < w; ++j)
//         {
//             val += lv - src[i][li++].r;
//             set_all_pixel(image_dst, i, ti++, round(val * iarr));
//         }
//     }
// }
//
//
// void box_blur_4(Image *src, Image *dst, int w, int h, int r)
//{
//     for (int i = 0; i < h; ++i)
//         for (int j = 0; j < w; ++j)
//             dst->pixels[i][j] = src->pixels[i][j];
//
//     box_blur_H_4(src, dst, w, h, r);
//     box_blur_T_4(src, dst, w, h, r);
// }
//
//
// void gaussian_blur_4(Image *src, Image *dst, int w, int h, int r)
//{
//     float *kernel = gaussian_kernel(r, 3);
//     box_blur_4(src, dst, w, h, (kernel[0] - 1) / 2);
//     box_blur_4(src, dst, w, h, (kernel[1] - 1) / 2);
//     box_blur_4(src, dst, w, h, (kernel[2] - 1) / 2);
// }
//
//
// void gaussian_blur(Image *image, int radius)
//{
//     Image dst = copy_image(image);
//     for (int i = 0; i < (int)image->height; ++i)
//         for (int j = 0; j < (int)image->width; ++j)
//             set_all_pixel(&dst, i, j, 0);
//     gaussian_blur_4(image, &dst, image->width, image->height, radius);
//
//     for (int i = 0; i < (int)image->height; ++i)
//         for (int j = 0; j < (int)image->width; ++j)
//             set_all_pixel(image, i, j, dst.pixels[i][j].r);
// }

double **build_gaussian_kernel(int radius)
{
    double sigma = radius / 2.0 > 1.0 ? radius / 2.0 : 1.0;
    int kwidht = (2 * round(radius)) + 1;

    double **kernel = calloc(kwidht, sizeof(double *));
    for (int i = 0; i < kwidht; ++i)
        kernel[i] = calloc(kwidht, sizeof(double));
    double sum = 0.0;

    for (double x = -radius; x < radius; ++x)
    {
        for (double y = -radius; y < radius; ++y)
        {
            double exponum = -(x * x + y * y);
            double expodeno = 2 * sigma * sigma;
            double expression = exp(exponum / expodeno);
            double kvalue = expression / (2 * M_PI * sigma * sigma);

            kernel[(int)x + radius][(int)y + radius] = kvalue;
            sum += kvalue;
        }
    }

    for (int x = 0; x < kwidht; ++x)
        for (int y = 0; y < kwidht; ++y)
            kernel[x][y] /= sum;

    return kernel;
}

void gaussian_blur(Image *image, int radius)
{
    int kwidht = (2 * round(radius)) + 1;
    double **kernel = build_gaussian_kernel(radius);

    Image tmp_image = copy_image(image);
    for (unsigned int x = radius; x < image->height - radius; ++x)
    {
        for (unsigned int y = radius; y < image->width - radius; ++y)
        {
            double val = 0.0;

            for (int _x = -radius; _x < radius; ++_x)
            {
                for (int _y = -radius; _y < radius; ++_y)
                {
                    double kvalue = kernel[_x + radius][_y + radius];
                    val += tmp_image.pixels[x - _x][y - _y].r * kvalue;
                }
            }
            set_all_pixel(image, x, y, val);
        }
    }

    for (int i = 0; i < kwidht; ++i)
        free(kernel[i]);
    free(kernel);

    free_image(&tmp_image);
}
