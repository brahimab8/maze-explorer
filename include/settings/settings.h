#ifndef SETTINGS_H
#define SETTINGS_H

#include "config.h"

#include <stdbool.h>
#include <stddef.h>


typedef struct {
    const char *name;
    int  *value_ptr;
    bool (*setter)(int newval);
    int   min, max;
} SettingDesc;

// Runtime‐modifiable settings snapshot
typedef struct {
    int width;          // [MAZE_MIN_SIZE, MAZE_MAX_SIZE]
    int height;         // [MAZE_MIN_SIZE, MAZE_MAX_SIZE]
    int initial_shots;  // starting bullets
    int fps;
    char player_symbol;
    char exit_symbol;
} GameSettings;

// Get current settings
void settings_get(GameSettings *out);

// Setters (return false if out of range)
bool settings_set_width(int w);
bool settings_set_height(int h);
bool settings_set_initial_shots(int n);
bool settings_set_fps(int f);
bool settings_set_player_symbol(int c);
bool settings_set_exit_symbol(int c);
// bool settings_set_monster_symbol(int c);
// bool settings_set_item_symbol(int c);

// Accessors for the metadata
const SettingDesc *settings_descriptors(void);
size_t settings_descriptor_count(void);

#endif // SETTINGS_H
