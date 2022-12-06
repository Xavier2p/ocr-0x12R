#include "includes/gui.h"
#include "../neural_network/include/training.h"
#include "../image_traitment/include/image_traitment.h"

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

char *launcher_adaptative_threshold(Image *image)
{
    adaptative_threshold(image);
    return "Threshold Applied";
}

char *launcher_blob(Image *image)
{
    remove_small_blob(image);
    return "Removed small Blob";
}

char *launcher_erosion(Image *image)
{
    erosion(image);
    return "Erosion Applied";
}

char *launcher_dilatation(Image *image)
{
    dilatation(image);
    return "Dilatation Applied";
}

int launcher_train(double hl, double nbn, double lr)
{
    return training(NULL, hl, nbn, lr, NULL, 1);
}
