#include "../../include/image_traitment/segmentation.h"

double *create_square_image(Image *image, int i, int j, int size)
{
    Image tmp = { .width = size, .height = size, .pixels = NULL, .path = NULL };

    tmp.pixels = (Pixel **)calloc(size, sizeof(Pixel *));
    for (unsigned int x = 0; x < size; ++x)
    {
        tmp.pixels[x] = (Pixel *)calloc(size, sizeof(Pixel));
        if (tmp.pixels[x] == NULL)
            errx(EXIT_FAILURE,
                 "Error while allocating pixels pointers for the image");
    }

    for (int _i = i; _i < i + size; ++i)
    {
        for (int _j = j; _j < j + size; ++j)
        {
            tmp.pixels[_i - i][_j - j].r = image->pixels[i + _i][j + _j].r;
            tmp.pixels[_i - i][_j - j].g = image->pixels[i + _i][j + _j].g;
            tmp.pixels[_i - i][_j - j].b = image->pixels[i + _i][j + _j].b;
        }
    }

    Image tmp_resized = resize_image(&tmp, SIZE_OF_NEURAL_INPUT);
    double *res =
        calloc(SIZE_OF_NEURAL_INPUT * SIZE_OF_NEURAL_INPUT, sizeof(double));

    for (int _i = 0; _i < size; ++i)
        for (int _j = 0; _j < size; ++j)
            res[_i * size + j] = tmp_resized.pixels[i][j].r == 255 ? 1 : 0;
    
    return res;
}



