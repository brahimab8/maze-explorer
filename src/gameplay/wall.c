#include "gameplay/wall.h"


void wall_destroy(Cell **grid, int rows, int cols, int x, int y, Direction dir) {
    if (x < 0 || x >= cols || y < 0 || y >= rows) return;
    switch (dir) {
      case DIR_UP:
        if (!grid[y][x].wallN) return;
        grid[y][x].wallN = 0;
        if (y>0)        grid[y-1][x].wallS = 0;
        break;
      case DIR_DOWN:
        if (!grid[y][x].wallS) return;
        grid[y][x].wallS = 0;
        if (y<rows-1)   grid[y+1][x].wallN = 0;
        break;
      case DIR_LEFT:
        if (!grid[y][x].wallW) return;
        grid[y][x].wallW = 0;
        if (x>0)        grid[y][x-1].wallE = 0;
        break;
      case DIR_RIGHT:
        if (!grid[y][x].wallE) return;
        grid[y][x].wallE = 0;
        if (x<cols-1)   grid[y][x+1].wallW = 0;
        break;
    }
}

void wall_destroy_cb(void *user_data,
                     Cell **grid,
                     int rows,
                     int cols,
                     int x,
                     int y,
                     Direction dir)
{
    (void)user_data;      // unused
    wall_destroy(grid,
                 rows,
                 cols,
                 x,
                 y,
                 dir);
}
