/*
 * =====================================================================================
 *
 *       Filename:  preprocess.c
 *
 *    Description: Increase contrast - normalize the lights
 *
 *        Version:  1.0
 *        Created:  10/28/2022 02:26:21 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Paolo Wattebled
 *   Organization:  Fach'OCR
 *
 * =====================================================================================
 */
#include "include/preprocess.h"

void grayscale(Image *image)
{
    Pixel **pixels = image->pixels;
    unsigned int w = image->width;
    unsigned int h = image->height;

    unsigned int r, g, b;
    for (unsigned int i = 0; i < h; ++i)
    {
        for (unsigned int j = 0; j < w; ++j)
        {
            r = pixels[i][j].r;
            g = pixels[i][j].g;
            b = pixels[i][j].b;
            unsigned int average = 0.3 * r + 0.59 * g + 0.11 * b;
            set_all_pixel(image, i, j, average);
        }
    }
}

void image_contrast(Image *image, size_t n)
{
    for (size_t i = 0; i < image->height; ++i)
    {
        for (size_t j = 0; j < image->width; ++j)
        {
            float v = image->pixels[i][j].r;
            for (size_t x = 0; i < n; i++)
            {
                if (v >= x * (255.f / n) && v <= (x + 1) * (255.f / n))
                    set_all_pixel(image, i, j, (x + 1) * (255.f / n));
            }
        }
    }
}

float max_color(Image *image)
{
    float m = 0;
    for (size_t i = 0; i < image->height; ++i)
    {
        for (size_t j = 0; j < image->width; ++j)
        {
            if (image->pixels[i][j].r > m)
                m = image->pixels[i][j].r;
        }
    }

    return m;
}

void image_normalize_brightness(Image *image)
{
    float m = max_color(image);
    for (size_t i = 0; i < image->height; ++i)
    {
        for (size_t j = 0; j < image->width; ++j)
        {
            set_all_pixel(image, i, j,
                          255 - image->pixels[i][j].r * (255.f / m));
        }
    }
}

void invert(Image *image)
{
    for (size_t i = 0; i < image->height; ++i)
    {
        for (size_t j = 0; j < image->width; ++j)
        {
            set_all_pixel(image, i, j, 255 - image->pixels[i][j].r);
        }
    }
}
