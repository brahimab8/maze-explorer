#ifndef GAMEPLAY_PROJECTILE_H
#define GAMEPLAY_PROJECTILE_H

#include <stdbool.h>
#include "engine/movement.h"  // for Direction

// Maximum shots in flight at once
#define MAX_PROJECTILES  10

/** A single in‐flight shot. */
typedef struct {
    int       x, y;      // current cell coordinates
    Direction dir;       // travel direction
    bool      active;    // still flying?
} Projectile;

/**
 * Fire a new projectile from (sx,sy) heading in dir.
 * Appends to `list[]` and increments *count
 */
void projectile_fire(Projectile *list,
                     int *count,
                     int sx,
                     int sy,
                     Direction dir);

/**
 * Advance every active projectile by one cell:
 *  - If it immediately hits a wall, calls on_hit_wall(x,y) and destroys the shot.
 *  - Otherwise moves it one cell; if now out of bounds, drops it.
 *  - Otherwise, if on_hit_monster(x,y) returns true, drops it (monster logic is external).
 *  - Otherwise keeps it alive for the next frame.
 *
 * Both callbacks may be NULL if not needed.
 */
void projectile_update(Projectile *list,
                       int *count,
                       Cell **grid,
                       int rows,
                       int cols,
                       void (*on_hit_wall)(Cell **, int, int, int, int, Direction),
                       bool (*on_hit_monster)(int x, int y));

#endif // GAMEPLAY_PROJECTILE_H
