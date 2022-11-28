#ifndef BLOB_H
#define BLOB_H

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wctype.h>
#include <sys/queue.h>

#include "linkedlist.h"
#include "struct.h"
#include "utilis_image.h"

struct DotQueue
{
    Dot dot;
    SLIST_ENTRY(DotQueue) next;
};

SLIST_HEAD(slisthead, DotQueue);

void main_blob(Image *image);
#endif
