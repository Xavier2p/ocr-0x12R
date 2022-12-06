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
#include <stdio.h>
#include <stdlib.h>
#include "image_traitment/include/utilis_image.h"
#include "neural_network/include/neural_network.h"
#include "image_traitment/include/image_traitment.h"
#include "image_traitment/include/segmentation.h"
#include "image_traitment/include/write_number.h"
#include "sudoku_solver/sudoku_solver.h"
#include "gui/includes/gui.h"
#include "image_traitment/include/write_number.h"


int main(int argc, char **argv)
{
    if (argc == 2)
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
        int **to_solve_grid = segmentation(&computed_image, &n);


        // Solve the grid
        solve_sudoku(to_solve_grid, 0, 0);


        Image img = write_numbers(sudoku_grid, to_solve_grid);



        SDL_Surface* final_surface = create_surface(&img);

        const char* file_name = "picture.bmp";


        SDL_SaveBMP(final_surface, file_name);




        for (int i = 0; i < 10; ++i)
            free(sudoku_grid[i]);
        free(sudoku_grid);
        free_network(&n);
        free_image(&computed_image);

        printf("\n");
        SDL_Quit();
    }
    else
    {
        init_gui(argc, argv);
    }

    return 0;
}
