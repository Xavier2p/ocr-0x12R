#include <stdio.h>
#include <stdlib.h>
#include "../../../include/neural_network/training.h"

int main()
{
    int result = training("./saved_data/weights.data", 3, 200, 0.01, NULL, 1);
    //3 200 0.01
    return result;
}
