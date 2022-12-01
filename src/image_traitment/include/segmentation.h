#ifndef SEGMENTAION_H
#define SEGMENTAION_H

#include <stdlib.h>
#include "utilis_image.h"
#include "../../neural_network/include/training.h"

#define PATH_TO_WEIGHTS "best_weights-83.data"
#define SIZE_OF_NEURAL_INPUT 28

double *create_square_image(Image *image, int i, int j, int size, int cordi,
                            int cordj);

void segmentation(Image *image);

#endif
