#include "include/blob.h"

int blob_detection(Image *image, Dot start, unsigned int prev, int new_color)
{
    int w = image->width;
    int h = image->height;
    int nb_dots = 0;

    struct slisthead head;
    SLIST_INIT(&head);

    struct DotQueue *p = malloc(sizeof(*p));
    p->dot = start;
    SLIST_INSERT_HEAD(&head, p, next);

    while (!SLIST_EMPTY(&head))
    {
        p = SLIST_FIRST(&head);
        SLIST_REMOVE_HEAD(&head, next);

        int x = p->dot.X;
        int y = p->dot.Y;
        free(p);

        if (x < 0 || x >= w || y < 0 || y >= h)
            continue;

        if (image->pixels[y][x].r == prev)
        {
            set_all_pixel(image, y, x, new_color);
            nb_dots++;

            p = malloc(sizeof(*p));
            p->dot.X = x - 1;
            p->dot.Y = y;
            SLIST_INSERT_HEAD(&head, p, next);

            p = malloc(sizeof(*p));
            p->dot.X = x + 1;
            p->dot.Y = y;
            SLIST_INSERT_HEAD(&head, p, next);

            p = malloc(sizeof(*p));
            p->dot.X = x;
            p->dot.Y = y - 1;
            SLIST_INSERT_HEAD(&head, p, next);

            p = malloc(sizeof(*p));
            p->dot.X = x;
            p->dot.Y = y + 1;
            SLIST_INSERT_HEAD(&head, p, next);
        }
    }

    return nb_dots;
}

Square compute_corners(Image *image)
{
    Pixel **pixels = image->pixels;
    int w = image->width;
    int h = image->height;

    Dot tl = { 0, 0 };
    Dot br = { 0, 0 };
    Dot bl = { 0, 0 };
    Dot tr = { 0, 0 };

    size_t minSum = w + h;
    size_t maxSum = 0;
    int minDiff = w > h ? w : h;
    int maxDiff = -1 * minDiff;

    for (int y = 0; y < h; ++y)
    {
        for (int x = 0; x < w; ++x)
        {
            if (pixels[y][x].r != 88)
                continue;

            size_t sum = x + y;
            int diff = x - y;

            if (sum < minSum)
            {
                minSum = sum;
                tl.X = x;
                tl.Y = y;
            }

            if (sum > maxSum)
            {
                maxSum = sum;
                br.X = x;
                br.Y = y;
            }

            if (diff < minDiff)
            {
                minDiff = diff;
                bl.X = x;
                bl.Y = y;
            }

            if (diff > maxDiff)
            {
                maxDiff = diff;
                tr.X = x;
                tr.Y = y;
            }
        }
    }

    Square res = { .tl = tl, .tr = tr, .bl = bl, .br = br };
    return res;
}

Dot find_biggest_blob(Image *image)
{
    int w = image->width;
    int h = image->height;
    Image c_image = copy_image(image);

    Dot max_start;
    int max_point = 0;

    for (int i = 0; i < h; ++i)
    {
        for (int j = 0; j < w; ++j)
        {
            Dot tmp_start = { .X = j, .Y = i };
            int tmp_nb = blob_detection(&c_image, tmp_start, 255, 0);

            if (tmp_nb > max_point)
            {
                max_start.X = tmp_start.X;
                max_start.Y = tmp_start.Y;
                max_point = tmp_nb;
            }
        }
    }

    free_image(&c_image);
    return max_start;
}

void remove_small_blob(Image *image)
{
    int threshold = 70;
    int w = image->width;
    int h = image->height;

    for (int i = 0; i < h; ++i)
    {
        for (int j = 0; j < w; ++j)
        {
            if (image->pixels[i][j].r != 255)
                continue;

            Dot tmp_start = { .X = j, .Y = i };
            int tmp_nb = blob_detection(image, tmp_start, 255, 88);

            if (tmp_nb < threshold)
            {
                blob_detection(image, tmp_start, 88, 0);
            }
        }
    }

    for (int i = 0; i < h; ++i)
    {
        for (int j = 0; j < w; ++j)
        {
            Dot tmp_start = { .X = j, .Y = i };
            if (image->pixels[i][j].r == 88)
                blob_detection(image, tmp_start, 88, 255);
        }
    }
}

Square find_corners(Image *image)
{
    Dot biggest_blob_dot = find_biggest_blob(image);

    blob_detection(image, biggest_blob_dot, 255, 88);

    Square corners = compute_corners(image);

    draw_dot(image, &corners.bl, 4);
    draw_dot(image, &corners.br, 4);
    draw_dot(image, &corners.tl, 4);
    draw_dot(image, &corners.tr, 4);

    return corners;
}
