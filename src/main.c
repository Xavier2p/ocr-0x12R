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
#include "main.h"

//----- HELPER FUNCTIONS -----//

char* strtoupper(char* string)
{
    for (size_t i = 0; i < strlen(string); i++)
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

//----- MAIN -----//

void main_image(const char* path)
{
    // Import image
    SDL_Surface* surface = IMG_Load(path);
    printf("Imported image of size %ix%i\n", surface->w, surface->h);
    Image image = create_image(surface, surface->w, surface->h);

    // Create the name to save image
    image.path = (char*)calloc(strlen(path) + 5, sizeof(char));
    image.path[0] = 'r';
    image.path[1] = 'e';
    image.path[2] = 's';
    image.path[3] = '_';
    strcat(image.path, path);

    // Resize the image and free the others
    resize_image(&image, 800);
    printf("new h = %i , new w = %i\n", image.height, image.width);

    // Free imnage and surface used for import
    SDL_FreeSurface(surface);

    // Initialise the neural network
    Network n;
    load_weights(&n, PATH_TO_WEIGHTS);

    printf("\n");
    // Compute all operation on the image to segment it
    image_traitment(&image);

    // Segment the image and export the grid for the sudoku solver
    int** sudoku_grid = segmentation(&image, &n);

    printf("\nNon-Solved Grid: \n");
    print_grid(sudoku_grid);

    // Solve the grid
    solve_sudoku(sudoku_grid);

    printf("\nSolved Grid: \n");
    print_grid(sudoku_grid);

    for (int i = 0; i < 10; ++i)
        free(sudoku_grid[i]);
    free(sudoku_grid);
    free_network(&n);
    free_image(&image);

    printf("\n");
    SDL_Quit();
}

void main_train(unsigned int nb_hidden, unsigned int nb_neurons,
                double learning_rate)
{
    training(NULL, nb_hidden, nb_neurons, learning_rate, NULL, 1);
}

int main(int argc, char** argv)
{
    int c;
    char output[100];

    t_options options = {
        NULL, // input file
        2, // hidden layers
        200, // nb neurons
        0.1, // learning rate
        1, // verbose
        2, // mode
    };

    static struct option long_options[] = {
        { "mode", required_argument, 0, 'm' },
        { "help", no_argument, 0, 'h' },
        { "nb-layers", required_argument, 0, 'l' },
        { "nb-neurons", required_argument, 0, 'n' },
        { "learning-rate", required_argument, 0, 'r' },
        { 0, 0, 0, 0 }
    };

    while (1)
    {
        int option_index = 0;
        c = getopt_long(argc, argv, OPTSTR, long_options, &option_index);
        if (c == -1)
            break;

        switch (c)
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

    switch (options.mode)
    {
    case 0: // IMAGE
        if (options.input == NULL)
            errx(1,
                 "Invalid arguments: IMAGE mode is specified without the -i "
                 "flag");
        main_image(options.input);
        break;

    case 1: // TRAIN
        main_train(options.nb_layers, options.nb_neurons,
                   options.learning_rate);
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
