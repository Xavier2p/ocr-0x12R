#include "include/image_traitment.h"
#include "include/adaptative_threshold.h"
#include "include/blob.h"
#include "include/canny.h"
#include "include/linkedlist.h"
#include "include/otsu.h"
#include "include/preprocess.h"
#include "include/utilis_image.h"

void image_traitment(Image *image)
{
    // Preprocess

    // Grayscale
    grayscale(image);

    // Blur
    gaussian_blur(image);

    // Binarization thechniques

    // Adaptive threshold
    adaptative_threshold(image);

    // Remove small blobs
    remove_small_blob(image);

    erosion(image);
    //      dilatation(image);

    // Find the corners of the grid
    Square corners = find_corners(image);

    // Resize, rotate and correct perspective
    homographic_transform(image, &corners, 756);
    save_image(image, "homographic_transform_");
}
