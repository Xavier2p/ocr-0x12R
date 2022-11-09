#ifndef SDL_IMPORT_UTILS_H
#define SDL_IMPORT_UTILS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <err.h>
#include <stdlib.h>

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

Uint32 get_pixel(SDL_Surface *surface, int x, int y);

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel);

Image create_image(SDL_Surface *surface, int width, int height);

SDL_Surface *create_surface(Image *image);

#endif
