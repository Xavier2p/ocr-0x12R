/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  XOR logic gate
 *
 *        Version:  1.0
 *        Created:  09/17/2022 09:39:01 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Arthur Guelennoc
 *   Organization:  Fash'OCR
 *
 * =====================================================================================
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Param initialization

double train_input[8] = { 0, 0, 0, 1, 1, 0, 1, 1 }; // Training input
double train_key[4] = { 0, 1, 1, 0 }; // Training input results
double test_input[8] = { 0, 0, 0, 1, 1, 0, 1, 1 }; // Testing input
double test_key[4] = { 0, 1, 1, 0 }; // Testing input results

double L2[2]; // Hidden layer neurons
double L3[1]; // Output layer neuron

double L1_to_L2_weights[4]; // Input to Hidden layer weights
double L2_to_L3_weights[2]; // Hidden to Output layer weights

double learning_rate = 0.5; // Neural network LR

double *pointer = NULL; // Pointer for training or testing

// Chain rule variables

double derivative_L3[1]; // Output layer derivative
double derivative_L2_to_L3[1]; // Hidden to output layer value derivative

double derivative_L2_to_L3_weights[2]; // Hidden to output layer weights
                                       // derivatives
double L2_suggested_weight_changes[2];

double derivative_L2[2]; // Hidden layer derivatives
double derivative_L1_to_L2[2]; // Input to hidden layer values derivatives

double
    derivative_L1_to_L2_weights[4]; // Input to hidden layer weights derivatives
double L1_suggested_weight_changes[4];

// Neural Network

double sigmoid(double x)
{
    return 1 / (1 + exp(-1.0 * x));
}

double sigmoid_prime(double x)
{
    return (x * (1 - x));
}

void pass_through() // Pass through neural network
{
    for (size_t i = 0; i < 2; i++)
    {
        for (size_t j = 0; j < 2; j++)
            L2[i] += L1_to_L2_weights[(j * 2) + i] * pointer[j];

        L2[i] = sigmoid(L2[i]); // Apply sigmoid
    }

    for (size_t i = 0; i < 2; i++)
        L3[0] += L2[i] * L2_to_L3_weights[i];

    L3[0] = sigmoid(L3[0]);
}

void clear_neurons()
{
    for (size_t i = 0; i < 2; i++)
        L2[i] = 0;

    L3[0] = 0;
}

void clear_backprop()
{
    for (size_t i = 0; i < 4; i++)
    {
        derivative_L1_to_L2_weights[i] = 0;

        if (i < 2)
        {
            derivative_L2[i] = 0;
            derivative_L1_to_L2[i] = 0;
            derivative_L2_to_L3_weights[i] = 0;
        }
    }
    derivative_L3[0] = 0;
    derivative_L2_to_L3[0] = 0;
}

void apply_weights()
{
    for (size_t i = 0; i < 4; i++)
    {
        L1_to_L2_weights[i] += L1_suggested_weight_changes[i];
        L1_suggested_weight_changes[i] = 0;

        if (i < 2)
        {
            L2_to_L3_weights[i] += L2_suggested_weight_changes[i];
            L2_suggested_weight_changes[i] = 0;
        }
    }
}

void L1_to_L2_weight_updater()
{
    for (size_t i = 0; i < 4; i++)
        L1_suggested_weight_changes[i] +=
            -1 * learning_rate * derivative_L1_to_L2_weights[i];
}

void L2_to_L3_weight_updater()
{
    for (size_t i = 0; i < 2; i++)
        L2_suggested_weight_changes[i] +=
            -1 * learning_rate * derivative_L2_to_L3_weights[i];
}

void backprop() // Backpropagate to fix weights
{
    for (int epoch = 0; epoch < 100000; epoch++)
    {
        pointer = test_input;

        for (size_t set = 0; set < 4; set++)
        {
            pass_through(); // Run through the network

            // L2_to_L3 chain rule

            derivative_L3[0] = L3[0] - train_key[set];

            derivative_L2_to_L3[0] = sigmoid_prime(L3[0]) * derivative_L3[0];

            for (size_t i = 0; i < 2; i++)
                derivative_L2_to_L3_weights[i] = L2[i] * derivative_L2_to_L3[0];

            L2_to_L3_weight_updater();

            // L1_to_L2 chain rule

            for (size_t i = 0; i < 2; i++)
                derivative_L2[i] = L2_to_L3_weights[i] * derivative_L2_to_L3[0];

            for (size_t i = 0; i < 2; i++)
                derivative_L1_to_L2[i] =
                    sigmoid_prime(L2[i]) * derivative_L2[i];

            for (size_t i = 0; i < 2; i++)
                for (size_t j = 0; j < 2; j++)
                    derivative_L1_to_L2_weights[(i * 2) + j] =
                        pointer[i] * derivative_L1_to_L2[j];

            L1_to_L2_weight_updater();

            pointer += 2; // Move the pointer to the next set

            clear_backprop(); // Clear backprop local variables
            clear_neurons(); // Clear neurons
        }

        apply_weights(); // Apply suggested weight changes
    }
}

void initialize_weights() // Initialize weights
{
    srand(time(NULL)); // Initialize random num generator

    for (size_t i = 0; i < 4; i++)
        L1_to_L2_weights[i] = (double)rand() / (double)RAND_MAX;

    for (size_t i = 0; i < 2; i++)
        L2_to_L3_weights[i] = (double)rand() / (double)RAND_MAX;
}

double process_result(int a, int b, double c)
{
    return (fabs(c - a) <= fabs(c - b) ? a : b);
}

int main()
{
    initialize_weights();

    backprop();

    pointer = test_input;

    printf("%s\n", "Output    Desired output");
    for (size_t i = 0; i < 4; i++)
    {
        pass_through();
        printf("%lf  %lf\n", process_result(0, 1, L3[0]), test_key[i]);
        clear_neurons();
        pointer += 2;
    }

    putchar('\n');

    printf("%s\n", "Input layer weights");
    for (size_t i = 0; i < 4; i++)
        printf("%lf\n", L1_to_L2_weights[i]);

    putchar('\n');

    printf("%s\n", "Hidden layer weights");
    for (int i = 0; i < 2; i++)
        printf("%lf\n", L2_to_L3_weights[i]);

    return 0;
}
