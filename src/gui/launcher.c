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

char* launcher_threshold()
{
    // adaptative_threshold(&image);
    return "[NI] Threshold Applied";
}

char* launcher_canny()
{
    // canny_edge_detection(&image);
    return "[NI] Canny Applied";
}

char* launcher_blob()
{
    // main_blob(&image);
    return "[NI] Blob Applied";
}

char* launcher_dilatation()
{
    // dilatation(&image);
    return "[NI] Dilatation Applied";
}

char* launcher_erosion()
{
    // erodation(&image);
    return "[NI] Erosion Applied";
}
