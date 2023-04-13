#include "settings/settings.h"
#include "config.h"

// Backing storage for each setting:
static int g_maze_width     = MAZE_DEFAULT_WIDTH;
static int g_maze_height    = MAZE_DEFAULT_HEIGHT;
static int g_initial_shots  = INITIAL_SHOTS;
static int g_fps            = DEFAULT_FPS;
static int g_player_symbol   = DEFAULT_PLAYER_SYMBOL;
static int g_exit_symbol     = DEFAULT_EXIT_SYMBOL;
static int g_projectile_symbol = DEFAULT_PROJECTILE_SYMBOL;
static int g_monster_symbol    = DEFAULT_MONSTER_SYMBOL;

static bool symbol_setter(int *field, int c) {
    if (c < 32 || c > 126) return false;
    *field = c;
    return true;
}

// Table of all settings
static SettingDesc g_settings[] = {
    { "Width",          &g_maze_width,       settings_set_width,         MAZE_MIN_SIZE,        MAZE_MAX_SIZE },
    { "Height",         &g_maze_height,      settings_set_height,        MAZE_MIN_SIZE,        MAZE_MAX_SIZE },
    { "Initial Shots",  &g_initial_shots,    settings_set_initial_shots, MIN_INITIAL_SHOTS,    MAX_INITIAL_SHOTS },
    { "FPS",            &g_fps,              settings_set_fps,           MIN_FPS,              MAX_FPS },
    { "Player Symbol", &g_player_symbol,     settings_set_exit_symbol,   32,                   126 },
    { "Exit Symbol",   &g_exit_symbol,       settings_set_exit_symbol,   32,                   126 },
    { "Projectile Sym", &g_projectile_symbol, settings_set_projectile_symbol, 32,               126 },
    { "Monster Symbol",   &g_monster_symbol,    settings_set_monster_symbol, 32,                 126 },

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
    out->player_symbol   = (char)g_player_symbol;
    out->exit_symbol     = (char)g_exit_symbol;
    out->projectile_symbol = (char)g_projectile_symbol;
    out->monster_symbol    = (char)g_monster_symbol;
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

bool settings_set_player_symbol(int c) {
    return symbol_setter(&g_player_symbol, c);
}
bool settings_set_exit_symbol(int c) {
    return symbol_setter(&g_exit_symbol, c);
}

bool settings_set_projectile_symbol(int c) { 
    return symbol_setter(&g_projectile_symbol, c); 
}

bool settings_set_monster_symbol(int c)  { 
    return symbol_setter(&g_monster_symbol, c); 
}
