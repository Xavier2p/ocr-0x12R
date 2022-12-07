#include "includes/gui.h"
#include "../neural_network/include/training.h"
#include "../image_traitment/include/image_traitment.h"

Square corners;
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
    corners = find_corners(image);
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
    load_weights(&n, PATH_TO_WEIGHTS);
    // Add the new file from Arthur

    origin = segmentation(image, &n);
    free_network(&n);
    return "Segmentation done, please check the data";
}

char* launcher_solve(Image* image)
{
    int** solved = calloc(10, sizeof(int*));

    for (int i = 0; i < 10; i++)
    {
        solved[i] = calloc(10, sizeof(int));
        for (int j = 0; j < 10; j++)
        {
            solved[i][j] = origin[i][j];
        }
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
    return "Solved sudoku";
}

int launcher_train(double hl, double nbn, double lr)
{
    return training(NULL, hl, nbn, lr, NULL, 1);
}

Image *get_first_image()
{
    return &first_image;
}

int** get_origin_grid()
{
    return origin;
}
