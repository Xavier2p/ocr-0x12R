#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <err.h>

//----- PARAM INITIALIZATION -----//

typedef struct Neuron
{
    unsigned int nb_weights;
    double* weights;
    double value;
    double delta;
} Neuron;

typedef struct Layer
{
    unsigned int nb_neurons;
    Neuron* neurons;
} Layer;

typedef struct Network
{
    unsigned int size_input;
    unsigned int size_output;
    unsigned int nb_layers;
    unsigned int nb_hidden_neurons;
    Layer* layers;
} Network;

//----- NEURON INITIALIZATION -----//

Neuron new_neuron(unsigned int nb_weights);

void init_neuron(Neuron* neuron);

void free_neuron(Neuron* neuron);

//----- LAYER INITIALIZATION -----//

void init_layer(Layer* layer, unsigned int weights);

Layer new_layer(unsigned int size, unsigned int weights);

void free_layer(Layer* layer);

//----- NETWORK INITIALIZATION -----//

void init_weights(Network* network);

void init_network(Network* network);

Network new_network(unsigned int size_input, unsigned int nb_hidden,
                    unsigned int nb_neurons, unsigned int size_output);

void free_network(Network* network);

//----- ACTIVATION FUNCTIONS -----//

double relu(double x);

double relu_prime(double x);

double sigmoid(double x);

double sigmoid_prime(double x);

double softmax(double x);

void softmax_layer(Layer* layer);

//----- FRONTPROPAGATION -----//

void front_propagation(Network* network, double input_data[],
                       unsigned int clean);

//----- BACKPROPAGATION -----//

void int_to_array(double x, double desired_output[], unsigned int size);

void back_propagation(Network* network, double desired_output[]);

void gradient_descent(Network* network, double learning_rate);

#endif
