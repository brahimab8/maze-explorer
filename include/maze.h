#ifndef MAZE_H
#define MAZE_H

#include <stdbool.h>

typedef struct {
    unsigned visited : 1;
    unsigned wallN   : 1;
    unsigned wallS   : 1;
    unsigned wallE   : 1;
    unsigned wallW   : 1;
} Cell;

void init_maze(int rows, int cols, Cell ***out_grid);
void carve(Cell **grid, int rows, int cols, int r, int c);
void free_maze(Cell **grid);

#endif // MAZE_H
