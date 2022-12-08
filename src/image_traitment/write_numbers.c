#include "include/write_number.h"

void resize_draw(Image *src, Image *number_img, int x, int y, int dimension,
                 int color)
{
    int block = dimension / 9;
    int num_size = dimension / 14;
    int gap = (block - num_size) / 2;
    // int xgap = gap * 4 / 5;

    int new_width = 0;
    int new_height = 0;

    if (number_img->height == number_img->width)
    {
        new_width = num_size;
        new_height = num_size;
    }

    else if (number_img->height > number_img->width)
    {
        new_width =
            (int)((float)num_size
                  * ((float)number_img->width / (float)number_img->height));
        new_height = num_size;
    }

    else
    {
        new_width = num_size;
        new_height =
            (int)((float)num_size
                  * ((float)number_img->height / (float)number_img->width));
    }

    int w1 = number_img->width;
    int h1 = number_img->height;
    // calculate the ratio between the new size and the old size
    int j_ratio = (int)((w1 << 16) / new_width) + 1;
    int i_ratio = (int)((h1 << 16) / new_height) + 1;

    Pixel white = src->pixels[x * block + gap - 1][y * block + gap - 1];

    Pixel blue = { .r = 133, .g = 161, .b = 242 };

    Pixel brown = { .r = 255, .g = 124, .b = 107 };

    Pixel brown_change = { .r = 135, .g = 227, .b = 207 };
    // calculate the new pixels
    int j2, i2;
    for (int i = 0; i < new_height; i++)
    {
        for (int j = 0; j < new_width; j++)
        {
            j2 = ((j * j_ratio) >> 16);
            i2 = ((i * i_ratio) >> 16);

            // take the background of the current block
            Pixel curP = { .r = number_img->pixels[i2][j2].r,
                           .g = number_img->pixels[i2][j2].g,
                           .b = number_img->pixels[i2][j2].b };

            // color in blue
            if (color == 1 && curP.r < 12)
                src->pixels[x * block + i + gap][y * block + j + gap] = blue;

            // color in brown
            else if (!color && curP.r < 12)
                src->pixels[x * block + i + gap][y * block + j + gap] = brown;

            else if (color == 3 && curP.r < 12)
                src->pixels[x * block + i + gap][y * block + j + gap] =
                    brown_change;

            // color in white and search the brown color
            else if (color == 2 && curP.r < 12)
                src->pixels[x * block + i + gap][y * block + j + gap] = white;
        }
    }
}

void add_number(Image *src, int x, int y, int number, int color)
{
    const char *file;

    switch (number)
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

    Image number_img =
        create_image(number_img_sdl, number_img_sdl->w, number_img_sdl->h);

    int size = (src->width + src->height) / 2;

    resize_draw(src, &number_img, x, y, size, color);

    free_image(&number_img);
}

Image write_numbers(int **origin, int **solved)
{
    SDL_Surface *sudoku_sdl = IMG_Load("src/image_traitment/numbers/grid.png");

    Image sudoku_img = create_image(sudoku_sdl, sudoku_sdl->w, sudoku_sdl->h);

    for (int y = 0; y < 9; y++)
    {
        for (int x = 0; x < 9; x++)
        {
            if (origin[x][y] == 0 && origin != solved)
                add_number(&sudoku_img, x, y, solved[x][y], 1); // blue

            else if (origin[x][y] != 0)
                add_number(&sudoku_img, x, y, solved[x][y], 0); // brown
        }
    }

    return sudoku_img;
}

void change_number(Image *sudoku_img, int **grid, int x, int y, int number)
{
    add_number(sudoku_img, x, y, grid[x][y], 2); // white

    if (number != 0)
        add_number(sudoku_img, x, y, number, 3); // brown

    grid[x][y] = number;
}
