/*
 * =====================================================================================
 *
 *       Filename:  houghtransform.c
 *
 *    Description: Perform hough transform algorithm on the image
 *
 *        Version:  1.0
 *        Created:  10/28/2022 03:38:25 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Paolo Wattebled
 *   Organization:  Fach'OCR
 *
 * =====================================================================================
 */
#include "include/hough_transform.h"

void draw_line(Image *image, Line *line);

MyList hough_transform(Image *image, int threshold)
{
    int width = image->width;
    int height = image->height;
    Pixel **pixels = image->pixels;

    double hough_h =
        (sqrt(2.0) * (double)(height > width ? height : width)) / 2;
    double _accu_h = hough_h * 2.0, _accu_w = 180;
    double center_x = width / 2.0, center_y = height / 2.0;

    unsigned int *_accu =
        (unsigned int *)calloc(_accu_h * _accu_w, sizeof(unsigned int));

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if (pixels[i][j].r > 250)
            {
                for (int t = 0; t < 180; ++t)
                {
                    double r =
                        (((double)j - center_x) * cos((double)t * DEG2RAD))
                        + (((double)i - center_y) * sin((double)t * DEG2RAD));
                    if ((int)((round(r + hough_h) * 180.0)) + t
                        >= _accu_h * _accu_w)
                        continue;
                    _accu[(int)((round(r + hough_h) * 180.0)) + t]++;
                }
            }
        }
    }

    MyList all_lines = { NULL, NULL, 0 };

    for (int r = 0; r < _accu_h - 1; r++)
    {
        for (int t = 0; t < _accu_w - 1; t++)
        {
            if ((int)_accu[(int)(r * _accu_w) + t] >= threshold)
            {
                // Is this point a local maxima (9x9)
                int max = _accu[(int)(r * _accu_w) + t];
                for (int ly = -4; ly <= 4; ly++)
                {
                    for (int lx = -4; lx <= 4; lx++)
                    {
                        if ((ly + r >= 0 && ly + r < _accu_h)
                            && (lx + t >= 0 && lx + t < _accu_w))
                        {
                            if ((int)_accu[(int)((r + ly) * _accu_w) + (t + lx)]
                                > max)
                            {
                                max =
                                    _accu[(int)((r + ly) * _accu_w) + (t + lx)];
                                ly = lx = 5;
                            }
                        }
                    }
                }
                if (max > (int)_accu[(int)(r * _accu_w) + t])
                    continue;

                int x1, y1, x2, y2;
                x1 = y1 = x2 = y2 = 0;

                if (t >= 45 && t <= 135)
                {
                    // y = (r - x cos(t)) / sin(t)
                    x1 = 0;
                    y1 = ((double)(r - (_accu_h / 2))
                          - ((x1 - (width / 2.0)) * cos(t * DEG2RAD)))
                            / sin(t * DEG2RAD)
                        + (height / 2.0);
                    x2 = width - 0;
                    y2 = ((double)(r - (_accu_h / 2))
                          - ((x2 - (width / 2.0)) * cos(t * DEG2RAD)))
                            / sin(t * DEG2RAD)
                        + (height / 2.0);
                }
                else
                {
                    // x = (r - y sin(t)) / cos(t);
                    y1 = 0;
                    x1 = ((double)(r - (_accu_h / 2))
                          - ((y1 - (height / 2.0)) * sin(t * DEG2RAD)))
                            / cos(t * DEG2RAD)
                        + (width / 2.0);
                    y2 = height - 0;
                    x2 = ((double)(r - (_accu_h / 2))
                          - ((y2 - (height / 2.0)) * sin(t * DEG2RAD)))
                            / cos(t * DEG2RAD)
                        + (width / 2.0);
                }
                // append to list line
                Line line;
                line.xStart = x1;
                line.xEnd = x2;
                line.yStart = y1;
                line.yEnd = y2;

                void *p = Line_tovptr(line);
                append(&all_lines, p);
            }
        }
    }

    free(_accu);
    return all_lines;
}

void draw_line(Image *image, Line *line)
{
    int w = image->width;
    int h = image->height;
    Pixel color = { .r = 255, .g = 0, .b = 0 };

    int x0 = line->xStart;
    int y0 = line->yStart;

    int x1 = line->xEnd;
    int y1 = line->yEnd;

    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;

    int err = dx + dy;

    while (1)
    {
        if (0 <= x0 && x0 < w && 0 <= y0 && y0 < h)
        {
            image->pixels[y0][x0].r = color.r;
            image->pixels[y0][x0].g = color.g;
            image->pixels[y0][x0].b = color.b;
        }

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;

        if (e2 >= dy)
        {
            err += dy;
            x0 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

void compute_hough(Image *image)
{
    Image draw_image_hough = copy_image(image);
    int hough_threshold =
        image->width > image->height ? image->width / 4 : image->height / 4;

    MyList lines = hough_transform(image, hough_threshold);
    MyList simplified_lines = simplify_lines(&lines, 40);

    for (size_t i = 0; i < simplified_lines.length; ++i)
    {
        Line *l = get_value(&simplified_lines, i);
        draw_line(&draw_image_hough, l);
    }

    free_list(&lines);
    free_list(&simplified_lines);
    save_image(&draw_image_hough, "res_hough_");
    free_image(&draw_image_hough);
}
