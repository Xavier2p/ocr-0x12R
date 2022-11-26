#include "../../include/image_traitment/blob.h"
#include <sys/types.h>
#include <time.h>

void free_blob_list(MyList *list)
{
    Node *n = list->head;
    for (; n != NULL; n = n->next)
    {
        Blob *blob = (Blob *)n->value;
        free(blob->dots);
    }
    free_list(list);
}

Blob copy_blob(Blob *blob)
{
    Blob res = { .length = blob->length, .dots = NULL };
    res.dots = (Dot *)calloc(blob->length, sizeof(Dot));
    for (int i = 0; i < blob->length; ++i)
    {
        Dot tmp = { .X = blob->dots[i].X, .Y = blob->dots[i].Y };
        res.dots[i] = tmp;
    }
    return res;
}

int blob_detection(Image *image, Dot start)
{
    Pixel **pixels = image->pixels;
    int w = image->width;
    int h = image->height;
    int nb_dots = 0;

    MyQueue q = { NULL, NULL, 0 }; // Queue of Dot

    if (pixels[start.Y][start.X].r == 255)
        enqueue(&q, Dot_tovptr(start));

    while (!is_empty(&q))
    {
        Dot *p = (Dot *)dequeue(&q);
        int x = p->X;
        int y = p->Y;
        free(p);

        if (x < 0 || x >= w || y < 0 || y >= h)
            continue;

        if (pixels[y][x].r == 255)
        {
            set_all_pixel(image, y, x, 0);
            nb_dots++;

            Dot d1 = { .X = x - 1, .Y = y };
            enqueue(&q, Dot_tovptr(d1));

            Dot d2 = { .X = x + 1, .Y = y };
            enqueue(&q, Dot_tovptr(d2));

            Dot d3 = { .X = x, .Y = y - 1 };
            enqueue(&q, Dot_tovptr(d3));

            Dot d4 = { .X = x, .Y = y + 1 };
            enqueue(&q, Dot_tovptr(d4));
        }
    }

    free_queue(&q);
    return nb_dots;
}

void main_blob(Image *image)
{
    int w = image->width;
    int h = image->height;
    Image c_image = copy_image(image);
    Pixel **pixels = c_image.pixels;

    Dot max_start;
    int max_point = 0;

    for (int i = 0; i < h; ++i)
    {
        for (int j = 0; j < w; ++j)
        {
            Dot tmp_start = { .X = j, .Y = i };
            int tmp_nb = blob_detection(&c_image, tmp_start);

            if (tmp_nb > max_point)
            {
                max_start.X = tmp_start.X;
                max_start.Y = tmp_start.Y;
                max_point = tmp_nb;
            }
        }
    }

    blob_detection(image, max_start);
}
