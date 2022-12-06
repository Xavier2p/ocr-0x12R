#ifndef LAUNCHER_H
#define LAUNCHER_H

#define STEPS 3

// FUNCTIONS
char* launcher_grayscale(Image* image);
char* launcher_contrast(Image* image);
char* launcher_normalize(Image* image);
// char *launcher_threshold(Image *image);
// char *launcher_canny(Image *image);
// char *launcher_blob(Image *image);
// char *launcher_dilatation(Image *image);
// char *launcher_erosion(Image *image);
//
//  Array of pointers to functions
char* (*steps[STEPS])(Image* image) = { launcher_grayscale, launcher_contrast,
                                        launcher_normalize };
// launcher_threshold,
//                            launcher_canny,      launcher_blob,
//                            launcher_dilatation, launcher_erosion };

#endif
