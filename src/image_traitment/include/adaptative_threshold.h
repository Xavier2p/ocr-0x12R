#ifndef ADAPTATIVE_THRESHOLD_H
#define ADAPTATIVE_THRESHOLD_H

#include "utilis_image.h"
#include "gaussian_filter.h"
#include <math.h>
#include <stdlib.h>

void adaptative_threshold(Image *image);

void dilatation(Image *image);

void erosion(Image *image);

#endif
