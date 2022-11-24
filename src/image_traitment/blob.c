#include "../../include/image_traitment/blob.h"

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

MyList main_blob(Image *image)
{
    int w = image->width;
    int h = image->height;
}
