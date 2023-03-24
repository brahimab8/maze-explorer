#include "engine/maze.h"

#include <stdio.h>
#include <stdlib.h>

void init_maze(int rows, int cols, Cell ***out) {
    Cell *pool = calloc(rows * cols, sizeof(Cell));
    Cell **grid = malloc(rows * sizeof(Cell*));
    if (!pool || !grid) {
        perror("malloc");
        exit(1);
    }
    for (int r = 0; r < rows; ++r) {
        grid[r] = pool + r * cols;
        for (int c = 0; c < cols; ++c) {
            grid[r][c].visited = 0;
            grid[r][c].wallN = grid[r][c].wallS =
            grid[r][c].wallE = grid[r][c].wallW = 1;
        }
    }
    *out = grid;
}

void free_maze(Cell **grid) {
    free(grid[0]);
    free(grid);
}

static void remove_wall(Cell *a, Cell *b, int d) {
    switch (d) {
        case 0: a->wallN = 0; b->wallS = 0; break;
        case 1: a->wallS = 0; b->wallN = 0; break;
        case 2: a->wallW = 0; b->wallE = 0; break;
        case 3: a->wallE = 0; b->wallW = 0; break;
    }
}

void carve(Cell **grid, int rows, int cols, int r, int c) {
    grid[r][c].visited = 1;
    int dirs[4] = {0, 1, 2, 3};
    for (int i = 3; i > 0; --i) {
        int j = rand() % (i + 1);
        int t = dirs[i];
        dirs[i] = dirs[j];
        dirs[j] = t;
    }
    for (int i = 0; i < 4; ++i) {
        int d = dirs[i];
        int nr = r + (d == 0 ? -1 : d == 1 ? 1 : 0);
        int nc = c + (d == 2 ? -1 : d == 3 ? 1 : 0);
        if (nr < 0 || nr >= rows || nc < 0 || nc >= cols || grid[nr][nc].visited)
            continue;
        remove_wall(&grid[r][c], &grid[nr][nc], d);
        carve(grid, rows, cols, nr, nc);
    }
}
