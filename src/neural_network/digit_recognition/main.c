#include <stdio.h>
#include <stdlib.h>
#include "../../../include/neural_network/training.h"

int main()
{
    int result = training("./saved_data/weights.data", 2, 200, 0.025, NULL, 1);
    return result;
}
