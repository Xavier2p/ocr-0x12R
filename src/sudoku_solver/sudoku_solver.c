/*
 * =====================================================================================
 *
 *       Filename:  struc_sudoku_solver.c
 *
 *    Description:  File containing the sudoku solver and the function to
 *    import a grid
 *
 *        Version:  1.0
 *        Created:  09/13/2022 07:40:29 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Paolo.Wattebled
 *   Organization: Fach'OCR
 *
 * =====================================================================================
 */
#include "sudoku_solver.h"

void print_grid(int **sudoku_grid)
{
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            printf("%d", sudoku_grid[i][j]);
        }
        printf("\n");
    }
}
int is_safe(int **grid, int row, int col, int num)
{
    for (int x = 0; x <= 8; x++)
        if (grid[row][x] == num)
            return 0;

    for (int x = 0; x <= 8; x++)
        if (grid[x][col] == num)
            return 0;

    int startRow = row - row % 3, startCol = col - col % 3;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[i + startRow][j + startCol] == num)
                return 0;

    return 1;
}

int solve_sudoku_rec(int **grid, int row, int col)
{
    if (row == N - 1 && col == N)
        return 1;

    if (col == N)
    {
        row++;
        col = 0;
    }

    if (grid[row][col] > 0)
        return solve_sudoku_rec(grid, row, col + 1);

    for (int num = 1; num <= N; num++)
    {
        if (is_safe(grid, row, col, num) == 1)
        {
            grid[row][col] = num;

            if (solve_sudoku_rec(grid, row, col + 1) == 1)
                return 1;
        }

        grid[row][col] = 0;
    }

    return 0;
}

void import_grid(int **grid, char filename[])
{
    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
        printf("file can't be opened \n");

    char line[25];

    size_t bias_hori = 0;
    size_t j = 0;
    while (fgets(line, sizeof(line), fp))
    {
        if (line[0] == '\n' || line[0] == '\r' || line[0] == ' ')
            bias_hori++;
        else
        {
            size_t bias_verti = 0;
            for (size_t i = 0; line[i] != '\n'; i++)
            {
                if (line[i] == ' ')
                    bias_verti++;
                else if (line[i] == '.')
                    grid[j - bias_hori][i - bias_verti] = 0;
                else
                    grid[j - bias_hori][i - bias_verti] = line[i] - '0';
            }
        }
        j++;
    }

    fclose(fp);
}

void export_grid(int **grid, char filename[])
{
    FILE *fp;
    fp = fopen(filename, "w");

    for (size_t i = 0; i < 9; i++)
    {
        for (size_t j = 0; j < 9; j++)
        {
            if (j == 2 || j == 5)
            {
                fputc(grid[i][j] + '0', fp);
                fputc(' ', fp);
            }
            else
                fputc(grid[i][j] + '0', fp);
        }
        fputc('\n', fp);
        if (i == 2 || i == 5)
            fputc('\n', fp);
    }

    fclose(fp);
}

void solve_sudoku(int **grid, int row, int col)
{
    solve_sudoku_rec(grid, row, col);

    export_grid(grid, "grid.result");
}

// int main(int argc, char **argv)
//{
//     if (argc > 2)
//         return 1;
//
//     int **grid = (int **)calloc(9, sizeof(int *));
//     for (size_t i = 0; i < 8; ++i)
//         grid[i] = (int *)calloc(9, sizeof(int));
//
//     import_grid(grid, argv[1]);
//
//     solve_sudoku(grid, 0, 0);
//
//     char *res = (char *)calloc(strlen(argv[1]) + 8, sizeof(char));
//     int i = 0;
//     for (; argv[1][i] != '\0'; ++i)
//         res[i] = argv[1][i];
//
//     res[i] = '.';
//     res[i + 1] = 'r';
//     res[i + 2] = 'e';
//     res[i + 3] = 's';
//     res[i + 4] = 'u';
//     res[i + 5] = 'l';
//     res[i + 6] = 't';
//     res[i + 7] = '\0';
//
//     export_grid(grid, res);
//
//     for (size_t i = 0; i < 9; ++i)
//         free(grid[i]);
//
//     free(grid);
//     free(res);
//
//     return 0;
// }
