#include "game.h"
#include "save_game.h"
#include "ui.h"
#include <time.h>
#include <string.h>

void game_init(GameContext *g,
               const GameSettings *initial_cfg,
               UI *ui)
{
    // copy settings, clear grid
    g->cfg = *initial_cfg;
    g->grid = NULL;

    // seed and input via UI
    ui->seed_rng((unsigned)time(NULL));
    ui->read_line("Enter your name: ",
                  g->maze.player_name,
                  sizeof g->maze.player_name);

    if (!g->maze.player_name[0])
        strcpy(g->maze.player_name, "Player");

    g->maze.level = 1;
    g->maze.bullets = g->cfg.initial_shots;
    g->maze.time_secs = 0.0;
    g->frame_delay_ms = 1000 / g->cfg.fps;

    g->slot = strdup(g->maze.player_name);
    ui->save_slot(g->slot, &g->maze,
                  g->cfg.height, g->cfg.width,
                  g->maze.time_secs);
}
