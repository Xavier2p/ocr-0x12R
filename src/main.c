/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Main file to run the OCR
 *
 *        Version:  1.0
 *        Created:  11/30/2022 09:13:02 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Paolo Wattebled, Arthur Guelennoc, Xavier de Place,
 *         Mathieu Pastre
 *   Organization:  Ox12R
 *
 * =====================================================================================
 */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "image_traitment/include/utilis_image.h"
#include "neural_network/include/neural_network.h"
#include "image_traitment/include/image_traitment.h"
#include "image_traitment/include/segmentation.h"
#include "neural_network/include/training.h"
#include "sudoku_solver/sudoku_solver.h"
#include "gui/includes/gui.h"

void image(const char* path)
{
    // Import image
    SDL_Surface *surface = IMG_Load(path);
    printf("Imported image of size %ix%i\n", surface->w, surface->h);
    Image tmp_image = create_image(surface, surface->w, surface->h);

    // Create the name to save image
    tmp_image.path = (char *)calloc(strlen(path) + 5, sizeof(char));
    tmp_image.path[0] = 'r';
    tmp_image.path[1] = 'e';
    tmp_image.path[2] = 's';
    tmp_image.path[3] = '_';
    strcat(tmp_image.path, path);

    // Resize the image and free the others
    Image image = resize_image(&tmp_image, 800);
    printf("new h = %i , new w = %i\n", image.height, image.width);

    // Free imnage and surface used for import
    free_image(&tmp_image);
    SDL_FreeSurface(surface);

    // Initialise the neural network
    Network n;
    load_weights(&n, PATH_TO_WEIGHTS);

    printf("\n");
    // Compute all operation on the image to segment it
    Image computed_image = image_traitment(&image);

    free_image(&image);

    // Segment the image and export the grid for the sudoku solver
    int **sudoku_grid = segmentation(&computed_image, &n);

    printf("\nNon-Solved Grid: \n");
    print_grid(sudoku_grid);

    // Solve the grid
    solve_sudoku(sudoku_grid, 0, 0);

    printf("\nSolved Grid: \n");
    print_grid(sudoku_grid);

    for (int i = 0; i < 10; ++i)
        free(sudoku_grid[i]);
    free(sudoku_grid);
    free_network(&n);
    free_image(&computed_image);

    printf("\n");
    SDL_Quit();
}

void train(unsigned int nb_hidden, unsigned int nb_neurons, double learning_rate)
{
    training(NULL, nb_hidden, nb_neurons, learning_rate, NULL, 1);
}

char* strtoupper(char* string)
{
    for(size_t i = 0; i < strlen(string); i++)
    {
        string[i] = toupper(string[i]);
    }
    return string;
}

void print_usage()
{
    printf("Usage: neural -p <path> | -h <nb_hidden> -n <nb_neurons> -l "
            "<nb_layers> | -i <image> | -s <state>\n");
    exit(2);
}

#define OPTSTR "hvi:o:n:m:b:c:l:a:"
char* MODE[] = {"IMAGE", "TRAIN", "GUI"};
typedef struct {
    char* input;
    int nb_layers;
    int nb_neurons;
    double learning_rate;
    int verbose;
    int mode;
} t_options;

static const char* ARGS_HELP =
"%s 0x12R help:\n"
"[ Image mode options ]\n"
"   -i file: Specify the input file (required)\n"
"[ Train mode options ]\n"
"   -l n / --nb-layers n: Specify the number of hidden layers (default: 2)\n"
"   -n n / --nb-neurons n: Specify the numbers of neurons per hidden layer (default: 200)\n"
"   -r n / --learning-rate n: Specify the learning rate (default: 0.1)\n"
"[ General options ]\n"
"   -v: Set the verbose level (default: 1)\n"
"   --mode mode: Specify the mode to use. Can be one of IMAGE/TRAIN/GUI (default is GUI)\n"
"   -h / --help: Show usage and quit\n"
;

int main(int argc, char **argv)
{
    int c;
    char output[100];

    t_options options = {
        NULL,   // input file
        2,      // hidden layers
        200,    // nb neurons
        0.1,    // learning rate
        1,      // verbose
        2,      // mode
    };

    static struct option long_options[] = {
        {"mode", required_argument, 0, 'm'},
        {"help", no_argument, 0, 'h'},
        {"nb-layers", required_argument, 0, 'l'},
        {"nb-neurons", required_argument, 0, 'n'},
        {"learning-rate", required_argument, 0, 'r'},
        {0, 0, 0, 0}
    };

    while (1)
    {
        int option_index = 0;
        c = getopt_long(argc, argv, OPTSTR, long_options, &option_index);
        if (c == -1)
            break;

        switch(c)
        {
            case 'v':
                options.verbose = atoi(optarg);
                break;
            case 'i':
                options.input = optarg;
                break;
            case 'm':
                strcpy(output, optarg);
                strtoupper(output);
                int found = 0;
                for (size_t i = 0; i < 3; ++i)
                {
                    if (strcmp(optarg, MODE[i]) == 0)
                    {
                        found = 1;
                        options.mode = i;
                    }
                }
                if (found == 0)
                    print_usage();
                break;
            case 'h':
                print_usage();
                break;
            case 'l':
                options.nb_layers = atoi(optarg);
                break;
            case 'n':
                options.nb_neurons = atoi(optarg);
                break;
            case 'r':
                options.learning_rate = atof(optarg);
                break;
            default:
                print_usage();
                break;
        }
    }

    switch(options.mode)
    {
        case 0: // IMAGE
            if (options.input == NULL)
                errx(1, "Invalid arguments: IMAGE mode is specified without the -i flag");
            image(options.input);
            break;
        case 1: // TRAIN
            train(options.nb_layers, options.nb_neurons, options.learning_rate);
            break;
        case 2: // GUI
            init_gui(argc, argv);
            break;
        default:
            print_usage();
            break;
    }

    return 0;
}
