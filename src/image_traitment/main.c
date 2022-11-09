#include <err.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/image_traitment/linkedlist.h"
#include "../../include/image_traitment/otsu.h"
#include "../../include/image_traitment/preprocess.h"
#include "../../include/image_traitment/utilis_image.h"
#include "../../include/image_traitment/gaussian_filter.h"
#include "../../include/image_traitment/grid_detection.h"
#include "../../include/image_traitment/hough_transform.h"
#include "../../include/image_traitment/blob.h"
#include "../../include/image_traitment/canny.h"

void draw_dot2(Image *image, Dot *dot, int size)
{
    int width = image->width;
    int height = image->height;
    int x = dot->X;
    int y = dot->Y;
    for (int i = -size; i < size; ++i)
    {
        for (int j = -size; j < size; ++j)
        {
            if (x + i >= 0 && x + i < height && j + y >= 0 && j + y < width)
            {
                image->pixels[x + i][y + j].r = 255;
            }
        }
    }
}

void save_image2(Image *image, char *name)
{
    SDL_Surface *final_surface = create_surface(image);
    char *res = malloc(strlen(name) + strlen(image->path) + 1);
    strcpy(res, name);
    strcat(res, image->path);
    SDL_SaveBMP(final_surface, res);
    SDL_FreeSurface(final_surface);
    free(res);
}

int main(int argc, char **argv)
{
    if (argc != 2)
        errx(EXIT_FAILURE, "Usage: image-file");

    // Import image
    SDL_Surface *surface = IMG_Load(argv[1]);
    printf("Imported image of size %ix%i\n", surface->w, surface->h);
    Image tmp_image = create_image(surface, surface->w, surface->h);
    SDL_FreeSurface(surface);

    // Create the name to save image
    tmp_image.path = (char *)calloc(strlen(argv[1]) + 5, sizeof(char));
    tmp_image.path[0] = 'r';
    tmp_image.path[1] = 'e';
    tmp_image.path[2] = 's';
    tmp_image.path[3] = '_';
    strcat(tmp_image.path, argv[1]);

    // Resize the image and free the others
    Image image = resize_image(&tmp_image, 800);
    free_image(&tmp_image);
    printf("new h = %i , new w = %i\n", image.height, image.width);

    // Preprocess
    surface_to_grayscale(&image);
    save_image2(&image, "gray_scale_");
    image_contrast(&image, 12);
    save_image2(&image, "contrast_");
    image_normalize_brightness(&image);
    save_image2(&image, "brightness_");
    gaussian_blur(&image, 4);
    save_image2(&image, "gaussian_blur_");

    canny_edge_detection(&image);

    // Binarisation
//    int otsuthresh = otsu(&image);
//    apply_threshold(&image, otsuthresh);
//    hysteris(&image);

    Image draw_image = copy_image(&image);
    MyList allblob = find_blob(&draw_image);
    printf("nb bolb = %lu\n", allblob.length);
    int max = 0;
    int index = 0;
    for (size_t i = 0; i < allblob.length; ++i)
    {
        Blob *b = get_value(&allblob, i);
        if (b->length > max)
        {
            max = b->length;
            index = i;
        }
    }
    Dot *corner_dot = find_corners((Blob*)get_value(&allblob, index));
    for (int i = 0; i < 4; ++i)
    {
        draw_dot2(&draw_image, &corner_dot[i], 10);
    }
    free(corner_dot);
    free_blob_list(&allblob);

    save_image2(&image, "");
    save_image2(&draw_image, "res_blob_");
    free_image(&image);

    SDL_Quit();

    return EXIT_SUCCESS;
}
