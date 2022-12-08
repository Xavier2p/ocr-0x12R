#ifndef MAIN_H
#define MAIN_H
#define OPTSTR "hvi:o:n:m:b:c:l:a:"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <err.h>
#include "image_traitment/include/utilis_image.h"
#include "neural_network/include/neural_network.h"
#include "image_traitment/include/image_traitment.h"
#include "image_traitment/include/segmentation.h"
#include "neural_network/include/training.h"
#include "sudoku_solver/sudoku_solver.h"
#include "gui/includes/gui.h"

const char* MODE[] = { "IMAGE", "TRAIN", "GUI" };

typedef struct
{
    char* input;
    int nb_layers;
    int nb_neurons;
    double learning_rate;
    int mode;
} t_options;

static const char* ARGS_HELP =
    "0x12R help:\n"
    "[ Image mode options ]\n"
    "   -i file: Specify the input file (required)\n"
    "[ Train mode options ]\n"
    "   -l n / --nb-layers n: Specify the number of hidden layers (default: "
    "2)\n"
    "   -n n / --nb-neurons n: Specify the numbers of neurons per hidden layer "
    "(default: 200)\n"
    "   -a n / --learning-rate n: Specify the learning rate (default: 0.1)\n"
    "[ General options ]\n"
    "   --mode MODE: Specify the mode to use. Can be one of IMAGE/TRAIN/GUI "
    "(default: GUI)\n"
    "   -h / --help: Show usage and quit\n";

//----- HELPER FUNCTIONS -----//

void strtoupper(char* string);

void print_usage(int error_code);

//----- MAIN -----//

void main_image(const char* path);

void main_train(unsigned int nb_hidden, unsigned int nb_neurons,
                double learning_rate);

int main(int argc, char** argv);

#endif
