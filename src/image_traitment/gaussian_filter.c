/*
 * =====================================================================================
 *
 *    Filename:  gaussian_filter.c
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
#include <SDL2/SDL_audio.h>
#include "include/utilis_image.h"

#define GAUSSIAN_RANGE 2
#define GAUSSIAN_BLUR_SIGMA 1.3
//
// int clamp(int value, int min, int max)
//{
//    if (value < min)
//        return min;
//    if (value > max)
//        return max;
//    return value;
//}
//
// double Gaussian(double x, double sigma)
//{
//    double x2 = x * x;
//    double sigma2 = sigma * sigma;
//    return exp(-x2 / (2 * sigma2));
//}
//
// double *Build_GaussianKernel(int kernelSize)
//{
//    double sum = 0;
//    double *kernel = calloc(kernelSize, sizeof(*kernel));
//
//    for (int x = 0; x < kernelSize; ++x)
//    {
//        double value = Gaussian(x - GAUSSIAN_RANGE, GAUSSIAN_BLUR_SIGMA);
//        kernel[x] = value;
//        sum += value;
//    }
//
//    for (int x = 0; x < kernelSize; ++x)
//    {
//        kernel[x] /= sum;
//    }
//
//    return kernel;
//}
//
// void gaussian_blur(Image *image)
//{
//    int h = image->height;
//    int w = image->width;
//    Image c_image = copy_image(image);
//    Pixel **pixels = c_image.pixels;
//
//    int kernelSize = GAUSSIAN_RANGE * 2 + 1;
//    double *kernel = Build_GaussianKernel(kernelSize);
//
//    unsigned char *r = calloc(h * w, sizeof(*r));
//
//    for (int y = 0; y < h; ++y)
//    {
//        for (int x = GAUSSIAN_RANGE; x < w - GAUSSIAN_RANGE; ++x)
//        {
//            double sum = 0;
//            for (int k = 0; k < kernelSize; ++k)
//            {
//                sum += pixels[y][x + k - GAUSSIAN_RANGE].r * kernel[k];
//            }
//            r[y * w + x] = clamp(sum, 0, 255);
//        }
//    }
//
//    for (int y = GAUSSIAN_RANGE; y < h - GAUSSIAN_RANGE; ++y)
//    {
//        for (int x = GAUSSIAN_RANGE; x < w - GAUSSIAN_RANGE; ++x)
//        {
//            double sum = 0;
//            for (int k = 0; k < kernelSize; ++k)
//            {
//                sum += r[(y + k - GAUSSIAN_RANGE) * w + x] * kernel[k];
//            }
//            set_all_pixel(image, y, x, clamp(sum, 0, 255));
//        }
//    }
//
//    free_image(&c_image);
//    free(r);
//    free(kernel);
//}




//int get_pixel_one_dim(Image *source, int index)
//{
//    int i = index / source->width;
//    int j = index % source->width;
//    return source->pixels[i][j].r;
//}
//
//void set_pixel_one_dim(Image *source, int index, int val)
//{
//    int i = index / source->width;
//    int j = index % source->width;
//    set_all_pixel(source, i, j, val);
//}
//
//int *gaussian_kernel(int sigma, int n)
//{
//    float w_ideal = sqrt((12.0 * sigma * sigma / n) + 1);
//    int wl = floor(w_ideal);
//    if (wl % 2 == 0)
//        wl--;
//    int wu = wl + 2;
//
//    float m_ideal =
//        (12 * sigma * sigma - n * wl * wl - 4 * n * wl - 3 * n) / (-4 * wl - 4);
//    int m = round(m_ideal);
//
//    int *kernel = malloc(n * sizeof(int));
//
//    for (int i = 0; i < n; i++)
//        kernel[i] = (i < m ? wl : wu);
//
//    return kernel;
//}
//
//void box_blur_h(Image *source, Image *target, int w, int h, int radius)
//{
//    double iarr = (double)1 / (radius + radius + 1);
//    for (int i = 0; i < h; i++)
//    {
//        int ti = i * w;
//        int li = ti;
//        int ri = ti + radius;
//        int fv = get_pixel_one_dim(source, ti);
//        int lv = get_pixel_one_dim(source, ti + w - 1);
//
//        unsigned currennt_val = fv * (radius + 1);
//
//        for (int j = 0; j < radius; j++)
//        {
//            int val = get_pixel_one_dim(source, ti + j);
//            currennt_val += val;
//        }
//
//        for (int j = 0; j <= radius; j++)
//        {
//            int val = get_pixel_one_dim(source, ri++);
//            currennt_val += val;
//
//            set_pixel_one_dim(target, ti++, currennt_val * iarr);
//        }
//
//        for (int j = radius + 1; j < w - radius; j++)
//        {
//            int first_pixel_val = get_pixel_one_dim(source, ri++);
//            int second_pixle_val = get_pixel_one_dim(source, li++);
//
//            currennt_val += first_pixel_val - second_pixle_val;
//
//            set_pixel_one_dim(target, ti++, currennt_val * iarr);
//        }
//
//        for (int j = w - radius; j < w; j++)
//        {
//            int pixel_val = get_pixel_one_dim(source, li++);
//            currennt_val += lv - pixel_val;
//
//            set_pixel_one_dim(target, ti++, currennt_val * iarr);
//        }
//    }
//}
//
//void box_blur_t(Image *source, Image *target, int w, int h, int radius)
//{
//    double iarr = (double)1 / (radius + radius + 1);
//    for (int i = 0; i < w; i++)
//    {
//        int ti = i;
//        int li = ti;
//        int ri = ti + radius * w;
//
//        int fv = get_pixel_one_dim(source, ti);
//        int lv = get_pixel_one_dim(source, ti + w * (h - 1));
//        unsigned currennt_val = fv * (radius + 1);
//
//        for (int j = 0; j < radius; j++)
//        {
//            int val = get_pixel_one_dim(source, ti + j * w);
//            currennt_val += val;
//        }
//
//        for (int j = 0; j <= radius; j++)
//        {
//            int val = get_pixel_one_dim(source, ri);
//            currennt_val += val - fv;
//
//            set_pixel_one_dim(target, ti, currennt_val * iarr);
//
//            ri += w;
//            ti += w;
//        }
//
//        for (int j = radius + 1; j < h - radius; j++)
//        {
//            int first_pixel_val = get_pixel_one_dim(source, ri);
//            int second_pixle_val = get_pixel_one_dim(source, li);
//
//            currennt_val += first_pixel_val - second_pixle_val;
//
//            set_pixel_one_dim(target, ti, currennt_val * iarr);
//
//            li += w;
//            ri += w;
//            ti += w;
//        }
//
//        for (int j = h - radius; j < h; j++)
//        {
//            int pixel_val = get_pixel_one_dim(source, li);
//            currennt_val += lv - pixel_val;
//            set_pixel_one_dim(target, ti, currennt_val * iarr);
//
//            li += w;
//            ti += w;
//        }
//    }
//}
//
//void box_blur(Image *source, Image *target, int w, int h, int radius)
//{
//    for (int i = 0; i < h; i++)
//    {
//        for (int j = 0; j < w; j++)
//        {
//            target->pixels[i][j] = source->pixels[i][j];
//        }
//    }
//
//    box_blur_h(target, source, w, h, radius);
//    box_blur_t(source, target, w, h, radius);
//}
//
//void gaussian_blur(Image *source, int radius)
//{
//    int width = source->width;
//    int height = source->height;
//    // allocate image
//    Image *target = malloc(sizeof(Image));
//
//    target->width = width;
//    target->height = height;
//
//    target->pixels = calloc(height, sizeof(Pixel *));
//    for (int row = 0; row < height; row++)
//    {
//        target->pixels[row] = calloc(width, sizeof(Pixel));
//    }
//
//    if (source->path != NULL)
//    {
//        target->path = calloc(strlen(source->path) + 1, sizeof(char));
//        strcpy(target->path, source->path);
//    }
//
//    int *bxs = gaussian_kernel(radius, 3);
//    box_blur(source, target, width, height, (bxs[0] - 1) / 2);
//    box_blur(target, source, width, height, (bxs[1] - 1) / 2);
//    box_blur(source, target, width, height, (bxs[2] - 1) / 2);
//
//    for (int i = 0; i < height; i++)
//    {
//        for (int j = 0; j < width; j++)
//        {
//            source->pixels[i][j] = target->pixels[i][j];
//        }
//    }
//
//    free(bxs);
//    free_image(target);
//}


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
