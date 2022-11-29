#ifndef GUI_H
#define GUI_H
#include <stdlib.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define UI_FILE "assets/gui.xml"
#define CSS_FILE "assets/gui.css"
#define PATH "ocr-0x12/"
#define LOGO "assets/logo.png"

#include "utilis_image.h"
#include "gui_tools.h"

void init_gui(int argc, char *argv[]);
GtkBuilder* builder;

#endif
