#include "includes/gui.h"
#include "../neural_network/include/training.h"

char *launcher_grayscale(Image *image)
{
    grayscale(image);
    return "Set to Grayscale";
}

char *launcher_contrast(Image *image)
{
    image_contrast(image, 12);
    return "Contrast Applied";
}

char *launcher_normalize(Image *image)
{
    image_normalize_brightness(image);
    return "Normalized Brightness";
}

// char *launcher_threshold(Image *image)
//{
//     // adaptative_threshold(image);
//     return "[NI] Threshold Applied";
// }
//
// char *launcher_canny(Image *image)
//{
//     // canny_edge_detection(image);
//     return "[NI] Canny Applied";
// }
//
// char *launcher_blob(Image *image)
//{
//     // main_blob(image);
//     return "[NI] Blob Applied";
// }
//
// char *launcher_dilatation(Image *image)
//{
//     // dilatation(image);
//     return "[NI] Dilatation Applied";
// }
//
// char *launcher_erosion(Image *image)
//{
//     // erodation(image);
//     return "[NI] Erosion Applied";
// }

int launcher_neural(double hl, double nbn, double lr)
{
    return training(NULL, hl, nbn, lr, NULL, 1);
}
