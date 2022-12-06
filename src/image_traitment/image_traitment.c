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

    // Blur
    gaussian_blur(image, 2);


    // Binarization thechniques

    // Adaptive threshold
    adaptative_threshold(image);



    // Remove small blobs
    remove_small_blob(image);

    erosion(image);
    dilatation(image);

    // Find the corners of the grid
    Square corners = main_blob(image);

    // save_image(image, "blob_");

    // Resize, rotate and correct perspective
    Image final_grid = homographic_transform(image, &corners, 756);

    save_image(&final_grid, "adaptative_threshold");
    return final_grid;
}
