#ifndef LAUNCHER_H
#define LAUNCHER_H

#define STEPS 9

// FUNCTIONS
char* laucher_resize(Image* image);
char* launcher_grayscale(Image* image);
char* launcher_blur(Image* image);
char* launcher_adaptative_threshold(Image* image);
char* launcher_blob(Image* image);
char* launcher_erosion(Image* image);
char* launcher_dilatation(Image* image);
char* launcher_find_corners(Image* image);
char* launcher_homographic_transform(Image* image);
char* launcher_segmentation(Image* image);

//  Array of pointers to functions
char* (*steps[STEPS])(Image* image) = {
    laucher_resize,        launcher_grayscale,
    launcher_blur,         launcher_adaptative_threshold,
    launcher_blob,         launcher_erosion,
    launcher_find_corners, launcher_homographic_transform,
    launcher_segmentation
};

int launcher_train(double hl, double nbn, double lr);
int** get_origin_grid();
Image *get_first_image();

#endif

