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
#include <stdio.h>

//----- HELPER FUNCTIONS -----//

char* strtoupper(char* string)
{
    for (size_t i = 0; i < strlen(string); i++)
    {
        // Import image
        SDL_Surface *surface = IMG_Load(argv[1]);
        printf("Imported image of size %ix%i\n", surface->w, surface->h);
        Image tmp_image = create_image(surface, surface->w, surface->h);

        // Create the name to save image
        tmp_image.path = (char *)calloc(strlen(argv[1]) + 5, sizeof(char));
        tmp_image.path[0] = 'r';
        tmp_image.path[1] = 'e';
        tmp_image.path[2] = 's';
        tmp_image.path[3] = '_';
        strcat(tmp_image.path, argv[1]);

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
        int **to_solve_grid = NULL;


        // MALLOC AND COPY


        printf("\nNon-Solved Grid: \n");
        print_grid(sudoku_grid);

        // Solve the grid
        solve_sudoku(sudoku_grid, 0, 0);

       // Your function




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
    return string;
}

void print_usage(int error_code)
{
    if (error_code == 0)
        printf("%s", ARGS_HELP);
    else
        errx(error_code, "%s", ARGS_HELP);

    exit(error_code);
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

    const char* output = "Do you wish to save these weights in order to use them in IMAGE mode? [Y/N]";
    printf("%s\n",output);
    char save;
    int n = scanf("%c", &save);

    while (n != 1 || (save != 'Y' && save != 'N'))
    {
        printf("%s\n%s\n", "Wrong input.", output);
        n = scanf("%c", &save);
    }

    if (save == 'Y')
        rename("src/neural_network/saved_data/weights.data", "src/neural_network/saved_data/best_weights.data");
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
        2, // mode
    };

    static struct option long_options[] = {
        { "mode", required_argument, 0, 'm' },
        { "help", no_argument, 0, 'h' },
        { "nb-layers", required_argument, 0, 'l' },
        { "nb-neurons", required_argument, 0, 'n' },
        { "learning-rate", required_argument, 0, 'a' },
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
                    print_usage(1);
                break;

            case 'h':
                print_usage(0);
                break;

            case 'l':
                options.nb_layers = atoi(optarg);
                break;

            case 'n':
                options.nb_neurons = atoi(optarg);
                break;

            case 'a':
                options.learning_rate = atof(optarg);
                break;

            default:
                print_usage(1);
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
            print_usage(1);
            break;
    }

    return 0;
}
