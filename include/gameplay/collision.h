#ifndef COLLISION_H
#define COLLISION_H

#include <stdbool.h>
#include "game/game.h"
#include "gameplay/monster.h"

/** Remove a monster at (x,y) in the given context. */
bool kill_monster_at(GameContext *ctx, int x, int y);

/** Callback for projectile‐monster collisions. */
bool on_hit_monster_cb(void *user_data, int x, int y);

#endif // COLLISION_H
