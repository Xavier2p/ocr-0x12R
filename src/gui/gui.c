/*
 * ============================================================================
 *
 *       Filename:  gui.c
 *
 *    Description: GUI for the OCR.
 *
 *        Version:  1.0
 *        Created:  10/03/2022 19:54:10
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Xavier de Place
 *   Organization:  FACHOCR
 *
 * ============================================================================
 */
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <gtk/gtk.h>
#include <stdlib.h>

#include "resize.h"
#include "utilis_image.h"

#define UI_FILE "assets/gui.xml"
#define CSS_FILE "assets/gui.css"
#define PATH "ocr-0x12/"
#define LOGO "assets/fachocr.jpg"

GtkWidget *window = NULL;
GtkWidget *button_Load = NULL;
GtkWidget *button_Save = NULL;
GtkWidget *button_Quit = NULL;
GtkBuilder *builder = NULL;
GtkWidget *fixed_one = NULL;
GtkLabel *label_one = NULL;
GtkWidget *image_one = NULL;
GtkWidget *button_Launch = NULL;
GError *error = NULL;
gchar *filename_ui = NULL;
char *filename_image = NULL;

/**
 * @brief  initialize the GUI
 * @param argc: number of arguments
 * @param argv[]: arguments
 * @return 0 if success, -1 if error
 */
void init_gui(int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    filename_ui = g_build_filename(UI_FILE, NULL);

    gtk_builder_add_from_file(builder, filename_ui, &error);
    g_free(filename_ui);
    if (error)
    {
        gint code = error->code;
        g_printerr("%i: %s\n", code, error->message);
        g_error_free(error);
    }

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_builder_connect_signals(builder, NULL);

    fixed_one = GTK_WIDGET(gtk_builder_get_object(builder, "fixed_one"));
    button_Load = GTK_WIDGET(gtk_builder_get_object(builder, "LoadImage"));
    button_Save = GTK_WIDGET(gtk_builder_get_object(builder, "button_Save"));
    button_Quit = GTK_WIDGET(gtk_builder_get_object(builder, "button_Quit"));
    button_Launch =
        GTK_WIDGET(gtk_builder_get_object(builder, "button_Launch"));
    label_one = GTK_LABEL(gtk_builder_get_object(builder, "label_one"));
    image_one = GTK_WIDGET(gtk_builder_get_object(builder, "image_one"));

    // Load CSS
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, CSS_FILE, NULL);

    // Inject CSS
    GdkScreen *screen = gdk_screen_get_default();
    gtk_style_context_add_provider_for_screen(screen,
                                              GTK_STYLE_PROVIDER(cssProvider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);

    gtk_widget_show(window);
    gtk_main();
}

/**
 * @brief  load an image
 * @param button: widget
 * @return void
 */
void file_select(GtkFileChooserButton *button)
{
    // This function allow the user to choose a file to compute, with an UI.
    // Goal => put the filename in a ptr to load image with SDL.
    // remove all the path before PATH included
    printf("In the function\n");
    filename_image = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(button));
    printf("%s\n", filename_image);
    // char* file_w_path = strstr(filename_image, PATH);
    //  file_w_path += strlen(PATH);

    // add "loaded file:" before the filename
    // char *label = malloc(strlen(file_w_path) + 13);
    // strcpy(label, "Loaded file: ");
    // strcat(label, file_w_path);
    // set label to see what is the filename
    gtk_label_set_text(label_one, filename_image);
    // free(label);
}

/**
 * @brief This function is called when the user click on the button "Save"
 */
void on_button_Save_clicked()
{
    gtk_label_set_text(GTK_LABEL(label_one), (const gchar *)"Button Save");
}

/**
 * @brief This function is called when the user click on the button "Quit"
 */
void on_button_Quit_clicked()
{
    gtk_label_set_text(GTK_LABEL(label_one), (const gchar *)"Button Quit");
    gtk_main_quit();
}

/**
 * @brief This function is called when the user click on the button "Resize"
 */
void on_button_Resize_clicked()
{
    gtk_label_set_text(GTK_LABEL(label_one),
                       (const gchar *)"Auto Resizing the image...");
    // Import image
    SDL_Surface *surface = IMG_Load(filename_image);
    printf("Image loaded\n");
    // Resize image
    SDL_Surface *resized = resize_surface(surface, 300, 300);
    free(surface);
    printf("Image resized\n");
    // create the name of the resized image
    char *filename_resized = malloc(strlen(filename_image) + 14);
    strcpy(filename_resized, filename_image);
    strcat(filename_resized, "_resized.png");
    // save image
    IMG_SavePNG(resized, filename_resized);
    free(resized);
    // set label to notify the user
    gtk_label_set_text(label_one,
                       (const gchar *)"Image resized!\nPress Launch to see it");
    filename_image = filename_resized;
}

/**
 * @brief This function is called when the user click on the button "Rotate"
 */
void on_button_Rotate_clicked()
{
    gtk_label_set_text(GTK_LABEL(label_one), (const gchar *)"Button Rotate");
}

/**
 * @brief This function is called when the user click on the button "Settings"
 */
void on_button_Settings_clicked()
{
    gtk_label_set_text(GTK_LABEL(label_one), (const gchar *)"Button Settings");
}

/**
 * @brief This function is called when the user click on the button "Launch"
 */
void on_button_Launch_clicked()
{
    // set label to see what we are on the program
    gtk_label_set_text(GTK_LABEL(label_one),
                       (const gchar *)"OCR in progress...");

    // display the image
    int x = 300, y = 10;
    if (image_one)
        gtk_container_remove(GTK_CONTAINER(fixed_one), image_one);
    // load image
    image_one = gtk_image_new_from_file(filename_image);
    // create the widget to show image
    gtk_widget_show(image_one);
    // add the widget to the container
    gtk_container_add(GTK_CONTAINER(fixed_one), image_one);
    // set the position of the widget
    gtk_fixed_move(GTK_FIXED(fixed_one), image_one, x, y);
}

/**
 * @brief Temporary main function to test the GUI
 */
int main(int argc, char *argv[])
{
    init_gui(argc, argv);
    return EXIT_SUCCESS;
}
