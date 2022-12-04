#ifndef LAUNCHER_H
#define LAUNCHER_H

#define STEPS 8

// FUNCTIONS
char *launcher_grayscale();
char *launcher_contrast();
char *launcher_normalize();
char *launcher_threshold();
char *launcher_canny();
char *launcher_blob();
char *launcher_dilatation();
char *launcher_erosion();

// Array of pointers to functions
char *(*steps[STEPS])() = { launcher_grayscale,  launcher_contrast,
                            launcher_normalize,  launcher_threshold,
                            launcher_canny,      launcher_blob,
                            launcher_dilatation, launcher_erosion };

#endif
