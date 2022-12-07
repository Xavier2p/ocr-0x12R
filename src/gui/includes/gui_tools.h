#ifndef GUI_TOOLS_H
#define GUI_TOOLS_H

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

#include "gui.h"
#include "../../image_traitment/include/utilis_image.h"

GdkPixbuf* convert_image_to_gui(Image* image);

void set_image_to_gui(GdkPixbuf* pixbuf, char* GtkimageID, GtkBuilder* builder);

void change_image_on_gui(Image* _image, char* GtkimageID, GtkBuilder* builder);

double char_to_double(char* str);

#endif
