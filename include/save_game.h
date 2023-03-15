#ifndef SAVE_GAME_H
#define SAVE_GAME_H

#include "maze_ui.h"
#include <stdbool.h>

/**
 * State to write/read for a save slot.
 */
typedef struct {
    int     level;          // current level
    int     rows, cols;     // maze dimensions
    int     bullets;        // remaining shots
    double   elapsed_secs;  // game time
} SaveState;

/**
 * Save the current game state into `saves/<slot_name>.sav`.
 * Returns true on success.
 */
bool save_game_slot(const char *slot_name,
                    const MazeUI *ui,
                    int rows,
                    int cols,
                    double elapsed_secs);

/**
 * Load a previously saved slot into the provided outputs.
 * Returns true if the slot exists and was read successfully.
 */
bool load_game_slot(const char *slot_name,
                    MazeUI *out_ui,
                    int *out_rows,
                    int *out_cols,
                    double *out_elapsed_secs);

#endif // SAVE_GAME_H
