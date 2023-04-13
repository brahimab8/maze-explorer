#ifndef COLLISION_H
#define COLLISION_H

#include <stdbool.h>
#include "game/game.h"
#include "gameplay/monster.h"

// /**
//  * Remove a monster at the given (x,y) position in the game context.
//  * Returns true if a monster was found and removed, false otherwise.
//  */
// bool kill_monster_at(GameContext *ctx, int x, int y);

bool on_hit_monster_cb(void *user_data, int x, int y);

#endif // COLLISION_H
