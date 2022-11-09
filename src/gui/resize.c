#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "utilis_image.h"

// create a function to resize the sdl surface
// the function takes the surface and the new size as parameters
// the function returns the resized surface
SDL_Surface *resize_surface(SDL_Surface *surface, int new_width, int new_height)
{
    // create a new surface with the new size
    SDL_Surface *new_surface =
        SDL_CreateRGBSurface(0, new_width, new_height, 32, 0, 0, 0, 0);

    // calculate the ratio between the new size and the old size
    double ratio_width = (double)surface->w / (double)new_width;
    double ratio_height = (double)surface->h / (double)new_height;

    // calculate the new pixels
    for (int x = 0; x < new_height; x++)
    {
        for (int y = 0; y < new_width; y++)
        {
            // calculate the position of the pixel in the old image
            int old_x = (int)(x * ratio_height);
            int old_y = (int)(y * ratio_width);

            // copy the pixel from the old image to the new image
            Uint32 pixel = get_pixel(surface, old_x, old_y);
            put_pixel(new_surface, x, y, pixel);
        }
    }

    // return the new surface
    return new_surface;
}
