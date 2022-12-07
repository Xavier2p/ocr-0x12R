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

int get_pixel_one_dim(Image *source, int index)
{
    int i = index / source->width;
    int j = index % source->width;
    return source->pixels[i][j].r;
}

void set_pixel_one_dim(Image *source, int index, int val)
{
    int i = index / source->width;
    int j = index % source->width;
    set_all_pixel(source, i, j, val);
}

int *gaussian_kernel(int sigma, int n)
{
    float w_ideal = sqrt((12.0 * sigma * sigma / n) + 1);
    int wl = floor(w_ideal);
    if (wl % 2 == 0)
        wl--;
    int wu = wl + 2;

    float m_ideal =
        (12 * sigma * sigma - n * wl * wl - 4 * n * wl - 3 * n) / (-4 * wl - 4);
    int m = round(m_ideal);

    int *kernel = calloc(n, sizeof(int));

    for (int i = 0; i < n; i++)
        kernel[i] = (i < m ? wl : wu);

    return kernel;
}

void box_blur_h(Image *source, Image *target, int w, int h, int radius)
{
    double iarr = (double)1 / (radius + radius + 1);
    for (int i = 0; i < h; i++)
    {
        int ti = i * w;
        int li = ti;
        int ri = ti + radius;
        int fv = get_pixel_one_dim(source, ti);
        int lv = get_pixel_one_dim(source, ti + w - 1);

        unsigned currennt_val = fv * (radius + 1);

        for (int j = 0; j < radius; j++)
        {
            int val = get_pixel_one_dim(source, ti + j);
            currennt_val += val;
        }

        for (int j = 0; j <= radius; j++)
        {
            int val = get_pixel_one_dim(source, ri++);
            currennt_val += (val - fv);

            set_pixel_one_dim(target, ti++, currennt_val * iarr);
        }

        for (int j = radius + 1; j < w - radius; j++)
        {
            int first_pixel_val = get_pixel_one_dim(source, ri++);
            int second_pixle_val = get_pixel_one_dim(source, li++);

            currennt_val += first_pixel_val - second_pixle_val;

            set_pixel_one_dim(target, ti++, currennt_val * iarr);
        }

        for (int j = w - radius; j < w; j++)
        {
            int pixel_val = get_pixel_one_dim(source, li++);
            currennt_val += lv - pixel_val;

            set_pixel_one_dim(target, ti++, currennt_val * iarr);
        }
    }
}

void box_blur_t(Image *source, Image *target, int w, int h, int radius)
{
    double iarr = (double)1 / (radius + radius + 1);
    for (int i = 0; i < w; i++)
    {
        int ti = i;
        int li = ti;
        int ri = ti + radius * w;

        int fv = get_pixel_one_dim(source, ti);
        int lv = get_pixel_one_dim(source, ti + w * (h - 1));
        unsigned currennt_val = fv * (radius + 1);

        for (int j = 0; j < radius; j++)
        {
            int val = get_pixel_one_dim(source, ti + j * w);
            currennt_val += val;
        }

        for (int j = 0; j <= radius; j++)
        {
            int val = get_pixel_one_dim(source, ri);
            currennt_val += val - fv;

            set_pixel_one_dim(target, ti, currennt_val * iarr);

            ri += w;
            ti += w;
        }

        for (int j = radius + 1; j < h - radius; j++)
        {
            int first_pixel_val = get_pixel_one_dim(source, ri);
            int second_pixle_val = get_pixel_one_dim(source, li);

            currennt_val += first_pixel_val - second_pixle_val;

            set_pixel_one_dim(target, ti, currennt_val * iarr);

            li += w;
            ri += w;
            ti += w;
        }

        for (int j = h - radius; j < h; j++)
        {
            int pixel_val = get_pixel_one_dim(source, li);

            currennt_val += lv + pixel_val;

            set_pixel_one_dim(target, ti, currennt_val * iarr);

            li += w;
            ti += w;
        }
    }
}

void box_blur(Image *source, Image *target, int w, int h, int radius)
{
    for (int i = 0; i < h; i++)
    {
        for (int j = 0; j < w; j++)
        {
            target->pixels[i][j] = source->pixels[i][j];
        }
    }

    box_blur_h(target, source, w, h, radius);
    box_blur_t(source, target, w, h, radius);
}

void gaussian_blur(Image *source)
{
    int radius = 2;
    int width = source->width;
    int height = source->height;
    // allocate image
    Image target = { .width = source->width,
                     .height = source->height,
                     .pixels = NULL,
                     .path = NULL };

    target.width = width;
    target.height = height;

    target.pixels = calloc(height, sizeof(Pixel *));
    for (int row = 0; row < height; row++)
    {
        target.pixels[row] = calloc(width, sizeof(Pixel));
    }

    if (source->path != NULL)
    {
        target.path = calloc(strlen(source->path) + 1, sizeof(char));
        strcpy(target.path, source->path);
    }

    int *bxs = gaussian_kernel(radius, 3);
    box_blur(source, &target, width, height, (bxs[0] - 1) / 2);
    box_blur(&target, source, width, height, (bxs[1] - 1) / 2);
    box_blur(source, &target, width, height, (bxs[2] - 1) / 2);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            source->pixels[i][j] = target.pixels[i][j];
        }
    }

    free(bxs);
    free_image(&target);
}
