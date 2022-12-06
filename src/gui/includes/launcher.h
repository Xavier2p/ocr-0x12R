#ifndef LAUNCHER_H
#define LAUNCHER_H

#define STEPS 6

// FUNCTIONS
char* launcher_grayscale(Image* image);
char* launcher_blur(Image* image);
char* launcher_adaptative_threshold(Image* image);
char* launcher_blob(Image* image);
char* launcher_erosion(Image* image);
char* launcher_dilatation(Image* image);

//  Array of pointers to functions
char* (*steps[STEPS])(Image* image) = { launcher_grayscale, launcher_blur, launcher_adaptative_threshold, launcher_blob, launcher_erosion, launcher_dilatation };

#endif
