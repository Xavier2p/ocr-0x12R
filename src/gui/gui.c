#include "includes/gui.h"

#include "includes/launcher.h"

/**
 * @brief define the variables
 */
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
GtkButton* button_edit = NULL;
GtkButton* button_next = NULL;
GtkWidget* dialog = NULL;
GtkWidget* edit_dialog = NULL;
GError* error = NULL;
gchar* filename_ui = NULL;
char* filename_image = NULL;
int pc = 0;
int selected = 0;

// params for custom train of NN
GtkEntry* param1 = NULL;
GtkEntry* param2 = NULL;
GtkEntry* param3 = NULL;

// params for custom edit of image
GtkEntry* nb_column = NULL;
GtkEntry* nb_row = NULL;
GtkEntry* new_value = NULL;

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
    edit_dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog-edit"));
    button_edit = GTK_BUTTON(gtk_builder_get_object(builder, "button-edit"));
    button_next = GTK_BUTTON(gtk_builder_get_object(builder, "button-next"));

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
    gtk_widget_hide(GTK_WIDGET(button_edit));
    gtk_widget_hide(edit_dialog);
    gtk_widget_show(GTK_WIDGET(button_save));
}

/**
 * These are the callbacks for the buttons at left
 * @brief on_save_button_clicked => save the image
 * @brief on_button_Quit_clicked => quit the program
 */
void on_save_button_clicked()
{
    gtk_label_set_text(GTK_LABEL(label_one), (const gchar*)"Saved image");
    save_image(&image, "saved.jpeg");
    printf("Image saved\n");
}
void on_button_Quit_clicked()
{
    free_image(&image);
    gtk_label_set_text(GTK_LABEL(label_one), (const gchar*)"Button Quit");
    g_object_unref(G_OBJECT(builder));
    gtk_main_quit();
}

/**
 * Functions for the custom parameters for NN
 * @brief on_custom_nn_clicked => open the dialog
 * @brief on_button_cancel_settings_clicked => close the dialog
 * @brief on_button_apply_settings_clicked => apply the parameters and retrain
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
    gtk_widget_hide(dialog);
}
void on_button_cancel_settings_clicked()
{
    gtk_widget_hide(dialog);
}
void on_button_apply_settings_clicked()
{
    int* nb_layers = calloc(1, sizeof(int));
    int* nb_neuron = calloc(1, sizeof(int));
    sscanf((char*)gtk_entry_get_text(param1), "%d", nb_layers);
    sscanf((char*)gtk_entry_get_text(param2), "%d", nb_neuron);
    double learningr = char_to_double((char*)gtk_entry_get_text(param3));
    gtk_widget_hide(dialog);
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
 * Functions for the edit dialog
 * @brief on_button_edit_clicked => open the dialog
 * @brief on_button_cancel_edit_clicked => close the dialog
 * @brief on_button_apply_edit_clicked => apply the parameters and train
 */
void on_button_edit_clicked()
{
    nb_column = GTK_ENTRY(gtk_builder_get_object(builder, "entry-col"));
    nb_row = GTK_ENTRY(gtk_builder_get_object(builder, "entry-line"));
    new_value = GTK_ENTRY(gtk_builder_get_object(builder, "entry-val"));
    gtk_label_set_text(GTK_LABEL(label_one), (const gchar*)"Editing the image");
    gtk_window_set_transient_for(GTK_WINDOW(edit_dialog), GTK_WINDOW(window));
    gtk_widget_show_all(edit_dialog);
    Image* prim_image = get_first_image();
    change_image_on_gui(prim_image, "primary-image", builder);
    gtk_dialog_run(GTK_DIALOG(edit_dialog));
    gtk_widget_hide(edit_dialog);
}
void on_button_cancel_edit_clicked()
{
    gtk_widget_hide(edit_dialog);
}
void on_button_apply_edits_clicked()
{
    int* col = calloc(1, sizeof(int));
    int* line = calloc(1, sizeof(int));
    int* val = calloc(1, sizeof(int));

    sscanf((char*)gtk_entry_get_text(nb_column), "%d", col);
    sscanf((char*)gtk_entry_get_text(nb_row), "%d", line);
    sscanf((char*)gtk_entry_get_text(new_value), "%d", val);
    gtk_label_set_text(GTK_LABEL(label_one), (const gchar*)"Edited Image");

    if (*col > 0 && *col <= 9 && *line > 0 && *line <= 9 && *val <= 9)
        set_new_number(&image, *col - 1, *line - 1, *val);

    gtk_entry_set_text(nb_column, "");
    gtk_entry_set_text(nb_row, "");
    gtk_entry_set_text(new_value, "");

    change_image_on_gui(&image, "main_image", builder);

    free(col);
    free(line);
    free(val);
}
void on_button_ok_edits_clicked()
{
    gtk_widget_hide(edit_dialog);
}

/**
 * These are the runner functions
 * @brief on_button_Next_clicked => run the next step
 * @brief on_button_Launch_clicked => run all the remaining steps
 */
void on_button_Next_clicked()
{
    if (selected == 0)
    {
        gtk_label_set_text(GTK_LABEL(label_one),
                           (const gchar*)"Please select an image first");
        return;
    }

    if (pc == STEP_BEFORE - 1)
        gtk_widget_show(GTK_WIDGET(button_edit));

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
void on_button_Launch_clicked()
{
    if (selected == 0)
    {
        gtk_label_set_text(GTK_LABEL(label_one),
                           (const gchar*)"Please select an image first");
        return;
    }

    if (pc < STEPS)
    {
        gtk_label_set_text(GTK_LABEL(label_one),
                           (const gchar*)"OCR in progress...");

        for (; pc < STEPS; pc++)
        {
            char* verbose = steps[pc](&image);
            printf("[OCR IN PROGRESS] -> %s\n", verbose);
            if (pc == STEP_BEFORE - 1)
            {
                pc++;
                break;
            }
        }

        change_image_on_gui(&image, "main_image", builder);
        gtk_label_set_text(
            GTK_LABEL(label_one),
            pc == STEP_BEFORE
                ? "Please check the grid\nThen, press [Next] or [Launch]"
                : "Solved sudoku");
        gtk_widget_show(GTK_WIDGET(button_edit));

        if (pc == STEPS - 1)
            save_image(&image, "solved.jpeg");
    }
}
