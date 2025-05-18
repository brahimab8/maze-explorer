#include "game/game.h"
#include "util/save_game.h"
#include "gameplay/player.h" 
#include "ui/ui.h"

#include <time.h>
#include <string.h>

void game_init(GameContext *g,
               const GameSettings *initial_cfg,
               UI *ui)
{
    
    ui->clear_screen();

    ui->print(
    "    __  ___                   ______           __                    \n"
    "   /  |/  /____ ____ ___     / ____/  __ ____ / /____ _____ __  _____\n"
    "  / /|_/ / __  /_  // _ |   / __/ | |/ / __  / / __  / ___/ _ |/ ___/\n"
    " / /  / / /_/ / / //  __/  / /___ > < / /_/ / / /_/ / /  /  __/ /    \n"
    "/_/  /_/_____/ /___|___/  /_____/__/|/ ____/_/_____/_/  /____/_/     \n"
    "                                    /_/                              \n\n"
    );

    // copy settings, clear grid
    g->cfg = *initial_cfg;
    // start out at zero / empty
    memset(&g->maze, 0, sizeof g->maze);
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

    // // Pull the dynamic symbols straight from the config
    g->maze.player_symbol     = g->cfg.player_symbol;
    g->maze.exit_symbol       = g->cfg.exit_symbol;
    g->maze.projectile_symbol = g->cfg.projectile_symbol;
    g->maze.monster_symbol    = g->cfg.monster_symbol;
    g->maze.item_symbol       = g->cfg.item_symbol;

    g->projectile_count = 0;

    g->slot = strdup(g->maze.player_name);
    ui->save_slot(g->slot, &g->maze,
                  g->cfg.height, g->cfg.width,
                  g->maze.time_secs);
}
