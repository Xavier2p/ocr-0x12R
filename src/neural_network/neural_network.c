#include "include/neural_network.h"

//----- NEURON INITIALIZATION -----//

Neuron new_neuron(unsigned int nb_weights)
{
    Neuron neuron = {
        .nb_weights = nb_weights,
        .weights = NULL,
        .value = 0,
        .delta = 1,
    };

    if (nb_weights != 0)
    {
        neuron.weights = (double*)calloc((nb_weights + 1), sizeof(double));
        if (neuron.weights == NULL)
            errx(EXIT_FAILURE, "Error while allocating memory");
    }

    return neuron;
}

void init_neuron(Neuron* neuron)
{
    unsigned int nb_weights = neuron->nb_weights;

    for (unsigned int i = 0; i < nb_weights; i++)
        neuron->weights[i] = (double)rand() / (double)RAND_MAX;
}

void free_neuron(Neuron* neuron)
{
    free(neuron->weights);
}

//----- LAYER INITIALIZATION -----//

void init_layer(Layer* layer, unsigned int weights)
{
    unsigned int nb_neurons = layer->nb_neurons;

    for (unsigned int i = 0; i < nb_neurons; i++)
        layer->neurons[i] = new_neuron(weights);
}

Layer new_layer(unsigned int size, unsigned int weights)
{
    Layer layer = {
        .nb_neurons = size,
        .neurons = NULL,
    };

    layer.neurons = (Neuron*)calloc((size + 1), sizeof(struct Neuron));
    if (layer.neurons == NULL)
        errx(EXIT_FAILURE, "Error while allocating memory");

    init_layer(&layer, weights);

    return layer;
}

void free_layer(Layer* layer)
{
    for (unsigned int i = 0; i < layer->nb_neurons; i++)
        free_neuron(&(layer->neurons[i]));

    free(layer->neurons);
}

//----- NETWORK INITIALIZATION -----//

void init_weights(Network* network)
{
    srand(time(NULL));

    for (unsigned int i = 0; i < network->nb_layers; i++)
    {
        Layer* curr_layer = &(network->layers[i]);

        for (unsigned int j = 0; j < curr_layer->nb_neurons; j++)
            init_neuron(&(curr_layer->neurons[j]));
    }
}

void init_network(Network* network)
{
    network->layers[0] = new_layer(network->size_input, 0);

    for (unsigned int i = 1; i < network->nb_layers - 1; i++)
        network->layers[i] = new_layer(network->nb_hidden_neurons,
                                       network->layers[i - 1].nb_neurons);

    network->layers[network->nb_layers - 1] =
        new_layer(network->size_output,
                  network->layers[network->nb_layers - 2].nb_neurons);

    init_weights(network);
}

Network new_network(unsigned int size_input, unsigned int nb_hidden,
                    unsigned int nb_neurons, unsigned int size_output)
{
    Network network = {
        .size_input = size_input,
        .size_output = size_output,
        .nb_layers = nb_hidden + 2,
        .nb_hidden_neurons = nb_neurons,
        .layers = NULL,
    };

    network.layers =
        (Layer*)calloc((network.nb_layers + 1), sizeof(struct Layer));
    if (network.layers == NULL)
        errx(EXIT_FAILURE, "Error while allocating memory");

    init_network(&network);

    return network;
}

void free_network(Network* network)
{
    for (unsigned int i = 0; i < network->nb_layers; i++)
        free_layer(&(network->layers[i]));

    free(network->layers);
}

//----- ACTIVATION FUNCTIONS -----//

double relu(double x)
{
    return x > 0 ? x : 0.01 * x;
}

double relu_prime(double x)
{
    return x > 0 ? 1 : 0.01;
}

double sigmoid(double x)
{
    return 1 / (1 + exp(-1.0 * x));
}

double sigmoid_prime(double x)
{
    return (x * (1 - x));
}

double softmax(double x)
{
    return exp(x) / (1 + exp(x));
}

void softmax_layer(Layer* layer)
{
    double sum = 0;

    for (unsigned int i = 0; i < layer->nb_neurons; i++)
    {
        layer->neurons[i].value = exp(layer->neurons[i].value);
        sum += layer->neurons[i].value;
    }

    for (unsigned int i = 0; i < layer->nb_neurons; i++)
        layer->neurons[i].value = layer->neurons[i].value / sum;
}

//----- FRONTPROPAGATION -----//

void front_propagation(Network* network, double input_data[],
                       unsigned int clean)
{
    Layer* prev_layer;
    Layer* curr_layer = &(network->layers[0]);

    for (unsigned int i = 0; i < curr_layer->nb_neurons; i++)
        curr_layer->neurons[i].value = input_data[i];

    for (unsigned int i = 1; i < network->nb_layers; i++)
    {
        prev_layer = &(network->layers[i - 1]);
        curr_layer = &(network->layers[i]);

        for (unsigned int j = 0; j < curr_layer->nb_neurons; j++)
        {
            Neuron* curr_neuron = &(curr_layer->neurons[j]);
            curr_neuron->value = 0;

            for (unsigned int k = 0; k <= prev_layer->nb_neurons; k++)
                curr_neuron->value += prev_layer->neurons[k].value
                    * curr_neuron->weights[k] / 100;

            if (clean == 0)
                curr_neuron->value = relu(curr_neuron->value);
        }
    }

    if (clean == 0)
        softmax_layer(curr_layer);
}

//----- BACKPROPAGATION -----//

void int_to_array(double x, double desired_output[], unsigned int size)
{
    for (size_t i = 0; i < size; i++)
        desired_output[i] = (i == x ? 1 : 0);
}

void back_propagation(Network* network, double desired_output[])
{
    double error_var = 0;

    Neuron* curr_neuron;
    Layer* output_layer = &(network->layers[network->nb_layers - 1]);

    for (unsigned int i = 0; i < output_layer->nb_neurons; i++)
    {
        curr_neuron = &(output_layer->neurons[i]);
        error_var = desired_output[i] - curr_neuron->value;
        curr_neuron->delta = error_var * sigmoid_prime(curr_neuron->value);
    }

    for (unsigned int i = network->nb_layers - 1; i >= 2; i--)
    {
        Layer curr_layer = network->layers[i];
        Layer* previousLayer = &(network->layers[i - 1]);

        for (unsigned int j = 0; j < previousLayer->nb_neurons; j++)
        {
            error_var = 0;
            curr_neuron = &(previousLayer->neurons[j]);

            for (unsigned int k = 0; k < curr_layer.nb_neurons; k++)
                error_var += curr_layer.neurons[k].delta
                    * curr_layer.neurons[k].weights[j];

            curr_neuron->delta = error_var * relu_prime(curr_neuron->value);
        }
    }
}

void gradient_descent(Network* network, double learning_rate)
{
    for (unsigned int i = network->nb_layers - 1; i >= 1; i--)
    {
        Layer* curr_layer = &(network->layers[i]);
        Layer* previousLayer = &(network->layers[i - 1]);

        for (unsigned int j = 0; j < curr_layer->nb_neurons; j++)
        {
            Neuron* curr_neuron = &(curr_layer->neurons[j]);

            for (unsigned int k = 0; k < previousLayer->nb_neurons; k++)
                curr_neuron->weights[k] += curr_neuron->delta
                    * previousLayer->neurons[k].value * learning_rate;
        }
    }
}
