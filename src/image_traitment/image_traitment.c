#include "include/image_traitment.h"

Image image_traitment(Image *image)
{
    // Preprocess
    grayscale(image);
    // save_image(&image, "gray_scale_");
    image_contrast(image, 12);
    // save_image(&image, "contrast_");
    image_normalize_brightness(image);
    // save_image(&image, "brightness_");
    gaussian_blur(image, 4);
    save_image(image, "gaussian_blur_");

    return copy_image(image);
    // adaptive_threshold(image);
//    apply_threshold(image, otsu(image));

    // Find the corners of the grid
 //   Square corners = main_blob(image);

    // Resize, rotate and correct perspective
  //  Image final_grid = homographic_transform(image, &corners, 756);

   // save_image(image, "save_");

   // return final_grid;
}

// int main(int argc, char **argv)
//{
//     if (argc == 2)
//     {
//         // Import image
//         SDL_Surface *surface = IMG_Load(argv[1]);
//         printf("Imported image of size %ix%i\n", surface->w, surface->h);
//
//         Image tmp_image = create_image(surface, surface->w, surface->h);
//
//         // Create the name to save image
//         tmp_image.path = (char *)calloc(strlen(argv[1]) + 5, sizeof(char));
//         tmp_image.path[0] = 'r';
//         tmp_image.path[1] = 'e';
//         tmp_image.path[2] = 's';
//         tmp_image.path[3] = '_';
//         strcat(tmp_image.path, argv[1]);
//
//         // Resize the image and free the others
//         Image image = resize_image(&tmp_image, 800);
//         printf("new h = %i , new w = %i\n", image.height, image.width);
//
//         free_image(&tmp_image);
//         SDL_FreeSurface(surface);
//
//         SDL_Quit();
//     }
// }
