#include "game/game.h"
#include "engine/maze.h"
#include "util/save_game.h"
#include "util/timer.h"
#include <time.h>
#include <stdlib.h>

static GameState setup_level(GameContext *g, UI *ui) {
    (void)ui;   // silence “unused parameter” warning

    if (g->grid) {
        free_maze(g->grid);
        g->grid = NULL;
    }
    init_maze(g->cfg.height, g->cfg.width, &g->grid);
    carve(g->grid, g->cfg.height, g->cfg.width, 0, 0);

    g->maze.bullets = g->cfg.initial_shots;
    if (g->maze.time_secs > 0.0) {
        timer_set_start_time(g->maze.time_secs);
    } else {
        timer_start();
    }
    return STATE_PLAY_LEVEL;
}

static GameState do_transition(GameContext *g, UI *ui) {
    timer_pause();
    free_maze(g->grid);
    g->grid = NULL;

    g->maze.level++;
    ui->save_slot(g->slot, &g->maze,
                  g->cfg.height, g->cfg.width,
                  g->maze.time_secs);

    ui->clear_screen();
    ui->print("Level %d complete!\nPress any key for level %d…",
              g->maze.level - 1, g->maze.level);
    ui->poll_input();

    return STATE_SETUP_LEVEL;
}

void run_game(const GameSettings *initial_cfg, UI *ui) {
    ui->seed_rng((unsigned)time(NULL));
    GameContext g;
    game_init(&g, initial_cfg, ui);
    timer_pause();

    GameState state = STATE_MENU;
    while (state != STATE_EXIT) {
        switch (state) {
          case STATE_MENU:
            timer_pause();
            state = game_menu(&g, ui);
            break;

          case STATE_SETUP_LEVEL:
            state = setup_level(&g, ui);
            break;

          case STATE_PLAY_LEVEL:
            state = game_play(&g, ui);
            break;

          case STATE_TRANSITION:
            state = do_transition(&g, ui);
            break;

          default:
            state = STATE_EXIT;
        }
    }

    if (g.grid) free_maze(g.grid);
    free(g.slot);
}
