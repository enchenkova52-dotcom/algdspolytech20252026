#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "paper.h"

struct Paper {
    int rows;
    int cols;
    bool** grid;
    bool** visited;
};

Paper* create_paper(int m, int n) {
    if (m <= 0 || n <= 0) {
        return NULL;
    }

    Paper* paper = (Paper*)malloc(sizeof(Paper));
    if (!paper) return NULL;

    paper->rows = m;
    paper->cols = n;

    paper->grid = (bool**)malloc(m * sizeof(bool*));
    if (!paper->grid) {
        free(paper);
        return NULL;
    }

    paper->visited = (bool**)malloc(m * sizeof(bool*));
    if (!paper->visited) {
        free(paper->grid);
        free(paper);
        return NULL;
    }

    for (int i = 0; i < m; i++) {
        paper->grid[i] = (bool*)malloc(n * sizeof(bool));
        paper->visited[i] = (bool*)malloc(n * sizeof(bool));

        if (!paper->grid[i] || !paper->visited[i]) {
            for (int j = 0; j <= i; j++) {
                if (paper->grid[j]) free(paper->grid[j]);
                if (paper->visited[j]) free(paper->visited[j]);
            }
            free(paper->grid);
            free(paper->visited);
            free(paper);
            return NULL;
        }
    }

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            paper->grid[i][j] = true;
            paper->visited[i][j] = false;
        }
    }

    return paper;
}

void destroy_paper(Paper* paper) {
    if (!paper) return;

    for (int i = 0; i < paper->rows; i++) {
        free(paper->grid[i]);
        free(paper->visited[i]);
    }
    free(paper->grid);
    free(paper->visited);
    free(paper);
}

bool is_valid_cell(Paper* paper, int row, int col) {
    if (!paper) return false;

    return (row >= 0 && row < paper->rows && col >= 0 && col < paper->cols &&
        paper->grid[row][col] && !paper->visited[row][col]);
}

void remove_cells(Paper* paper, int k, int* x_coords, int* y_coords) {
    if (!paper || !x_coords || !y_coords || k <= 0) return;

    for (int i = 0; i < k; i++) {
        int x = x_coords[i];
        int y = y_coords[i];

        if (x >= 0 && x < paper->rows && y >= 0 && y < paper->cols) {
            paper->grid[x][y] = false;
        }
    }
}

void dfs(Paper* paper, int row, int col) {
    if (!paper) return;

    paper->visited[row][col] = true;

    int directions[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

    for (int i = 0; i < 4; i++) {
        int new_row = row + directions[i][0];
        int new_col = col + directions[i][1];

        if (is_valid_cell(paper, new_row, new_col)) {
            dfs(paper, new_row, new_col);
        }
    }
}

int count_connected_components(Paper* paper) {
    if (!paper) return 0;

    for (int i = 0; i < paper->rows; i++) {
        for (int j = 0; j < paper->cols; j++) {
            paper->visited[i][j] = false;
        }
    }

    int count = 0;

    for (int i = 0; i < paper->rows; i++) {
        for (int j = 0; j < paper->cols; j++) {
            if (paper->grid[i][j] && !paper->visited[i][j]) {
                dfs(paper, i, j);
                count++;
            }
        }
    }

    return count;
}
