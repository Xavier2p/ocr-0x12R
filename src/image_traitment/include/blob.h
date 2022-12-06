#ifndef BLOB_H
#define BLOB_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>

#include "struct.h"
#include "utilis_image.h"
#include "adaptative_threshold.h"

struct DotQueue
{
    Dot dot;
    SLIST_ENTRY(DotQueue) next;
};

SLIST_HEAD(slisthead, DotQueue);

int blob_detection(Image *image, Dot start, unsigned int prev, int new_color);

Dot find_biggest_blob(Image *image);

void remove_small_blob(Image *image);

Square find_corners(Image *image);

#endif
