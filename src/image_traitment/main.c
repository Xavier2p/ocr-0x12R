#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "include/linkedlist.h"
#include "include/otsu.h"
#include "include/preprocess.h"
#include "include/utilis_image.h"
#include "include/gaussian_filter.h"
#include "include/grid_detection.h"
#include "include/hough_transform.h"
#include "include/blob.h"
#include "include/canny.h"
#include "include/homographic_transform.h"
#include "include/queue.h"
#include "include/adaptive_treshold.h"
#include "include/segmentation.h"

void compute_hough(Image *image)
{
    Image draw_image_hough = copy_image(image);
    int hough_threshold =
        image->width > image->height ? image->width / 4 : image->height / 4;

    MyList lines = hough_transform(image, hough_threshold);
    MyList simplified_lines = simplify_lines(&lines, 40);

    for (size_t i = 0; i < simplified_lines.length; ++i)
    {
        Line *l = get_value(&simplified_lines, i);
        draw_line(&draw_image_hough, l);
    }

    free_list(&lines);
    free_list(&simplified_lines);
    save_image(&draw_image_hough, "res_hough_");
    free_image(&draw_image_hough);
}

int main(int argc, char **argv)
{
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    // Import image
    SDL_Surface *surface = IMG_Load(argv[1]);
    printf("Imported image of size %ix%i\n", surface->w, surface->h);

    Image tmp_image = create_image(surface, surface->w, surface->h);

    // Create the name to save image
    tmp_image.path = (char *)calloc(strlen(argv[1]) + 5, sizeof(char));
    tmp_image.path[0] = 'r';
    tmp_image.path[1] = 'e';
    tmp_image.path[2] = 's';
    tmp_image.path[3] = '_';
    strcat(tmp_image.path, argv[1]);

    // Resize the image and free the others
    Image image = resize_image(&tmp_image, 800);
    printf("new h = %i , new w = %i\n", image.height, image.width);

    free_image(&tmp_image);
    SDL_FreeSurface(surface);

    // Preprocess
    surface_to_grayscale(&image);
    // save_image(&image, "gray_scale_");
    image_contrast(&image, 12);
    // save_image(&image, "contrast_");
    image_normalize_brightness(&image);
    // save_image(&image, "brightness_");
    gaussian_blur(&image, 4);
    // save_image(&image, "gaussian_blur_");
    // adaptive_threshold(&image);
    apply_threshold(&image, otsu(&image));

    Square corners = main_blob(&image);

    Image final_grid = homographic_transform(&image, &corners, 756);

    segmentation(&final_grid);

    save_image(&final_grid, "contrast_");

    // canny_edge_detection(&image);
    // compute_blob(&image);
    // compute_hough(&image);

    // save_image(&image, "");
    free_image(&image);
    free_image(&final_grid);

    SDL_Quit();

    return EXIT_SUCCESS;
}
