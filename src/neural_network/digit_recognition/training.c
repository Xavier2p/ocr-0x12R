/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  bro
 *
 *        Version:  1.0
 *        Created:  10/31/2022 11:08:17 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Arthur GUELENNOC
 *   Organization:  FACH'OCR
 *
 * =====================================================================================
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <err.h>
#include "../../../include/neural_network/neural_network.h"
#include "../../../include/neural_network/save_load.h"
#include "../../../include/neural_network/load_numeric.h"

//----- HELPER FUNCTIONS -----//

void remove_cursor(int disable)
{
    printf("%s", disable ? "\e[?25l" : "\e[?25h");
}

void print_usage()
{
    printf("Usage: neural -p <path> | -h <nb_hidden> -n <nb_neurons> -l <nb_layers> | -i <image>");
    exit(2);
}

//----- STATE 0 OR 1 (TEST/TRAIN) -----//

unsigned int fetch_result(Network *network)
{
    Layer *output_layer = &(network->layers[network->nb_layers - 1]);
    unsigned int result = 0;

    for (unsigned int i = 0; i < output_layer->nb_neurons; i++)
        result = (output_layer->neurons[i].value
                          >= output_layer->neurons[result].value
                      ? i
                      : result);

    return result;
}

void train(unsigned int nb_hidden, unsigned int nb_neurons, double learning_rate)
{
    Network n;
    Network *network = &n;
    *network = new_network(784, nb_hidden, nb_neurons, 10);

    double desired_output[network->size_output];
    for (unsigned int epoch = 0; epoch < 5; epoch++)
    {
        for (unsigned int curr_NUM = 0; curr_NUM < NUM_TRAIN; curr_NUM++)
        {
            printf("Training neural network (%d / %d) (%d / %d)... Please wait...\r",
                    curr_NUM + 1, NUM_TRAIN, epoch + 1, 5);

            front_propagation(network, train_image[curr_NUM], 0);
            int_to_array(train_label[curr_NUM], desired_output, network->size_output);
            back_propagation(network, desired_output);
            gradient_descent(network, learning_rate);
        }
    }

    save_weights(network, "./saved_data/weights.data");
    free_network(network);
}

int test(Network *network, double clean_input[])
{
    int success_rate = 0;

    front_propagation(network, clean_input, 1);
    for (unsigned int curr_NUM = 0; curr_NUM < NUM_TEST; curr_NUM++)
    {
        printf("Running through tests (%d / %d)... Please wait...            \r",
                curr_NUM + 1, NUM_TEST);

        front_propagation(network, test_image[curr_NUM], 0);

        int result = fetch_result(network);
        if (result == test_label[curr_NUM])
            success_rate += 1;
    }

    free_network(network);
    return success_rate;
}

//----- STATE 2 (RUN) -----//

int run(Network *network, double image[])
{
    front_propagation(network, image, 0);
    return fetch_result(network);
}

int training(char path[], double nb_hidden, double nb_neurons, double learning_rate, double image[], int state)
{
    Network n;
    Network *network = &n;

    load_numeric();
    double clean_input[784] = {0.0};

    if (state == 2 && path == NULL)
    {
        load_weights(network, "./saved_data/weights.data");
        return run(network, image);
    }
    else if (state == 0 || state == 1)
    {
        if (((state == 0) && path != NULL) || ((state == 1) && (nb_hidden == 0 ||
                    nb_neurons == 0 || learning_rate == 0)))
            print_usage();

        remove_cursor(1);

        if (state == 0)
            load_weights(network, path);
        if (state == 1)
        {
            train(nb_hidden, nb_neurons, learning_rate);
            load_weights(network, "./saved_data/weights.data");
        }

        int success_rate = test(network, clean_input);
        int success_percent = success_rate * 100 / NUM_TEST;
        printf("Success rate: (%d / %d) -> %d%%.                              \n",
                success_rate, NUM_TEST, success_percent);

        remove_cursor(0);
        return success_percent;

    }
    else
    {
        print_usage();
        return 0;
    }
}
