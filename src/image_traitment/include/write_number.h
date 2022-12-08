#ifndef WRITE_NUMBER_H
#define WRITE_NUMBER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <err.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "utilis_image.h"

Image write_numbers(int **origin, int **solved);
void change_number(Image *sudoku_img, int **grid, int x, int y, int number);

#endif
