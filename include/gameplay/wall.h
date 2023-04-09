#ifndef GAMEPLAY_WALL_H
#define GAMEPLAY_WALL_H

#include "engine/maze.h" 
#include "engine/movement.h"

/**
 * Remove the wall on side `dir` of cell (x,y).  Also remove the
 * opposite wall in the adjacent cell.
 */
void wall_destroy(Cell **grid, int rows, int cols, int x, int y, Direction dir);

#endif // GAMEPLAY_WALL_H
