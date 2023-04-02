#ifndef GAMEPLAY_PLAYER_H
#define GAMEPLAY_PLAYER_H

#include <stdbool.h>
#include "ui/ui.h"  
#include "engine/movement.h"

/**
 * Player state: position, facing direction, ammo.
 */
typedef struct {
    int x;
    int y;
    Direction dir;
    int bullets;
    char  symbol;         
} Player;

/**
 * Initialize the player at starting position (sx, sy) with given ammo.
 */
void player_init(Player *p, int sx, int sy, int initial_bullets, char symbol);

/**
 * Move the player one cell if no wall blocks:
 *  - grid: maze array
 *  - rows, cols: maze dimensions
 *  - act: one of INP_UP/DOWN/LEFT/RIGHT
 */
void player_move(Player *p, InputAction act, Cell **grid, int rows, int cols);

/**
 * Attempt to shoot: decrements bullets if any remain, returns true on success.
 */
bool player_shoot(Player *p);

#endif // GAMEPLAY_PLAYER_H


