#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/utilis_image.h"


void resize_draw(Image *src, Image *number_img, int x, int y, int dimension){

    int block = dimension / 9;
    int num_size = dimension / 14;
    int gap = block / 4;


    int new_width = 0;
    int new_height = 0;

    if (number_img->height == number_img->width)
    {
        new_width = num_size;
        new_height = num_size;
    }

    else if (number_img->height > number_img->width)
    {
        new_width = (int)((float)num_size
                * ((float)number_img->width / (float)number_img->height));
        new_height = num_size;
    }

    else
    {
        new_width = num_size;
        new_height = (int)((float)num_size
                           * ((float)number_img->height / (float)number_img->width));
    }



    int w1 = number_img->width;
    int h1 = number_img->height;
    // calculate the ratio between the new size and the old size
    int j_ratio = (int)((w1 << 16) / new_width) + 1;
    int i_ratio = (int)((h1 << 16) / new_height) + 1;


    Pixel p = { .r = 87,
                .g = 112,
                .b = 255 };


    // calculate the new pixels
    int j2, i2;
    for (int i = 0; i < new_height; i++)
    {
        for (int j = 0; j < new_width; j++)
        {
            //i += x;
            //j += y;

            j2 = ((j * j_ratio) >> 16);
            i2 = ((i * i_ratio) >> 16);

            Pixel curP = { .r = number_img->pixels[i2][j2].r,
                        .g = number_img->pixels[i2][j2].g,
                        .b = number_img->pixels[i2][j2].b };

            //if black value
            if (curP.r < 12)
                src->pixels[x*block+i+gap][y*block+j+gap] = p;
        }
    }

    //return src;

}



void add_number(Image *src, int x, int y, int number){

    const char *file;

    switch ( number )
    {
        case 0:
            file = "src/image_traitment/numbers/0.png";
            break;

        case 1:
            file = "src/image_traitment/numbers/1.png";
            break;

        case 2:
            file = "src/image_traitment/numbers/2.png";
            break;

        case 3:
            file = "src/image_traitment/numbers/3.png";
            break;

        case 4:
            file = "src/image_traitment/numbers/4.png";
            break;

        case 5:
            file = "src/image_traitment/numbers/5.png";
            break;

        case 6:
            file = "src/image_traitment/numbers/6.png";
            break;

        case 7:
            file = "src/image_traitment/numbers/7.png";
            break;

        case 8:
            file = "src/image_traitment/numbers/8.png";
            break;

        case 9:
            file = "src/image_traitment/numbers/9.png";
            break;


        default:
            errx(EXIT_FAILURE, "Invalid number value");
    }

    SDL_Surface *number_img_sdl = IMG_Load(file);


    Image number_img = create_image(number_img_sdl, number_img_sdl->w,
                                    number_img_sdl->h);

    int size = src->width;

    resize_draw(src, &number_img, x, y, size);


}


void write_numbers(Image *src, int **virgin, int **solved)
{
    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 9; x++)
        {
            if (virgin[x][y] == 0)
            {
                add_number(src, x, y, solved[x][y]);
            }
        }
    }
}
