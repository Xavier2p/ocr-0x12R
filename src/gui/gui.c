#include "includes/gui.h"
#include "includes/main.h"
#include "includes/launcher.h"

GtkWidget* window = NULL;
GtkWidget* button_Load = NULL;
GtkWidget* button_Save = NULL;
GtkWidget* button_Quit = NULL;
GtkBuilder* builder = NULL;
GtkWidget* fixed_one = NULL;
GtkLabel* label_one = NULL;
GtkWidget* image_one = NULL;
GtkWidget* button_Launch = NULL;
GtkCheckButton* custom_params = NULL;
GtkWidget* dialog = NULL;
GError* error = NULL;
gchar* filename_ui = NULL;
char* filename_image = NULL;
int pc = 0;

// params for custom train of NN
GtkEntry* param1 = NULL;
GtkEntry* param2 = NULL;
GtkEntry* param3 = NULL;


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
    dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog_settings"));
    custom_params =
        GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "custom_nn"));

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
    filename_image = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(button));
    gtk_label_set_text(GTK_LABEL(label_one),
                       (const gchar *)"Auto Resizing the image...");
    SDL_Surface *surface = IMG_Load(filename_image);
    image = create_image(surface, surface->w, surface->h);
    printf("Image loaded: %s\n", filename_image);
    change_image_on_gui(&image, "main_image");
    pc = 0;
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
    g_object_unref(G_OBJECT(builder));
    gtk_main_quit();
}

/**
 * @brief This function is called when the user click on the button "Resize"
 */
void on_button_Resize_clicked()
{
    gtk_label_set_text(GTK_LABEL(label_one), (const gchar *)"Button Resize");
}

void on_button_Next_clicked()
{
    if (pc < STEPS)
    {
        char* verbose = steps[pc]();
        pc++;
        change_image_on_gui(&image, "main_image");
        gtk_label_set_text(GTK_LABEL(label_one), (const gchar *) verbose);
    }

    else
    {
        gtk_label_set_text(GTK_LABEL(label_one), (const gchar *)"END");
    }
}

void on_button_appply_settings_clicked()
{
    int* nb_layers = malloc(sizeof(int));
    int* nb_neuron = malloc(sizeof(int));
    int* learningr = malloc(sizeof(int));
    sscanf((char*) gtk_entry_get_text(param1), "%d", nb_layers);
    sscanf((char*) gtk_entry_get_text(param2), "%d", nb_neuron);
    sscanf((char*) gtk_entry_get_text(param3), "%d", learningr);
    printf("training network....................................ok\n");
    printf("nb_layers = %d\nnb_neuron = %d\nlearningr = %d\n", *nb_layers,
            *nb_neuron, *learningr);
    gtk_widget_destroy(dialog);
}

/**
 * @brief  This function is called when the user click on the button "cancel"
 */
void on_button_cancel_settings_clicked()
{
    gtk_widget_hide(dialog);
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
void on_custom_nn_toggled()
{
    param1 = GTK_ENTRY(gtk_builder_get_object(builder, "entry-param1"));
    param2 = GTK_ENTRY(gtk_builder_get_object(builder, "entry-param2"));
    param3 = GTK_ENTRY(gtk_builder_get_object(builder, "entry-param3"));
    gtk_label_set_text(GTK_LABEL(label_one), (const gchar *)"Button Settings");
    gtk_window_set_transient_for (GTK_WINDOW(dialog), GTK_WINDOW(window));
    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

/**
 * @brief This function is called when the user click on the button "Launch"
 */
void on_button_Launch_clicked()
{
    gtk_label_set_text(GTK_LABEL(label_one),
                       (const gchar *)"OCR in progress...");
    // solve_all();
}

