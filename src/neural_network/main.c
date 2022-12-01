#include <stdio.h>
#include <stdlib.h>
#include "include/training.h"

int main()
{
    int result = training("./saved_data/weights.data", 3, 300, 0.015, NULL, 1);
    // 3 200 0.01
    // 3 250 0.01
    return result;
}
