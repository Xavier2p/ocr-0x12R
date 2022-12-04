#ifndef LAUNCHER_H
#define LAUNCHER_H

#define STEPS 3


// FUNCTIONS
char* launcher_grayscale();
char* launcher_contrast();
char* launcher_normalize();

// Array of pointers to functions
char* (*steps[STEPS])() = { launcher_grayscale, launcher_contrast, launcher_normalize };

#endif
