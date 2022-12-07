#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <unistd.h>
#include "neural_network.h"

int is_number(char text);

void save_weights(Network* network, char* path);

void load_weights(Network* network, int path_status);

#endif
