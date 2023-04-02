#ifndef ENGINE_MOVEMENT_H
#define ENGINE_MOVEMENT_H

#include "engine/maze.h"


typedef enum {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} Direction;

/**
 * Returns true if from (x,y) in a rows×cols grid you can step one cell in `dir`
 * without hitting a wall or going out of bounds.
 */
bool can_step(Cell **grid,
              int rows,
              int cols,
              int x,
              int y,
              Direction dir);

#endif // ENGINE_MOVEMENT_H
