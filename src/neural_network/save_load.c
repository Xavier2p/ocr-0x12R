#include "include/save_load.h"

//----- SAVE & LOAD WEIGHTS -----//

char* my_strncat(char* destination, const char* source, size_t num)
{
    if ((destination == NULL) && (source == NULL))
        return NULL;

    char* dest = destination;

    while (*dest != '\0')
    {
        dest++;
    }

    while (num--)
    {
        if (!(*dest++ = *source++))
        {
            return destination;
        }
    }
    *dest = '\0';

    return destination;
}

int is_number(char text)
{
    return (('0' <= text) && (text <= '9')) ? 1 : 0;
}

void save_weights(Network* network, char* path)
{
    printf("%s\r", "Saving weights... Please wait...");

    FILE* file;
    file = fopen(path, "w");

    char str_header[20];

    if (network->nb_layers - 2 > 0)
        sprintf(str_header, "%d|%d", (int)(network->nb_layers - 2),
                network->layers[1].nb_neurons);
    else
        sprintf(str_header, "0|0");

    fputs(str_header, file);

    for (unsigned int i = 1; i < network->nb_layers; i++)
    {
        fputs("\n#", file);
        Layer* curr_layer = &(network->layers[i]);

        for (unsigned int j = 0; j < curr_layer->nb_neurons; j++)
        {
            fputs("\n~\n", file);
            Neuron* curr_neuron = &(network->layers[i].neurons[j]);

            for (unsigned int k = 0; k < curr_neuron->nb_weights; k++)
            {
                char strtmp[20];
                sprintf(strtmp, "%f", curr_neuron->weights[k]);
                fputs(strtmp, file);
                fputs("|", file);
            }
        }
    }

    fclose(file);
}

void load_weights(Network* network, int path_status)
{
    char* path;
    if (path_status == 0)
        path = "src/neural_network/saved_data/best_weights.data";
    if (path_status == 1)
        path = "src/neural_network/saved_data/weights.data";

    printf("%s\r", "Loading weights file... Please wait..");

    if (access(path, F_OK) != 0)
        errx(EXIT_FAILURE, "Specified file doesn't exist.");

    FILE* file;
    file = fopen(path, "r");

    int nb_layers, nb_neurons;

    char str_nb_layers[50] = { 0 };
    char curr_char = getc(file);

    while (curr_char != EOF && curr_char != '|' && is_number(curr_char) == 1)
    {
        my_strncat(str_nb_layers, &curr_char, 1);
        curr_char = getc(file);
    }
    if (curr_char != EOF)
        nb_layers = atoi(str_nb_layers);
    else
        errx(EXIT_FAILURE, "Specified file incompatible.");

    char str_nb_neurons[50] = { 0 };
    curr_char = getc(file);

    while (curr_char != EOF && curr_char != '\n' && is_number(curr_char) == 1)
    {
        my_strncat(str_nb_neurons, &curr_char, 1);
        curr_char = getc(file);
    }
    if (curr_char != EOF)
        nb_neurons = atoi(str_nb_neurons);
    else
        errx(EXIT_FAILURE, "Specified file incompatible.");

    *network = new_network(784, nb_layers, nb_neurons, 10);

    char strtmp[20] = { 0 };
    int layers_index = 0, neurons_index = -1, weights_index = 0;
    memset(strtmp, 0, sizeof(strtmp));

    while ((curr_char = getc(file)) != EOF)
    {
        switch (curr_char)
        {
        case '#':
            layers_index++;
            neurons_index = -1;
            weights_index = 0;
            break;

        case '~':
            neurons_index++;
            weights_index = 0;
            break;

        case ' ':
        case '\n':
            break;

        case '|':
            network->layers[layers_index]
                .neurons[neurons_index]
                .weights[weights_index] = atof(strtmp);
            memset(strtmp, 0, sizeof(strtmp));
            weights_index++;
            break;

        default:
            strncat(strtmp, &curr_char, 1);
            break;
        }
    }

    fclose(file);
}
