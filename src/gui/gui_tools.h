#ifndef GUI_TOOLS_H
#define GUI_TOOLS_H

#include <stdlib.h>
#include "gui.h"
#include <stdio.h>
#include <gtk/gtk.h>
#include "utilis_image.h"


GdkPixbuf* convert_image_to_gui(Image* image);

void set_image_to_gui(GdkPixbuf* pixbuf, char* GtkimageID);

void change_image_on_gui(Image *_image, char *GtkimageID);

#endif
