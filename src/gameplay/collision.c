#include "game/game.h"
#include "gameplay/monster.h"

// Remove a monster at (x,y) in the given context.
// Returns true if a monster was there.
bool kill_monster_at(GameContext *ctx, int x, int y) {
    Monster *mons = ctx->maze.monsters;
    int      *n   = &ctx->maze.monster_count;
    for (int i = 0; i < *n; ++i) {
        if (mons[i].x == x && mons[i].y == y) {
            mons[i] = mons[*n - 1];
            --*n;
            return true;
        }
    }
    return false;
}

// Callback for when a projectile hits a monster.
bool on_hit_monster_cb(void *user_data, int x, int y) {
    // user_data must be a GameContext*
    return kill_monster_at((GameContext *)user_data, x, y);
}

