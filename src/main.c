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
#include "neural_network/include/neural_network.h"
#include "image_traitment/include/image_traitment.h"

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

        free_image(&tmp_image);
        SDL_FreeSurface(surface);

        image_traitment(&image);

        SDL_Quit();
    }

    return 0;
}
