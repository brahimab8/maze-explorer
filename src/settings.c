// settings.c
#include "settings.h"

// Backing storage initialized to compile‑time defaults
static int g_maze_width    = MAZE_DEFAULT_WIDTH;
static int g_maze_height   = MAZE_DEFAULT_HEIGHT;
static int g_initial_shots = INITIAL_SHOTS;

void settings_get(GameSettings *out) {
    out->width         = g_maze_width;
    out->height        = g_maze_height;
    out->initial_shots = g_initial_shots;
}

bool settings_set_width(int w) {
    if (w < MAZE_MIN_SIZE || w > MAZE_MAX_SIZE) return false;
    g_maze_width = w;
    return true;
}

bool settings_set_height(int h) {
    if (h < MAZE_MIN_SIZE || h > MAZE_MAX_SIZE) return false;
    g_maze_height = h;
    return true;
}

bool settings_set_initial_shots(int n) {
    if (n < 0) return false;
    g_initial_shots = n;
    return true;
}
