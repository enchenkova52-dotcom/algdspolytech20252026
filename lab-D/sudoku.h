#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_N 9
#define MAX_SIZE (MAX_N * MAX_N * MAX_N)

typedef struct Node {
  struct Node *left, *right, *up, *down;
  struct Node* column;
  int row_id;
  int size;
} Node;

typedef struct {
  int N;
  int grid[MAX_N][MAX_N];
  Node* head;
  Node* nodes;
  int node_count;
  int* solution;
  int solution_count;
} SudokuSolver;

Node* create_node(SudokuSolver* solver);
void init_solver(SudokuSolver* solver, int N);
void add_node_to_column(Node* column, Node* node);
void create_exact_cover(SudokuSolver* solver);
void cover_column(Node* column);
void uncover_column(Node* column);
Node* choose_column(SudokuSolver* solver);
int solve_exact_cover(SudokuSolver* solver, int depth);
void solution_to_sudoku(SudokuSolver* solver);
int is_valid_sudoku(SudokuSolver* solver);
int solve_sudoku(SudokuSolver* solver);
int read_input(const char* filename, SudokuSolver* solver);
void write_output(const char* filename, SudokuSolver* solver, int solved);

#endif
