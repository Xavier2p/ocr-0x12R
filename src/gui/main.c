#include <stdio.h>
#include <stdlib.h>
#include "includes/gui.h"

void func4()
{
    printf("pointed\n");
}

void func5()
{
    printf("functions.\n");
}

void func3()
{
    printf("array of\n");
}

void func2()
{
    printf("is an\n");
}

void func1()
{
    printf("This\n");
}


int main(int argc, char* argv[])
{
    init_gui(argc, argv);
    return EXIT_SUCCESS;
}


// int solve_all(argc, argv)
// {
//     // This function is ready to solve all the OCR,
//     // fully automatically.
//     // It will call the functions in the right order, to run it.
//     return EXIT_SUCCESS;
// }

