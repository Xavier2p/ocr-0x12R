#include "includes/gui.h"
#include "includes/preprocess.h"

char* launcher_grayscale()
{
    grayscale(&image);
    return "Set to Grayscale";
}

char* launcher_contrast()
{
    image_contrast(&image, 12);
    return "Contrast Applied";
}

char* launcher_normalize()
{
    image_normalize_brightness(&image);
    return "Normalized Brightness";
}

