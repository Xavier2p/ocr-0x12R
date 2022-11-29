#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../../include/image_traitment/linkedlist.h"
#include "../../include/image_traitment/otsu.h"
#include "../../include/image_traitment/preprocess.h"
#include "../../include/image_traitment/utilis_image.h"
#include "../../include/image_traitment/gaussian_filter.h"
#include "../../include/image_traitment/grid_detection.h"
#include "../../include/image_traitment/hough_transform.h"
#include "../../include/image_traitment/blob.h"
#include "../../include/image_traitment/canny.h"
#include "../../include/image_traitment/homographic_transform.h"
#include "../../include/image_traitment/queue.h"
#include "../../include/image_traitment/adaptive_treshold.h"
#include "../../include/image_traitment/segmentation.h"


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
    //    MyQueue q = {NULL, NULL, 0};
    //
    //    void *p = malloc(sizeof(int));
    //    *(int*)p = 42;
    //    void *p1 = malloc(sizeof(int));
    //    *(int*)p1 = 12;
    //    void *p2 = malloc(sizeof(int));
    //    *(int*)p2 = 2;
    //    void *p3 = malloc(sizeof(int));
    //    *(int*)p3 = 4;
    //    void *p4 = malloc(sizeof(int));
    //    *(int*)p4 = 6;
    //    void *p5 = malloc(sizeof(int));
    //    *(int*)p5 = 9;
    //
    //    enqueue(&q, p);
    //    enqueue(&q, p1);
    //    enqueue(&q, p2);
    //    enqueue(&q, p3);
    //    enqueue(&q, p4);
    //    enqueue(&q, p5);
    //
    //    int *d1 = ((int*)dequeue(&q));
    //    int *d2 = ((int*)dequeue(&q));
    //    int *d3 = ((int*)dequeue(&q));
    //    int *d4 = ((int*)dequeue(&q));
    //    int *d5 = ((int*)dequeue(&q));
    //    printf("deque = %d", *d1);
    //    printf("deque = %d", *d2);
    //    printf("deque = %d", *d3);
    //    printf("deque = %d", *d4);
    //    printf("deque = %d", *d5);
    //
    //    free(d1);
    //    free(d2);
    //    free(d3);
    //    free(d4);
    //    free(d5);
    //
    //
    //    free_queue(&q);
    //
    //    return 0;
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

    //    compute_homographic_transform(&tmp_image);

    // Resize the image and free the others
    Image image = resize_image(&tmp_image, 800);
    free_image(&tmp_image);
    printf("new h = %i , new w = %i\n", image.height, image.width);

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

    main_blob(&image);
    // canny_edge_detection(&image);

    //    compute_blob(&image);
    //    compute_hough(&image);

    save_image(&image, "");
    free_image(&image);

    SDL_Quit();

    return EXIT_SUCCESS;
}
