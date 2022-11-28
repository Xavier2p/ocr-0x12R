#include "gui_tools.h"

GdkPixbuf* convert_image_to_gui(Image* image)
{
    GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8,
                                       image->width, image->height);

    int width = gdk_pixbuf_get_width(pixbuf);
    int height = gdk_pixbuf_get_height(pixbuf);

    int n_channels = gdk_pixbuf_get_n_channels(pixbuf);
    int rowstride = gdk_pixbuf_get_rowstride(pixbuf);
    guchar *pixels = gdk_pixbuf_get_pixels(pixbuf);

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            guchar *p = pixels + y * rowstride + x * n_channels;
            p[0] = image->pixels[x][y].r;
            p[1] = image->pixels[x][y].g;
            p[2] = image->pixels[x][y].b;
            p[3] = 255;
        }
    }

    return pixbuf;
}

void set_image_to_gui(GdkPixbuf* pixbuf, char* GtkimageID)
{
    GtkImage *imageWidget =
        GTK_IMAGE(gtk_builder_get_object(builder, GtkimageID)); // get image

    int width = 300; //CLAMP(gtk_widget_get_allocated_width(GTK_WIDGET(panel)), 0,
                    //   1000);
    int height = 300;
        // CLAMP(gtk_widget_get_allocated_height(GTK_WIDGET(panel)), 0, 1000);


    // get image size
    int image_width = gdk_pixbuf_get_width(pixbuf);
    int image_height = gdk_pixbuf_get_height(pixbuf);

    // get scale factor
    double scale_factor = 1;
    if (image_width > width || image_height > height)
    {
        scale_factor = (double)width / image_width;
        if (scale_factor * image_height > height)
            scale_factor = (double)height / image_height;
    }
    int new_width = image_width * scale_factor;
    int new_height = image_height * scale_factor;

    printf("%f %d %d\n", scale_factor, width, height);
    // resize the image
    GdkPixbuf *resized_image = gdk_pixbuf_scale_simple(
        pixbuf, new_width, new_height, GDK_INTERP_BILINEAR); // resize image

    // set the image
    gtk_image_set_from_pixbuf(imageWidget, resized_image);

    // free
    g_object_unref(pixbuf);
    g_object_unref(resized_image);
}

void change_image_on_gui(Image *_image, char *GtkimageID)
{
    GdkPixbuf *pixbuf = convert_image_to_gui(_image);
    set_image_to_gui(pixbuf, GtkimageID);
}
