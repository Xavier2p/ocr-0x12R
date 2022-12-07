#include "includes/gui.h"

#include "includes/launcher.h"

Image image;
GtkWidget* window = NULL;
GtkWidget* button_Load = NULL;
GtkWidget* button_Quit = NULL;
GtkBuilder* builder = NULL;
GtkWidget* fixed_one = NULL;
GtkLabel* label_one = NULL;
GtkWidget* button_Launch = NULL;
GtkButton* custom_params = NULL;
GtkButton* button_save = NULL;
GtkWidget* dialog = NULL;
GError* error = NULL;
gchar* filename_ui = NULL;
char* filename_image = NULL;
int pc = 0;
int selected = 0;

// params for custom train of NN
GtkEntry* param1 = NULL;
GtkEntry* param2 = NULL;
GtkEntry* param3 = NULL;

/**
 * @brief  initialize the GUI
 * @param argc: number of arguments
 * @param argv[]: arguments
 */
void init_gui(int argc, char* argv[])
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
    button_Quit = GTK_WIDGET(gtk_builder_get_object(builder, "button_Quit"));
    button_Launch =
        GTK_WIDGET(gtk_builder_get_object(builder, "button_Launch"));
    label_one = GTK_LABEL(gtk_builder_get_object(builder, "label_one"));
    dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog_settings"));
    custom_params = GTK_BUTTON(gtk_builder_get_object(builder, "custom_nn"));
    button_save = GTK_BUTTON(gtk_builder_get_object(builder, "save-button"));

    // Load CSS
    GtkCssProvider* cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, CSS_FILE, NULL);

    // Inject CSS
    GdkScreen* screen = gdk_screen_get_default();
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
void file_select(GtkFileChooserButton* button)
{
    filename_image = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(button));
    gtk_label_set_text(GTK_LABEL(label_one), (const gchar*)"Loaded image");
    SDL_Surface* surface = IMG_Load(filename_image);
    image = create_image(surface, surface->w, surface->h);
    image.path = malloc(1);
    image.path[0] = '\0';
    printf("Image loaded: %s\n", filename_image);
    change_image_on_gui(&image, "main_image", builder);
    pc = 0;
    selected = 1;
}

/**
 * @brief This function is called when the user click on the button "Quit"
 */
void on_button_Quit_clicked()
{
    free_image(&image);
    gtk_label_set_text(GTK_LABEL(label_one), (const gchar*)"Button Quit");
    g_object_unref(G_OBJECT(builder));
    gtk_main_quit();
}

void on_button_Next_clicked()
{
    if (selected == 0)
    {
        gtk_label_set_text(GTK_LABEL(label_one),
                           (const gchar*)"Please select an image first");
        return;
    }

    if (pc < STEPS)
    {
        char* verbose = steps[pc](&image);
        pc++;
        change_image_on_gui(&image, "main_image", builder);
        gtk_label_set_text(GTK_LABEL(label_one), (const gchar*)verbose);
        printf("[OCR IN PROGRESS] -> %s\n", verbose);
    }

    else
        gtk_label_set_text(GTK_LABEL(label_one), (const gchar*)"END");
}

void on_save_button_clicked()
{
    gtk_label_set_text(GTK_LABEL(label_one), (const gchar*)"Saved image");
    save_image(&image, "saved.jpeg");
    printf("Image saved\n");
}

void on_button_appply_settings_clicked()
{
    int* nb_layers = calloc(1, sizeof(int));
    int* nb_neuron = calloc(1, sizeof(int));
    sscanf((char*)gtk_entry_get_text(param1), "%d", nb_layers);
    sscanf((char*)gtk_entry_get_text(param2), "%d", nb_neuron);
    double learningr = char_to_double((char*)gtk_entry_get_text(param3));
    gtk_widget_destroy(dialog);
    gtk_label_set_text(GTK_LABEL(label_one),
                       (const gchar*)"Settings applied, the Neural Network "
                                     "will be retrained");
    printf("training network....................................ok\n");
    printf("nb_layers = %d\nnb_neuron = %d\nlearningr = %f\n", *nb_layers,
           *nb_neuron, learningr);
    launcher_train((double)*nb_layers, (double)*nb_neuron, learningr);
    gtk_label_set_text(GTK_LABEL(label_one),
                       (const gchar*)"Neural Network retrained");
    free(nb_layers);
    free(nb_neuron);
}

/**
 * @brief  This function is called when the user click on the button "cancel"
 */
void on_button_cancel_settings_clicked()
{
    gtk_widget_hide(dialog);
}

/**
 * @brief This function is called when the user click on the button "Settings"
 */
void on_custom_nn_clicked()
{
    param1 = GTK_ENTRY(gtk_builder_get_object(builder, "entry-param1"));
    param2 = GTK_ENTRY(gtk_builder_get_object(builder, "entry-param2"));
    param3 = GTK_ENTRY(gtk_builder_get_object(builder, "entry-param3"));
    gtk_label_set_text(GTK_LABEL(label_one),
                       (const gchar*)"Choose your parameters");
    gtk_window_set_transient_for(GTK_WINDOW(dialog), GTK_WINDOW(window));
    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

/**
 * @brief This function is called when the user click on the button "Launch"
 */
void on_button_Launch_clicked()
{
    if (selected == 0)
    {
        gtk_label_set_text(GTK_LABEL(label_one),
                           (const gchar*)"Please select an image first");
        return;
    }

    else if (pc < STEPS)
    {
        gtk_label_set_text(GTK_LABEL(label_one),
                           (const gchar*)"OCR in progress...");

        for (int i = pc; i < STEPS; i++)
        {
            char* verbose = steps[i](&image);
            printf("[OCR IN PROGRESS] -> %s\n", verbose);
            pc = i;
        }

        change_image_on_gui(&image, "main_image", builder);
        gtk_label_set_text(GTK_LABEL(label_one), "Solved sudoku");
        save_image(&image, "solved.jpeg");
    }
}
