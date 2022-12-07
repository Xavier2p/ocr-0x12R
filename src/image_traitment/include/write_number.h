#ifndef ADAPTATIVE_THRESHOLD_H
#    define ADAPTATIIVE_THRESHOLD_H

#    include "utilis_image.h"


Image write_numbers(int **origin, int **solved);
void change_number(Image *sudoku_img, int **grid, int x, int y, int number);

#endif
