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
#include <stdio.h>
#include <time.h>

void print_grid(int** sudoku_grid)
{
    for (int i = 0; i < 9; ++i)
    {
        for (int j = 0; j < 9; ++j)
        {
            if (j == 3 || j == 6)
                printf("|");
            printf("%d", sudoku_grid[i][j]);
        }

        printf("\n");
        if (i == 2 || i == 5)
            printf("-----------\n");
    }
}

int is_safe(int** grid, int row, int col, int num)
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

int solve_sudoku_rec(int** grid, int row, int col, int* i)
{
    *i = *i + 1;
    if (*i >= 20000)
        return 0;

    if (row == N - 1 && col == N)
        return 1;

    if (col == N)
    {
        row++;
        col = 0;
    }

    if (grid[row][col] > 0)
        return solve_sudoku_rec(grid, row, col + 1, i);

    for (int num = 1; num <= N; num++)
    {
        if (is_safe(grid, row, col, num) == 1)
        {
            grid[row][col] = num;

            if (solve_sudoku_rec(grid, row, col + 1, i) == 1)
                return 1;
        }

        grid[row][col] = 0;
    }

    return 0;
}

void import_grid(int** grid, char filename[])
{
    FILE* fp = fopen(filename, "r");

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

void export_grid(int** grid, char filename[])
{
    FILE* fp;
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

void solve_sudoku(int** grid)
{
    int i = 0;

    if (solve_sudoku_rec(grid, 0, 0, &i) == 0)
    {
        printf("No solution exists\n\n");
    }
    else
    {
        printf("Sudoku solved:\n");

        print_grid(grid);
        export_grid(grid, "grid.result");
    }
}
