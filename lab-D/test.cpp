
#include <gtest/gtest.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
#include "C:/Users/Виолетта/source/repos/D/D/sudoku.h"
}

class SudokuTest : public ::testing::Test {
 protected:
  void SetUp() override {}

  void TearDown() override {
    remove("test_input.txt");
    remove("test_output.txt");
  }

  void create_test_file(const char* filename, const char* content) {
    FILE* file = fopen(filename, "w");
    if (file) {
      fprintf(file, "%s", content);
      fclose(file);
    }
  }

  char* read_output_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* content = (char*)malloc(length + 1);
    fread(content, 1, length, file);
    content[length] = '\0';

    fclose(file);
    return content;
  }

  int count_zeros_in_grid(SudokuSolver* solver) {
    int size = solver->N * solver->N;
    int zero_count = 0;
    for (int i = 0; i < size; i++) {
      for (int j = 0; j < size; j++) {
        if (solver->grid[i][j] == 0) {
          zero_count++;
        }
      }
    }
    return zero_count;
  }
};

TEST_F(SudokuTest, SolveSimpleSudoku3x3_HappyPath_no1) {
  const char* input =
      "3\n"
      "5 3 0 0 7 0 0 0 0\n"
      "6 0 0 1 9 5 0 0 0\n"
      "0 9 8 0 0 0 0 6 0\n"
      "8 0 0 0 6 0 0 0 3\n"
      "4 0 0 8 0 3 0 0 1\n"
      "7 0 0 0 2 0 0 0 6\n"
      "0 6 0 0 0 0 2 8 0\n"
      "0 0 0 4 1 9 0 0 5\n"
      "0 0 0 0 8 0 0 7 9\n";

  create_test_file("test_input.txt", input);

  SudokuSolver solver;
  ASSERT_TRUE(read_input("test_input.txt", &solver));
  init_solver(&solver, solver.N);
  int solved = solve_sudoku(&solver);

  EXPECT_TRUE(solved);
  EXPECT_TRUE(is_valid_sudoku(&solver));
  EXPECT_EQ(count_zeros_in_grid(&solver), 0);

  free(solver.nodes);
  free(solver.solution);
}

TEST_F(SudokuTest, SolveAlreadySolvedSudoku_EdgeCase_no2) {
  const char* input =
      "3\n"
      "5 3 4 6 7 8 9 1 2\n"
      "6 7 2 1 9 5 3 4 8\n"
      "1 9 8 3 4 2 5 6 7\n"
      "8 5 9 7 6 1 4 2 3\n"
      "4 2 6 8 5 3 7 9 1\n"
      "7 1 3 9 2 4 8 5 6\n"
      "9 6 1 5 3 7 2 8 4\n"
      "2 8 7 4 1 9 6 3 5\n"
      "3 4 5 2 8 6 1 7 9\n";

  create_test_file("test_input.txt", input);

  SudokuSolver solver;
  ASSERT_TRUE(read_input("test_input.txt", &solver));
  init_solver(&solver, solver.N);
  int solved = solve_sudoku(&solver);

  EXPECT_TRUE(solved);
  EXPECT_TRUE(is_valid_sudoku(&solver));

  free(solver.nodes);
  free(solver.solution);
}

TEST_F(SudokuTest, SolveImpossibleSudoku_RowConflict_no3) {
  const char* input =
      "3\n"
      "5 5 0 0 7 0 0 0 0\n"
      "6 0 0 1 9 5 0 0 0\n"
      "0 9 8 0 0 0 0 6 0\n"
      "8 0 0 0 6 0 0 0 3\n"
      "4 0 0 8 0 3 0 0 1\n"
      "7 0 0 0 2 0 0 0 6\n"
      "0 6 0 0 0 0 2 8 0\n"
      "0 0 0 4 1 9 0 0 5\n"
      "0 0 0 0 8 0 0 7 9\n";

  create_test_file("test_input.txt", input);

  SudokuSolver solver;
  ASSERT_TRUE(read_input("test_input.txt", &solver));
  init_solver(&solver, solver.N);
  int solved = solve_sudoku(&solver);

  EXPECT_FALSE(solved);

  free(solver.nodes);
  free(solver.solution);
}

TEST_F(SudokuTest, SolveEmptySudoku_AllZeros_no4) {
  const char* input =
      "3\n"
      "0 0 0 0 0 0 0 0 0\n"
      "0 0 0 0 0 0 0 0 0\n"
      "0 0 0 0 0 0 0 0 0\n"
      "0 0 0 0 0 0 0 0 0\n"
      "0 0 0 0 0 0 0 0 0\n"
      "0 0 0 0 0 0 0 0 0\n"
      "0 0 0 0 0 0 0 0 0\n"
      "0 0 0 0 0 0 0 0 0\n"
      "0 0 0 0 0 0 0 0 0\n";

  create_test_file("test_input.txt", input);

  SudokuSolver solver;
  ASSERT_TRUE(read_input("test_input.txt", &solver));
  init_solver(&solver, solver.N);
  int solved = solve_sudoku(&solver);

  EXPECT_TRUE(solved);
  EXPECT_TRUE(is_valid_sudoku(&solver));
  EXPECT_EQ(count_zeros_in_grid(&solver), 0);

  free(solver.nodes);
  free(solver.solution);
}

TEST_F(SudokuTest, SolveMinimalCluesSudoku_Algorithmic_no5) {
  const char* input =
      "3\n"
      "0 0 0 0 0 0 0 1 0\n"
      "0 0 0 0 0 2 0 0 3\n"
      "0 0 0 4 0 0 0 0 0\n"
      "0 0 0 0 0 0 5 0 0\n"
      "4 0 1 6 0 0 0 0 0\n"
      "0 0 7 1 0 0 0 0 0\n"
      "0 5 0 0 0 0 2 0 0\n"
      "0 0 0 0 8 0 0 4 0\n"
      "0 3 0 9 1 0 0 0 0\n";

  create_test_file("test_input.txt", input);

  SudokuSolver solver;
  ASSERT_TRUE(read_input("test_input.txt", &solver));
  init_solver(&solver, solver.N);
  int solved = solve_sudoku(&solver);

  EXPECT_TRUE(solved);
  EXPECT_TRUE(is_valid_sudoku(&solver));

  free(solver.nodes);
  free(solver.solution);
}

TEST_F(SudokuTest, SolveColumnConflictSudoku_InvalidInput_no6) {
  const char* input =
      "3\n"
      "1 0 0 0 0 0 0 0 0\n"
      "1 0 0 0 0 0 0 0 0\n"
      "0 0 0 0 0 0 0 0 0\n"
      "0 0 0 0 0 0 0 0 0\n"
      "0 0 0 0 0 0 0 0 0\n"
      "0 0 0 0 0 0 0 0 0\n"
      "0 0 0 0 0 0 0 0 0\n"
      "0 0 0 0 0 0 0 0 0\n"
      "0 0 0 0 0 0 0 0 0\n";

  create_test_file("test_input.txt", input);

  SudokuSolver solver;
  ASSERT_TRUE(read_input("test_input.txt", &solver));
  init_solver(&solver, solver.N);
  int solved = solve_sudoku(&solver);

  EXPECT_FALSE(solved);

  free(solver.nodes);
  free(solver.solution);
}

TEST_F(SudokuTest, SolveBlockConflictSudoku_InvalidInput_no7) {
  const char* input =
      "3\n"
      "1 0 0 0 0 0 0 0 0\n"
      "0 1 0 0 0 0 0 0 0\n"
      "0 0 0 0 0 0 0 0 0\n"
      "0 0 0 0 0 0 0 0 0\n"
      "0 0 0 0 0 0 0 0 0\n"
      "0 0 0 0 0 0 0 0 0\n"
      "0 0 0 0 0 0 0 0 0\n"
      "0 0 0 0 0 0 0 0 0\n"
      "0 0 0 0 0 0 0 0 0\n";

  create_test_file("test_input.txt", input);

  SudokuSolver solver;
  ASSERT_TRUE(read_input("test_input.txt", &solver));
  init_solver(&solver, solver.N);
  int solved = solve_sudoku(&solver);

  EXPECT_FALSE(solved);

  free(solver.nodes);
  free(solver.solution);
}

TEST_F(SudokuTest, SolveExampleSudoku_FromAssignment_no8) {
  const char* input =
      "3\n"
      "0 0 5 3 0 0 0 0 0\n"
      "8 0 0 0 0 0 0 2 0\n"
      "0 7 0 0 1 0 5 0 0\n"
      "4 0 0 0 0 5 3 0 0\n"
      "0 1 0 0 7 0 0 0 6\n"
      "0 3 2 0 0 0 0 8 0\n"
      "0 6 0 5 0 0 0 0 9\n"
      "0 0 4 0 0 0 0 3 0\n"
      "0 0 0 0 0 9 7 0 0\n";

  create_test_file("test_input.txt", input);

  SudokuSolver solver;
  ASSERT_TRUE(read_input("test_input.txt", &solver));
  init_solver(&solver, solver.N);
  int solved = solve_sudoku(&solver);

  EXPECT_TRUE(solved);
  EXPECT_TRUE(is_valid_sudoku(&solver));

  if (solved) {
    EXPECT_GE(solver.grid[0][0], 1);
    EXPECT_LE(solver.grid[0][0], 9);
    EXPECT_GE(solver.grid[8][8], 1);
    EXPECT_LE(solver.grid[8][8], 9);
  }

  free(solver.nodes);
  free(solver.solution);
}

TEST_F(SudokuTest, SolveDiagonalFilledSudoku_SpecialCase_no9) {
  const char* input =
      "3\n"
      "1 0 0 0 0 0 0 0 0\n"
      "0 2 0 0 0 0 0 0 0\n"
      "0 0 3 0 0 0 0 0 0\n"
      "0 0 0 4 0 0 0 0 0\n"
      "0 0 0 0 5 0 0 0 0\n"
      "0 0 0 0 0 6 0 0 0\n"
      "0 0 0 0 0 0 7 0 0\n"
      "0 0 0 0 0 0 0 8 0\n"
      "0 0 0 0 0 0 0 0 9\n";

  create_test_file("test_input.txt", input);

  SudokuSolver solver;
  ASSERT_TRUE(read_input("test_input.txt", &solver));
  init_solver(&solver, solver.N);
  int solved = solve_sudoku(&solver);

  EXPECT_TRUE(solved);
  EXPECT_TRUE(is_valid_sudoku(&solver));

  free(solver.nodes);
  free(solver.solution);
}

TEST_F(SudokuTest, SolveAlmostCompleteLastRow_EdgeCase_no10) {
  const char* input =
      "3\n"
      "0 0 0 0 0 0 0 0 0\n"
      "0 0 0 0 0 0 0 0 0\n"
      "0 0 0 0 0 0 0 0 0\n"
      "0 0 0 0 0 0 0 0 0\n"
      "0 0 0 0 0 0 0 0 0\n"
      "0 0 0 0 0 0 0 0 0\n"
      "0 0 0 0 0 0 0 0 0\n"
      "0 0 0 0 0 0 0 0 0\n"
      "1 2 3 4 5 6 7 8 0\n";

  create_test_file("test_input.txt", input);

  SudokuSolver solver;
  ASSERT_TRUE(read_input("test_input.txt", &solver));
  init_solver(&solver, solver.N);
  int solved = solve_sudoku(&solver);

  EXPECT_TRUE(solved);
  EXPECT_TRUE(is_valid_sudoku(&solver));
  EXPECT_EQ(solver.grid[8][8], 9);

  free(solver.nodes);
  free(solver.solution);
}

TEST_F(SudokuTest, ReadInputFile_NonExistentFile_no11) {
  SudokuSolver solver;
  int result = read_input("non_existent_file.txt", &solver);
  EXPECT_FALSE(result);
}

TEST_F(SudokuTest, ValidateSudoku_CorrectSolution_no12) {
  SudokuSolver solver;
  solver.N = 3;

  int correct_grid[9][9] = {
      {5, 3, 4, 6, 7, 8, 9, 1, 2}, {6, 7, 2, 1, 9, 5, 3, 4, 8},
      {1, 9, 8, 3, 4, 2, 5, 6, 7}, {8, 5, 9, 7, 6, 1, 4, 2, 3},
      {4, 2, 6, 8, 5, 3, 7, 9, 1}, {7, 1, 3, 9, 2, 4, 8, 5, 6},
      {9, 6, 1, 5, 3, 7, 2, 8, 4}, {2, 8, 7, 4, 1, 9, 6, 3, 5},
      {3, 4, 5, 2, 8, 6, 1, 7, 9}};

  memcpy(solver.grid, correct_grid, sizeof(correct_grid));

  EXPECT_TRUE(is_valid_sudoku(&solver));
}

TEST_F(SudokuTest, ValidateSudoku_IncorrectSolution_no13) {
  SudokuSolver solver;
  solver.N = 3;

  int incorrect_grid[9][9] = {
      {5, 5, 4, 6, 7, 8, 9, 1, 2}, {6, 7, 2, 1, 9, 5, 3, 4, 8},
      {1, 9, 8, 3, 4, 2, 5, 6, 7}, {8, 5, 9, 7, 6, 1, 4, 2, 3},
      {4, 2, 6, 8, 5, 3, 7, 9, 1}, {7, 1, 3, 9, 2, 4, 8, 5, 6},
      {9, 6, 1, 5, 3, 7, 2, 8, 4}, {2, 8, 7, 4, 1, 9, 6, 3, 5},
      {3, 4, 5, 2, 8, 6, 1, 7, 9}};

  memcpy(solver.grid, incorrect_grid, sizeof(incorrect_grid));

  EXPECT_FALSE(is_valid_sudoku(&solver));
}

TEST_F(SudokuTest, SolveMaxZerosSudoku_ExtremeCase_no14) {
  const char* input =
      "3\n"
      "0 0 0 0 0 6 0 0 0\n"
      "0 0 7 2 0 0 0 0 1\n"
      "3 0 0 0 0 0 0 0 0\n"
      "0 9 0 0 0 0 8 0 0\n"
      "0 0 0 5 0 4 0 0 0\n"
      "0 0 4 0 0 0 0 3 0\n"
      "0 0 0 0 0 0 0 0 6\n"
      "1 0 0 0 0 7 9 0 0\n"
      "0 0 0 8 0 0 0 0 0\n";

  create_test_file("test_input.txt", input);

  SudokuSolver solver;
  ASSERT_TRUE(read_input("test_input.txt", &solver));
  init_solver(&solver, solver.N);
  int solved = solve_sudoku(&solver);

  EXPECT_TRUE(solved);
  if (solved) {
    EXPECT_TRUE(is_valid_sudoku(&solver));
  }

  free(solver.nodes);
  free(solver.solution);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
