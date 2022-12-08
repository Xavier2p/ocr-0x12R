#ifndef SEGMENTAION_H
#define SEGMENTAION_H

#include <stdlib.h>
#include <stdio.h>

#include "utilis_image.h"
#include "blob.h"
#include "../../neural_network/include/training.h"

#define SIZE_OF_NEURAL_INPUT 28

double *create_square_image(Image *image, int i, int j, int size, int cordi,
                            int cordj);

int **segmentation(Image *image, Network *n);

#endif
