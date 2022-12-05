#include "include/image_traitment.h"
#include "include/adaptative_threshold.h"
#include "include/blob.h"
#include "include/canny.h"
#include "include/linkedlist.h"
#include "include/utilis_image.h"

Image image_traitment(Image *image)
{
    // Preprocess

    // Grayscale
    grayscale(image);

    // Contrast
    image_contrast(image, 12);

    // Brightness
    image_normalize_brightness(image);

    // Blur
    gaussian_blur(image, 2);


    // Binarization thechniques

    // Otsu threshold
    // apply_threshold(image, otsu(image));

    // Adaptive threshold
    adaptative_threshold(image);

    // Canny edges
    // canny_edge_detection(image);



    // Remove small blobs
    remove_small_blob(image);

    // Dilatation and erosion
    // erosion(image);
    // dilatation(image);

    save_image(image, "adaptative_threshold");
    // Find the corners of the grid
    Square corners = main_blob(image);

    // save_image(image, "blob_");

    // Resize, rotate and correct perspective
    Image final_grid = homographic_transform(image, &corners, 756);

    return final_grid;
}
