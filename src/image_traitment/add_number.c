#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utilis_image.h"


void resize_draw(Image *src, Image *number_img, int x, int y, int dimension){

    int new_width = 0;
    int new_height = 0;

    if (number_img->height == number_img->width)
    {
        new_width = dimension;
        new_height = dimension;
    }

    else if (number_img->height > number_img->width)
    {
        new_width = (int)((float)dimension
                          * ((float)number_img->width / (float)number_img->height));
        new_height = dimension;
    }

    else
    {
        new_width = dimension;
        new_height = (int)((float)dimension
                           * ((float)number_img->height / (float)number_img->width));
    }



    int w1 = number_img->width;
    int h1 = number_img->height;
    // calculate the ratio between the new size and the old size
    int j_ratio = (int)((w1 << 16) / new_width) + 1;
    int i_ratio = (int)((h1 << 16) / new_height) + 1;


    Pixel p = { .r = 35,
                .g = 122,
                .b = 198 };


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
                src->pixels[y+i][x+j] = p;
        }
    }

    //return src;

}



void add_number(Image src, int x, int y, int size, int number){

    const char *file;

    switch ( number )
    {
        case 0:
            file = "numbers/zero.png";
            break;

        case 1:
            file = "numbers/1.png";
            break;

        case 2:
            file = "numbers/2.png";
            break;

        case 3:
            file = "numbers/3.png";
            break;

        case 4:
            file = "numbers/4.png";
            break;

        case 5:
            file = "numbers/5.png";
            break;

        case 6:
            file = "numbers/6.png";
            break;

        case 7:
            file = "numbers/7.png";
            break;

        case 8:
            file = "numbers/8.png";
            break;

        case 9:
            file = "numbers/9.png";
            break;


        default:
            errx(EXIT_FAILURE, "Invalid number value");
    }

    SDL_Surface *number_img_sdl = IMG_Load(file);


    Image number_img = create_image(number_img_sdl, number_img_sdl->w,
                                    number_img_sdl->h);


    resize_draw(&src, &number_img, x, y, size);


}

