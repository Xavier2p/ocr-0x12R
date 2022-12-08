#include "include/training.h"
#include <err.h>

//----- HELPER FUNCTIONS -----//

double train_image[NUM_TRAIN][SIZE] = { 0 };
double test_image[NUM_TEST][SIZE] = { 0 };
int train_label[NUM_TRAIN];
int test_label[NUM_TEST];

void remove_cursor(int disable)
{
    printf("%s", disable ? "\033[?25l" : "\033[?25h");
}

//----- STATE 0 OR 1 (TEST/TRAIN) -----//

unsigned int fetch_result(Network* network)
{
    Layer* output_layer = &(network->layers[network->nb_layers - 1]);
    unsigned int result = 0;

    for (unsigned int i = 0; i < output_layer->nb_neurons; i++)
        result = (output_layer->neurons[i].value
                          >= output_layer->neurons[result].value
                      ? i
                      : result);

    return result;
}

void train(unsigned int nb_hidden, unsigned int nb_neurons,
           double learning_rate)
{
    Network n;
    Network* network = &n;
    *network = new_network(784, nb_hidden, nb_neurons, 10);

    double desired_output[network->size_output];
    for (unsigned int epoch = 0; epoch < 10; epoch++)
    {
        for (unsigned int curr_NUM = 0; curr_NUM < NUM_TRAIN; curr_NUM++)
        {
            printf("Training neural network (%d / %d) (%d / %d)... Please "
                   "wait...\r",
                   curr_NUM + 1, NUM_TRAIN, epoch + 1, 10);

            front_propagation(network, train_image[curr_NUM], 0);
            int_to_array(train_label[curr_NUM], desired_output,
                         network->size_output);
            back_propagation(network, desired_output);
            gradient_descent(network, learning_rate);
        }
    }

    save_weights(network, "src/neural_network/saved_data/weights.data");
    free_network(network);
}

int test(Network* network, double clean_input[])
{
    int success_rate = 0;

    front_propagation(network, clean_input, 1);
    for (unsigned int curr_NUM = 0; curr_NUM < NUM_TEST; curr_NUM++)
    {
        printf(
            "Running through tests (%d / %d)... Please wait...            \r",
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

int is_empty(double image[])
{
    int median = 0;

    for (unsigned int i = 0; i < 784; i++)
        median += image[i];

    return median < 20;
}

int run(Network* network, double image[])
{
    if (is_empty(image))
        return 0;

    front_propagation(network, image, 0);
    return fetch_result(network);
}

int training(double nb_hidden, double nb_neurons, double learning_rate,
             double image[], int state)
{
    Network n;
    Network* network = &n;

    if (state == 2)
    {
        load_weights(network, 0);
        return run(network, image);
    }
    else if (state == 0 || state == 1)
    {
        double clean_input[784] = { 0.0 };
        load_numeric(train_image, test_image, train_label, test_label);

        remove_cursor(1);

        if (state == 0)
            load_weights(network, 1);
        if (state == 1)
        {
            train(nb_hidden, nb_neurons, learning_rate);
            load_weights(network, 1);
        }

        int success_rate = test(network, clean_input);
        int success_percent = success_rate * 100 / NUM_TEST;
        printf(
            "Success rate: (%d / %d) -> %d%%.                              \n",
            success_rate, NUM_TEST, success_percent);

        remove_cursor(0);
        return success_percent;
    }
    else
    {
        err(1, "NN: wrong arguments!");
        return 0;
    }
}
