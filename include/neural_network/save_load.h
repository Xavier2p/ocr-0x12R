#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include "neural_network.h"

int is_number(char text);

void save_weights(Network *network, char *path);

void load_weights(Network *network, char *path);

#endif
