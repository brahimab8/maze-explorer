#include "settings/settings.h"
#include "config.h"

// Backing storage for each setting:
static int g_maze_width     = MAZE_DEFAULT_WIDTH;
static int g_maze_height    = MAZE_DEFAULT_HEIGHT;
static int g_initial_shots  = INITIAL_SHOTS;
static int g_fps            = DEFAULT_FPS;

// Table of all settings
static SettingDesc g_settings[] = {
    { "Width",          &g_maze_width,       settings_set_width,         MAZE_MIN_SIZE,        MAZE_MAX_SIZE },
    { "Height",         &g_maze_height,      settings_set_height,        MAZE_MIN_SIZE,        MAZE_MAX_SIZE },
    { "Initial Shots",  &g_initial_shots,    settings_set_initial_shots, MIN_INITIAL_SHOTS,     MAX_INITIAL_SHOTS },
    { "FPS",            &g_fps,              settings_set_fps,           MIN_FPS,              MAX_FPS },
};
static const size_t g_settings_count = sizeof g_settings / sizeof *g_settings;

const SettingDesc *settings_descriptors(void) {
    return g_settings;
}

size_t settings_descriptor_count(void) {
    return g_settings_count;
}

// Populate GameSettings struct
void settings_get(GameSettings *out) {
    out->width         = g_maze_width;
    out->height        = g_maze_height;
    out->initial_shots = g_initial_shots;
    out->fps           = g_fps;
}

// Individual setters
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
    if (n < MIN_INITIAL_SHOTS || n > MAX_INITIAL_SHOTS) return false;
    g_initial_shots = n;
    return true;
}

// New FPS setter
bool settings_set_fps(int f) {
    if (f < MIN_FPS || f > MAX_FPS) return false;
    g_fps = f;
    return true;
}
