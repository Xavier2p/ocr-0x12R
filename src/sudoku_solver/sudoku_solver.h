/*
 * =====================================================================================
 *
 *       Filename:  sudoku_file_manager.h
 *
 *    Description: Headers for the sudoku_solver file
 *
 *        Version:  1.0
 *        Created:  09/20/2022 09:46:24 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Paolo Wattebled
 *   Organization:  Fach'OCR
 *
 * =====================================================================================
 */
#ifndef SUDOKU_FILE_MANAGER_H
#define SUDOKU_FILE_MANAGER_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define N 9

void print_grid(int** sudoku_grid);

void export_grid(int** grid, char filename[]);

void import_grid(int** grid, char filename[]);

void solve_sudoku(int** grid);

#endif
