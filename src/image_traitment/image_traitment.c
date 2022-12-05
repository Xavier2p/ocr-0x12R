#include "include/image_traitment.h"
#include "include/adaptative_threshold.h"
#include "include/blob.h"
#include "include/linkedlist.h"
#include "include/utilis_image.h"

Image image_traitment(Image *image)
{
    // Preprocess
    grayscale(image);

    // Contrast
    image_contrast(image, 12);

    // Brightness
    image_normalize_brightness(image);

    // Blur
    gaussian_blur(image, 4);

    // Otsu threshold
    // apply_threshold(image, otsu(image));

    // Adaptive threshold
    adaptative_threshold(image);
    save_image(image, "gray_scale_");

    // Find the corners of the grid
    Square corners = main_blob(image);

    // Resize, rotate and correct perspective
    Image final_grid = homographic_transform(image, &corners, 756);


    return final_grid;
}
