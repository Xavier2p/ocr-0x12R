#include "include/image_traitment.h"
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
    apply_threshold(image, otsu(image));

    // Find the corners of the grid
    Square corners = main_blob(image);

    // Resize, rotate and correct perspective
    Image final_grid = homographic_transform(image, &corners, 756);

    save_image(&final_grid, "final_grid_");
    return final_grid;
}
