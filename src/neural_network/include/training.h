#ifndef TRAINING_H
#define TRAINING_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <err.h>
#include "neural_network.h"
#include "save_load.h"
#include "load_numeric.h"

//----- ARGUMENT MANAGEMENT -----//

void remove_cursor(int disable);

//----- DATASET TRAIN AND TEST -----//

unsigned int fetch_result(Network* network);

void train(unsigned int nb_hidden, unsigned int nb_neurons,
           double learning_rate);

int test(Network* network, double clean_input[]);

//----- MAIN -----//

int is_empty(double image[]);

int run(Network* network, double image[]);

int training(double nb_hidden, double nb_neurons, double learning_rate,
             double image[], int state);

#endif
