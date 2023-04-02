#include "engine/movement.h"

bool can_step(Cell **grid,
              int rows,
              int cols,
              int x,
              int y,
              Direction dir)
{
    if (x < 0 || x >= cols || y < 0 || y >= rows) return false;

    switch (dir) {
      case DIR_UP:
        return y > 0        && !grid[y][x].wallN;
      case DIR_DOWN:
        return y < rows-1   && !grid[y][x].wallS;
      case DIR_LEFT:
        return x > 0        && !grid[y][x].wallW;
      case DIR_RIGHT:
        return x < cols-1   && !grid[y][x].wallE;
    }
    return false;
}
