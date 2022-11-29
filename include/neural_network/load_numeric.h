#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <unistd.h>
#include <dirent.h>

#define SIZE 784
#define NUM_TRAIN 6298
#define NUM_TEST 6298

double train_image[NUM_TRAIN][SIZE] = {0};
double test_image[NUM_TEST][SIZE] = {0};
int train_label[NUM_TRAIN];
int test_label[NUM_TEST];

void load_numeric()
{
    size_t curr_NUM = 0;
    int expected;

    DIR *directory = opendir("../../../include/neural_network/data/numeric");
    struct dirent* file;
    char ch;

    while ((file = readdir(directory)) != NULL)
    {
        char tmp[100];
        strcpy(tmp, file->d_name);
        expected = tmp[0] - '0';

        char filename[1000];
        sprintf(filename, "%s/%s", "../../../include/neural_network/data/numeric", file->d_name);

        FILE* fin = fopen(filename, "r");

        if (fin != NULL && strcmp(file->d_name, ".") != 0
                && strcmp(file->d_name, "..") != 0)
        {
            size_t n = 0;
            ch = fgetc(fin);

            while (ch != EOF)
            {
                if (ch == '0')
                {
                    train_image[curr_NUM][n] = 0;
                    test_image[curr_NUM][n] = 0;
                    n++;
                }
                else if (ch == '1')
                {
                    train_image[curr_NUM][n] = 1;
                    test_image[curr_NUM][n] = 1;
                    n++;
                }

                ch = fgetc(fin);
            }

            train_label[curr_NUM] = expected;
            test_label[curr_NUM] = expected;
            curr_NUM++;

            fclose(fin);
        }
    }

    closedir(directory);
}
