#ifndef LOAD_NUMERIC_H
#define LOAD_NUMERIC_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <unistd.h>
#include <dirent.h>

#define SIZE 784
#define NUM_TRAIN 6298
#define NUM_TEST 6298

void load_numeric(double train_image[NUM_TRAIN][SIZE],
                  double test_image[NUM_TEST][SIZE], int train_label[NUM_TRAIN],
                  int test_label[NUM_TEST]);

#endif
