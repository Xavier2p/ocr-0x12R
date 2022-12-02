#ifndef PREPROCESS_H
#define PREPROCESS_H

#include "utilis_image.h"

void grayscale(Image *image);

void image_contrast(Image *image, size_t n);

float max_color(Image *image);

void image_normalize_brightness(Image *image);

void invert(Image *image);
#endif
