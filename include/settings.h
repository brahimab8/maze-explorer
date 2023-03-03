// settings.h
#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdbool.h>
#include "config.h"

// Runtime‐modifiable settings snapshot
typedef struct {
    int width;          // [MAZE_MIN_SIZE, MAZE_MAX_SIZE]
    int height;         // [MAZE_MIN_SIZE, MAZE_MAX_SIZE]
    int initial_shots;  // starting bullets
} GameSettings;

// Get current settings
void settings_get(GameSettings *out);

// Setters (return false if out of range)
bool settings_set_width(int w);
bool settings_set_height(int h);
bool settings_set_initial_shots(int n);

#endif // SETTINGS_H
