#include "config.h"
#include "settings.h"
#include "maze.h"
#include "maze_ui.h"

#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

int main(void) {
    srand((unsigned)time(NULL));

    GameSettings cfg;
    settings_get(&cfg);

    MazeUI ui;
    snprintf(ui.player_name, sizeof(ui.player_name), "player1");
    ui.level   = 1;
    ui.bullets = cfg.initial_shots;
    ui.time_secs = 0.0;

    int rows = cfg.height;
    int cols = cfg.width;
    Cell **grid;

    init_maze(rows, cols, &grid);
    carve(grid, rows, cols, 0, 0);
    draw_maze(grid, rows, cols, &ui);

    free_maze(grid);
    return 0;
}
