
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
#pragma warning(disable : 4996)
#pragma warning(disable : 4100)
#pragma warning(disable : 4267)
#pragma warning(disable : 4018)
#endif

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wsign-compare"
#endif

#define MAX_N 9

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

Node* create_node(SudokuSolver* solver) {
  if (!solver->nodes) {
    printf("ERROR: solver->nodes is NULL!\n");
    return NULL;
  }
  Node* node = &solver->nodes[solver->node_count++];
  node->left = node->right = node->up = node->down = NULL;
  node->column = NULL;
  node->row_id = -1;
  node->size = 0;
  return node;
}

void add_node_to_column(Node* column, Node* node) {
  node->column = column;
  node->up = column->up;
  node->down = column;
  column->up->down = node;
  column->up = node;
  column->size++;
}

void init_solver(SudokuSolver* solver, int N) {
  solver->N = N;
  solver->node_count = 0;

  int size = N * N;
  int max_nodes = 4 * size * size * size * 3;
  solver->nodes = (Node*)malloc(max_nodes * sizeof(Node));
  if (!solver->nodes) {
    printf("ERROR: Failed to allocate memory for nodes!\n");
    return;
  }

  solver->solution = (int*)malloc(size * size * sizeof(int));
  if (!solver->solution) {
    printf("ERROR: Failed to allocate memory for solution!\n");
    free(solver->nodes);
    solver->nodes = NULL;
    return;
  }
  solver->solution_count = 0;

  solver->head = create_node(solver);
  solver->head->left = solver->head;
  solver->head->right = solver->head;
  solver->head->up = solver->head;
  solver->head->down = solver->head;
  solver->head->size = 0;
}

void create_exact_cover(SudokuSolver* solver) {
  int N = solver->N;
  int size = N * N;
  int constraints = 4 * size * size;
  Node** columns = (Node**)malloc(constraints * sizeof(Node*));
  if (!columns) {
    printf("ERROR: malloc columns failed\n");
    return;
  }

  for (int i = 0; i < constraints; i++) {
    columns[i] = create_node(solver);
    if (!columns[i]) {
      free(columns);
      return;
    }

    columns[i]->left = solver->head->left;
    columns[i]->right = solver->head;
    solver->head->left->right = columns[i];
    solver->head->left = columns[i];
    columns[i]->up = columns[i];
    columns[i]->down = columns[i];
    columns[i]->size = 0;
  }

  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      int block = (r / N) * N + (c / N);
      for (int num = 1; num <= size; num++) {
        if (solver->grid[r][c] != 0 && solver->grid[r][c] != num) continue;

        int cell_constraint = r * size + c;
        int row_constraint = size * size + r * size + (num - 1);
        int col_constraint = 2 * size * size + c * size + (num - 1);
        int block_constraint = 3 * size * size + block * size + (num - 1);

        Node* node1 = create_node(solver);
        Node* node2 = create_node(solver);
        Node* node3 = create_node(solver);
        Node* node4 = create_node(solver);
        if (!node1 || !node2 || !node3 || !node4) {
          free(columns);
          return;
        }

        int encoded = r * size * size + c * size + (num - 1);
        node1->row_id = encoded;
        node2->row_id = encoded;
        node3->row_id = encoded;
        node4->row_id = encoded;

        add_node_to_column(columns[cell_constraint], node1);
        add_node_to_column(columns[row_constraint], node2);
        add_node_to_column(columns[col_constraint], node3);
        add_node_to_column(columns[block_constraint], node4);

        node1->right = node2;
        node2->right = node3;
        node3->right = node4;
        node4->right = node1;
        node1->left = node4;
        node2->left = node1;
        node3->left = node2;
        node4->left = node3;
      }
    }
  }
  free(columns);
}

void cover_column(Node* column) {
  column->right->left = column->left;
  column->left->right = column->right;
  for (Node* i = column->down; i != column; i = i->down) {
    for (Node* j = i->right; j != i; j = j->right) {
      j->down->up = j->up;
      j->up->down = j->down;
      j->column->size--;
    }
  }
}

void uncover_column(Node* column) {
  for (Node* i = column->up; i != column; i = i->up) {
    for (Node* j = i->left; j != i; j = j->left) {
      j->column->size++;
      j->down->up = j;
      j->up->down = j;
    }
  }
  column->right->left = column;
  column->left->right = column;
}

Node* choose_column(SudokuSolver* solver) {
  Node* best = solver->head->right;
  for (Node* col = solver->head->right; col != solver->head; col = col->right) {
    if (col->size < best->size) best = col;
  }
  return best;
}

int solve_exact_cover(SudokuSolver* solver, int depth) {
  (void)depth;
  if (solver->head->right == solver->head) return 1;
  Node* col = choose_column(solver);
  if (!col || col->size == 0) return 0;
  cover_column(col);
  for (Node* row = col->down; row != col; row = row->down) {
    solver->solution[solver->solution_count++] = row->row_id;
    for (Node* j = row->right; j != row; j = j->right) cover_column(j->column);
    if (solve_exact_cover(solver, depth + 1)) return 1;
    solver->solution_count--;
    for (Node* j = row->left; j != row; j = j->left) uncover_column(j->column);
  }
  uncover_column(col);
  return 0;
}

void solution_to_sudoku(SudokuSolver* solver) {
  int size = solver->N * solver->N;
  for (int i = 0; i < solver->solution_count; i++) {
    int encoded = solver->solution[i];
    int num = encoded % size + 1;
    encoded /= size;
    int c = encoded % size;
    int r = encoded / size;
    solver->grid[r][c] = num;
  }
}

int is_valid_sudoku(SudokuSolver* solver) {
  int size = solver->N * solver->N;
  for (int r = 0; r < size; r++) {
    int used[MAX_N * MAX_N + 1] = {0};
    for (int c = 0; c < size; c++) {
      int num = solver->grid[r][c];
      if (num == 0) continue;
      if (num < 1 || num > size || used[num]) return 0;
      used[num] = 1;
    }
  }
  for (int c = 0; c < size; c++) {
    int used[MAX_N * MAX_N + 1] = {0};
    for (int r = 0; r < size; r++) {
      int num = solver->grid[r][c];
      if (num == 0) continue;
      if (used[num]) return 0;
      used[num] = 1;
    }
  }
  for (int br = 0; br < solver->N; br++)
    for (int bc = 0; bc < solver->N; bc++) {
      int used[MAX_N * MAX_N + 1] = {0};
      for (int r = br * solver->N; r < (br + 1) * solver->N; r++)
        for (int c = bc * solver->N; c < (bc + 1) * solver->N; c++) {
          int num = solver->grid[r][c];
          if (num == 0) continue;
          if (used[num]) return 0;
          used[num] = 1;
        }
    }
  return 1;
}

int solve_sudoku(SudokuSolver* solver) {
  create_exact_cover(solver);
  int solved = solve_exact_cover(solver, 0);
  if (solved) {
    solution_to_sudoku(solver);
    if (!is_valid_sudoku(solver)) return 0;
  }
  return solved;
}

int read_input(const char* filename, SudokuSolver* solver) {
  FILE* file = fopen(filename, "r");
  if (!file) {
    printf("Cannot open input file\n");
    return 0;
  }
  if (fscanf(file, "%d", &solver->N) != 1) {
    fclose(file);
    return 0;
  }
  int size = solver->N * solver->N;
  for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++)
      if (fscanf(file, "%d", &solver->grid[i][j]) != 1) {
        fclose(file);
        return 0;
      }
  fclose(file);
  return 1;
}

void write_output(const char* filename, SudokuSolver* solver, int solved) {
  FILE* file = fopen(filename, "w");
  if (!file) {
    printf("Cannot open output file\n");
    return;
  }
  if (!solved) {
    fprintf(file, "0\n");
  } else {
    int size = solver->N * solver->N;
    fprintf(file, "%d\n", solver->N);
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        fprintf(file, "%d", solver->grid[i][j]);
        if (j < size - 1) fprintf(file, " ");
      }
      fprintf(file, "\n");
    }
  }
  fclose(file);
}

#ifdef SUDOKU_MAIN
int main(void) {
  SudokuSolver solver;
  memset(&solver, 0, sizeof(solver));

  if (!read_input("input.txt", &solver)) {
    printf("Error reading input file\n");
    return 1;
  }

  init_solver(&solver, solver.N);
  int solved = solve_sudoku(&solver);

  if (solved)
    printf("Sudoku solved successfully!\n");
  else
    printf("No solution found for sudoku\n");

  write_output("output.txt", &solver, solved);

  free(solver.nodes);
  free(solver.solution);

  return 0;
}
#endif
