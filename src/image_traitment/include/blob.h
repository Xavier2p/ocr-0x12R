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

Square main_blob(Image *image);
#endif
