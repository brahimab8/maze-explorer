#include "util/save_game.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

bool save_game_slot(const char *slot_name,
                    const MazeUI *maze,
                    int rows,
                    int cols,
                    double elapsed_secs)
{
    // Ensure the saves/ directory exists
    if (mkdir("saves", 0755) < 0 && errno != EEXIST) {
        return false;
    }

    // Build the path: saves/<slot_name>.sav
    char path[256];
    snprintf(path, sizeof path, "saves/%s.sav", slot_name);

    FILE *f = fopen(path, "wb");
    if (!f) return false;

    SaveState s = {
        .level     = maze->level,
        .rows      = rows,
        .cols      = cols,
        .bullets   = maze->bullets,
        .elapsed_secs  = elapsed_secs
    };

    bool ok = fwrite(&s, sizeof s, 1, f) == 1;
    fclose(f);
    return ok;
}

bool load_game_slot(const char *slot_name,
                    MazeUI *out_maze,
                    int *out_rows,
                    int *out_cols,
                    double *out_elapsed_secs)
{
    char path[256];
    snprintf(path, sizeof path, "saves/%s.sav", slot_name);

    FILE *f = fopen(path, "rb");
    if (!f) return false;

    SaveState s;
    bool ok = fread(&s, sizeof s, 1, f) == 1;
    fclose(f);
    if (!ok) return false;

    out_maze->level          = s.level;
    *out_rows              = s.rows;
    *out_cols              = s.cols;
    out_maze->bullets        = s.bullets;
    *out_elapsed_secs    = s.elapsed_secs; 
    // Preserve the player name in maze ->player_name
    return true;
}
