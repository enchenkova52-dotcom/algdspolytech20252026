#include "sudoku.h"

int main() {
    SudokuSolver solver;
    
    if (!read_input("input.txt", &solver)) {
        printf("Error reading input file\n");
        return 1;
    }
    
    init_solver(&solver, solver.N);
    
    int solved = solve_sudoku(&solver);
    
    if (solved) {
        printf("Sudoku solved successfully!\n");
    } else {
        printf("No solution found for sudoku\n");
    }
    
    write_output("output.txt", &solver, solved);
    
    free(solver.nodes);
    free(solver.solution);
    
    return 0;
}
