#include "includes/gui.h"
#include "../neural_network/include/training.h"
#include "../image_traitment/include/image_traitment.h"
#include "../image_traitment/include/write_number.h"

Square corners;
int user_input = 0;
int** origin = NULL;
Image first_image;

char* laucher_resize(Image* image)
{
    resize_image(image, 800);
    first_image = copy_image(image);
    return "Resize image";
}

char* launcher_grayscale(Image* image)
{
    grayscale(image);
    return "Set to Grayscale";
}

char* launcher_blur(Image* image)
{
    gaussian_blur(image);
    return "Perform gaussian blur";
}

char* launcher_adaptative_threshold(Image* image)
{
    adaptative_threshold(image);
    return "Threshold Applied";
}

char* launcher_blob(Image* image)
{
    remove_small_blob(image);
    return "Removed small Blob";
}

char* launcher_erosion(Image* image)
{
    erosion(image);
    return "Erosion Applied";
}

char* launcher_dilatation(Image* image)
{
    dilatation(image);
    return "Dilatation Applied";
}

char* launcher_find_corners(Image* image)
{
    Image copy = copy_image(image);
    dilatation(&copy);
    corners = find_corners(image);
    find_corners(&copy);

    for (unsigned int i = 0; i < image->height; ++i)
    {
        for (unsigned int j = 0; j < image->width; ++j)
        {
            if (copy.pixels[i][j].r == 88)
            {
                set_all_pixel(image, i, j, 88);
            }
        }
    }

    free_image(&copy);
    return "Corners found";
}

char* launcher_homographic_transform(Image* image)
{
    homographic_transform(image, &corners, 756);
    return "Homographic transform applied";
}

char* launcher_segmentation(Image* image)
{
    Network n;
    load_weights(&n, user_input);

    origin = segmentation(image, &n);
    free_network(&n);

    Image tmp = write_numbers(origin, origin);

    free_image(image);
    *image = tmp;

    return "Segmentation done, please check the data";
}

char* launcher_solve(Image* image)
{
    int** solved = calloc(10, sizeof(int*));

    for (int i = 0; i < 10; i++)
    {
        solved[i] = calloc(10, sizeof(int));
        for (int j = 0; j < 10; j++)
            solved[i][j] = origin[i][j];
    }

    solve_sudoku(solved);

    Image new_grid = write_numbers(origin, solved);
    free_image(image);

    *image = new_grid;

    for (int i = 0; i < 10; i++)
    {
        free(origin[i]);
        free(solved[i]);
    }
    free(origin);
    free(solved);
    free_image(&first_image);
    return "Solved sudoku";
}

int launcher_train(double hl, double nbn, double lr)
{
    user_input = 1;
    return training(hl, nbn, lr, NULL, 1);
}

Image* get_first_image()
{
    //    homographic_transform(&first_image, &corners, 756);
    //  save_image(&first_image, "first_image.jpeg");
    return &first_image;
}

void set_new_number(Image* image, int x, int y, int value)
{
    change_number(image, origin, y, x, value);
}
