/*
 * =====================================================================================
 *
 *       Filename:  struc_sudoku_solver.c
 *
 *    Description: This file will contain the struct of the game and the
 *    functions to import and create the board to solve.
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

#include "../../include/sudoku_solver/sudoku_file_manager.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define N 9

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

int solve_sudoku(int **grid, int row, int col)
{
    if (row == N - 1 && col == N)
        return 1;

    if (col == N)
    {
        row++;
        col = 0;
    }

    if (grid[row][col] > 0)
        return solve_sudoku(grid, row, col + 1);

    for (int num = 1; num <= N; num++)
    {
        if (is_safe(grid, row, col, num) == 1)
        {
            grid[row][col] = num;

            if (solve_sudoku(grid, row, col + 1) == 1)
                return 1;
        }

        grid[row][col] = 0;
    }
    return 0;
}

int main(int argc, char **argv)
{
    if (argc > 2)
        return 1;

    int **grid = (int **)calloc(10, sizeof(int *));
    for (size_t i = 0; i < 9; ++i)
        grid[i] = (int *)calloc(10, sizeof(int));

    // Import a grid from the file named by the user in the argv[1] slot.
    import_grid_block(grid, argv[1]);

    solve_sudoku(grid, 0, 0);

    // Change the name of the file by adding .result at the end. Then the grid
    // will be exported in the new named file
    char *res = (char *)calloc(strlen(argv[1]) + 8, sizeof(char));
    int i = 0;
    for (; argv[1][i] != '\0'; ++i)
        res[i] = argv[1][i];

    res[i] = '.';
    res[i + 1] = 'r';
    res[i + 2] = 'e';
    res[i + 3] = 's';
    res[i + 4] = 'u';
    res[i + 5] = 'l';
    res[i + 6] = 't';
    res[i + 7] = '\0';

    export_grid(grid, res);

    for (size_t i = 0; i < 10; ++i)
        free(grid[i]);
    free(grid);
    free(res);

    return 0;
}
