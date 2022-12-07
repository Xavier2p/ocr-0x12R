#include "include/load_numeric.h"

void load_numeric(double train_image[NUM_TRAIN][SIZE],
                  double test_image[NUM_TEST][SIZE], int train_label[NUM_TRAIN],
                  int test_label[NUM_TEST])
{
    size_t curr_NUM = 0;
    int expected;

    DIR* directory = opendir("src/neural_network/include/data/numeric");
    struct dirent* file;
    char ch;

    while ((file = readdir(directory)) != NULL)
    {
        char tmp[100];
        strcpy(tmp, file->d_name);
        expected = tmp[0] - '0';

        char filename[1000];
        sprintf(filename, "%s/%s", "src/neural_network/include/data/numeric",
                file->d_name);

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
        }

        fclose(fin);
    }

    closedir(directory);
}
