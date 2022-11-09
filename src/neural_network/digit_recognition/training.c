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
#include "../include/neural_network.h"
#include "../include/save_load.h"
#include "../include/mnist.h"


//----- MAIN -----//


unsigned int fetch_result(Network *network)
{
    Layer *output_layer = &(network->layers[network->nb_layers - 1]);
    unsigned int  result = 0;

    for (unsigned int i = 0; i < output_layer->nb_neurons; i++)
        result = (output_layer->neurons[i].value >= output_layer->neurons[result].value ? i : result);

    return result;
}

void train(unsigned int nb_hidden, unsigned int nb_neurons, double learning_rate)
{
    Network n;
    Network *network = &n;
    *network = new_network(784, nb_hidden, nb_neurons, 10);

    double desired_output[network->size_output];

    for (unsigned int epoch = 0; epoch < NUM_TRAIN; epoch++)
    {
        printf("Training neural network (%d / %d)... Please wait...\r", epoch + 1, NUM_TRAIN);

        front_propagation(network, train_image[epoch], 0);

        int_to_array(train_label[epoch], desired_output, network->size_output);

        back_propagation(network, desired_output);
        gradient_descent(network, learning_rate);
    }

    save_weights(network, "./saved_data/weights.data");

    free_network(network);
}

int test(Network *network)
{
    int success_rate = 0;

    double clean_input[784] = { 0.0 };
    front_propagation(network, clean_input, 1);

    for (unsigned int epoch = 0; epoch < NUM_TEST; epoch++)
    {
        printf("Running through tests (%d / %d)... Please wait...\r", epoch + 1, NUM_TEST);

        front_propagation(network, test_image[epoch], 0);

        int result = fetch_result(network);
        if (result == test_label[epoch])
            success_rate += 1;
    }

    free_network(network);

    return success_rate;
}

int main(int argc, char **argv)
{
    Network n;
    Network *network = &n;

    load_mnist();
    printf("\e[?25l");

    if (argc == 2)
        load_weights(network, argv[1]);
    else if (argc == 4)
    {
        double nb_hidden = atof(argv[1]);
        double nb_neurons = atof(argv[2]);
        double learning_rate = atof(argv[3]);

        train(nb_hidden, nb_neurons, learning_rate);
        load_weights(network, "./saved_data/weights.data");
    }
    else
        errx(EXIT_FAILURE, "Wrong number of arguments. RTFM");

    int success_rate = test(network);

    int success_percent = success_rate * 100 / NUM_TEST;

    printf("Success rate: (%d / %d) -> %d%%.                              \n", 
            success_rate, NUM_TEST, success_percent);

    printf("\e[?25h");

    return success_percent;
}
