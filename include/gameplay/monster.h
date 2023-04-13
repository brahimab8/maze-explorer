#ifndef GAMEPLAY_MONSTER_H
#define GAMEPLAY_MONSTER_H

#include <stdbool.h>
#include "engine/maze.h"
#include "engine/movement.h"

#define MAX_MONSTERS 10

typedef struct {
    int x, y;
    int base_cooldown;   // frames between moves
    int cooldown;        // frames until next move
} Monster;

/**
 * Given the level, returns how many monsters you should spawn.
 * Levels 1–5: 1 monster, 6–10: 2 monsters, etc., capped at MAX_MONSTERS.
 */
int monster_count_for_level(int lvl);

/**
 * Given the level, returns the “base cooldown” (in frames)
 * between moves for each monster.  Higher levels → faster (smaller cooldown).
 * Cycles every 5 levels.
 */
int monster_base_cooldown_for_level(int lvl);

/**
 * Fill `M[0..*n_out-1]` with newly-spawned monsters for this level.
 */
void monsters_init(Monster *M, int *n_out, int lvl, int rows, int cols);

/**
 * Advance each monster’s cooldown & (when ready) move one step toward (px,py).
 */
void monsters_step(Monster *list, int n,
                   int px, int py,
                   Cell **grid, int rows, int cols);

#endif // GAMEPLAY_MONSTER_H
