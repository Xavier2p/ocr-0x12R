#ifndef UTILIS_IMAGE_H
#define UTILIS_IMAGE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "struct.h"

#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

typedef struct Pixel
{
    unsigned int r, g, b;
} Pixel;

typedef struct Image
{
    unsigned int width;
    unsigned int height;
    Pixel **pixels;
    char *path;
} Image;

void draw_dot(Image *image, Dot *dot, int size);

Uint32 get_pixel(SDL_Surface *surface, int x, int y);

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);

Image create_image(SDL_Surface *surface, int width, int height);

SDL_Surface *create_surface(Image *image);

void set_all_pixel(Image *image, int i, int j, unsigned int val);

void free_image(Image *image);

Image copy_image(Image *image);

void resize_image(Image *image, int dimension);

void save_image(Image *image, char *name);
#endif
