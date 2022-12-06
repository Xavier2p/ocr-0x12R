#ifndef GUI_H
#define GUI_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

#define UI_FILE "src/gui/assets/gui.xml"
#define CSS_FILE "src/gui/assets/gui.css"
#define PATH "ocr-0x12/"
#define LOGO "src/gui/assets/logo.png"

#include "gui_tools.h"
#include "../../image_traitment/include/preprocess.h"

void init_gui(int argc, char* argv[]);

#endif
