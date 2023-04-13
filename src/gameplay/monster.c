#include "gameplay/monster.h"
#include <stdlib.h>

static int clamp(int v, int lo, int hi) { return v<lo?lo:(v>hi?hi:v); }

int monster_count_for_level(int lvl) {
    int batch = (lvl - 1) / 5;
    int cnt   = 1 + batch;
    return cnt > MAX_MONSTERS ? MAX_MONSTERS : cnt;
}

int monster_base_cooldown_for_level(int lvl) {
    // mapping 1→5, 2→4, 3→3, 4→2, 5→1, then repeat
    int within = (lvl - 1) % 5;       // 0..4
    return clamp(5 - within, 1, 5);   // 5,4,3,2,1
}

void monsters_init(Monster *M, int *n_out, int lvl, int rows, int cols) {
    int count = monster_count_for_level(lvl);
    *n_out = count;

    int base_cd = monster_base_cooldown_for_level(lvl) * 5; // TODO
    for (int i = 0; i < count; i++) {
        int x, y;
        // keep retrying until we're at least 3 cols and 3 rows away from the player
        do {
            x = rand() % cols;
            y = rand() % rows;
            // assuming player always starts at (0,0) in setup_level
        } while (abs(x - 0) < 3 && abs(y - 0) < 3);

        M[i].x = x;
        M[i].y = y;
        M[i].base_cooldown = base_cd;
        M[i].cooldown      = base_cd;
    }
}

static bool try_step(Monster *m, Cell **g, int rows, int cols,
                     int dx, int dy, Direction dir) {
    int nx = m->x + dx, ny = m->y + dy;
    if (nx<0||nx>=cols||ny<0||ny>=rows) return false;
    if (!can_step(g, rows, cols, m->x, m->y, dir)) return false;
    m->x = nx; m->y = ny;
    return true;
}

void monsters_step(Monster *list, int n,
                   int px, int py,
                   Cell **grid, int rows, int cols)
{
    for (int i = 0; i < n; i++) {
        Monster *m = &list[i];
        if (--m->cooldown > 0) continue;

        // choose major axis first
        int dx = px - m->x, dy = py - m->y;
        bool moved = false;
        if (abs(dx) >= abs(dy)) {
            if (dx > 0)   moved = try_step(m, grid, rows, cols, +1, 0, DIR_RIGHT);
            else if (dx < 0) moved = try_step(m, grid, rows, cols, -1, 0, DIR_LEFT);
            if (!moved && dy) {
                if (dy > 0)   moved = try_step(m, grid, rows, cols, 0, +1, DIR_DOWN);
                else           moved = try_step(m, grid, rows, cols, 0, -1, DIR_UP);
            }
        } else {
            if (dy > 0)   moved = try_step(m, grid, rows, cols, 0, +1, DIR_DOWN);
            else if (dy < 0) moved = try_step(m, grid, rows, cols, 0, -1, DIR_UP);
            if (!moved && dx) {
                if (dx > 0)   moved = try_step(m, grid, rows, cols, +1, 0, DIR_RIGHT);
                else           moved = try_step(m, grid, rows, cols, -1, 0, DIR_LEFT);
            }
        }

        // reset for next time
        m->cooldown = m->base_cooldown;
    }
}
