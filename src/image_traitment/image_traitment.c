#include "include/image_traitment.h"
#include "include/blob.h"
#include "include/linkedlist.h"
#include "include/utilis_image.h"

Image image_traitment(Image *image)
{
    // Preprocess
    grayscale(image);
    // save_image(&image, "gray_scale_");
    image_contrast(image, 12);
    // save_image(&image, "contrast_");
    image_normalize_brightness(image);
    // save_image(&image, "brightness_");
    gaussian_blur(image, 2);
    save_image(image, "blur_");

    //    adaptative_threshold(image);

    //    canny_edge_detection(image);

    //    save_image(image, "adaptive_");
    // REMOVE SMALL BLOB
    //  main_blob(image);

    // dilatation(image);
    // erodation(image);

    return copy_image(image);
    //    apply_threshold(image, otsu(image));

    // Find the corners of the grid
    //   Square corners = main_blob(image);

    // Resize, rotate and correct perspective
    //  Image final_grid = homographic_transform(image, &corners, 756);

    // save_image(image, "save_");

    // return final_grid;
}
