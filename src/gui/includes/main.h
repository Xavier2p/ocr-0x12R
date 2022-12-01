#ifndef MAIN_H
#define MAIN_H

void func1();
void func2();
void func3();
void func4();
void func5();

void (*funcs[5])() = {func1, func2, func3, func4, func5};

#endif
