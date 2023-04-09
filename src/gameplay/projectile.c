#include "gameplay/projectile.h"
#include "gameplay/wall.h"
#include <stdlib.h>

void projectile_fire(Projectile *list,
                     int *count,
                     int sx,
                     int sy,
                     Direction dir)
{
    if (*count >= MAX_PROJECTILES) return;
    list[*count].x      = sx;
    list[*count].y      = sy;
    list[*count].dir    = dir;
    list[*count].active = true;
    (*count)++;
}

void projectile_update(Projectile *list,
                       int *count,
                       Cell **grid,
                       int rows,
                       int cols,
                       void (*on_hit_wall)(Cell **, int, int, int, int, Direction),
                       bool (*on_hit_monster)(int x, int y))
{
    int dst = 0;
    for (int src = 0; src < *count; ++src) {
        Projectile p = list[src];
        if (!p.active) continue;

        // compute the cell it would move into
        int nx = p.x + (p.dir == DIR_RIGHT) - (p.dir == DIR_LEFT);
        int ny = p.y + (p.dir == DIR_DOWN)  - (p.dir == DIR_UP);

        // out of bounds?
        if (nx < 0 || nx >= cols || ny < 0 || ny >= rows) {
            p.active = false;
        }
        // wall hit?
        else if (!can_step(grid, rows, cols, p.x, p.y, p.dir)) {
            p.active = false;
            if (on_hit_wall) {
                on_hit_wall(grid, rows, cols, p.x, p.y, p.dir);
            }
        }
        else {
            // move into the new cell
            p.x = nx;
            p.y = ny;
            // monster hit?
            if (on_hit_monster && on_hit_monster(nx, ny)) {
                p.active = false;
            }
        }

        if (p.active) {
            // keep it for the next generation
            list[dst++] = p;
        }
    }
    *count = dst;
}
