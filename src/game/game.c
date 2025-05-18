#include "game/game.h"
#include "engine/maze.h"
#include "gameplay/play.h"
#include "util/save_game.h"
#include "util/timer.h"

#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// local monster storage
static Monster monsters[MAX_MONSTERS];
static int     monster_count = 0;

// local item storage
static Item     items[MAX_ITEMS];
static int      item_count = 0;

// Helper to display a message screen and wait for input
static GameState show_message_screen(UI *ui,
                                     GameState next_state,
                                     size_t line_count,
                                     ...)
{
    va_list args;
    ui->clear_screen();

    va_start(args, line_count);
    for (size_t i = 0; i < line_count; ++i) {
        const char *line = va_arg(args, const char *);
        ui->print("%s\n", line);
    }
    va_end(args);

    InputAction act = INP_NONE;
    // wait until user presses Enter or 'q'
    while (act != INP_SELECT && act != INP_QUIT) {
        act = ui->poll_input();
        ui->sleep_ms(20);
    }

    // if they hit 'q', exit the whole game
    if (act == INP_QUIT) {
        return STATE_EXIT;
    }
    // else (Enter) continue
    return next_state;
}


static GameState setup_level(GameContext *g, UI *ui) {
    (void)ui;   // silence “unused parameter” warning

    // reset maze
    game_setup_grid(g);

    if (g->maze.level == 1) {
        // first level: give the player a fresh load of ammo
        player_init(
            &g->player,
            /* sx */ 0,
            /* sy */ 0,
            /* initial_bullets */ g->cfg.initial_shots,
            /* symbol */          g->cfg.player_symbol
        );
    } else {
        // subsequent levels
        g->player.x = 0;
        g->player.y = 0;
        g->player.dir = DIR_DOWN;
    }
    g->maze.player_symbol = g->player.symbol;
    g->maze.player_x      = g->player.x;
    g->maze.player_y      = g->player.y;
    g->maze.bullets       = g->player.bullets;

    g->maze.exit_x        = g->cfg.width  - 1;
    g->maze.exit_y        = g->cfg.height - 1;

    g->maze.projectile_count  = 0;

    g->maze.monster_count = 0;

    // (Re)spawn the monsters for this level
    monsters_init(
        monsters, &monster_count,
        g->maze.level,
        g->cfg.height,
        g->cfg.width
    );
    // Push into UI layer
    memcpy(g->maze.monsters,
           monsters,
           monster_count * sizeof monsters[0]);
    g->maze.monster_count  = monster_count;

    // (Re)spawn the items for this level
    items_init(
        items, &item_count,
        g->cfg.height,       // rows
        g->cfg.width,        // cols
        0, 0,                // avoid player at
        g->cfg.item_symbol   // item symbol
    );

    // Push into UI layer
    memcpy(g->maze.items,
           items,
           item_count * sizeof items[0]);
    g->maze.item_count = item_count;

    return STATE_PLAY_LEVEL;
}

static GameState do_transition(GameContext *g, UI *ui) {
    timer_pause();
    free_maze(g->grid);
    g->grid = NULL;

    int prev_level   = g->maze.level;
    double total_time = g->maze.time_secs;
    int bullets_left = g->maze.bullets;
    g->maze.level++;

    ui->save_slot(g->slot, &g->maze,
                  g->cfg.height, g->cfg.width,
                  total_time);

    char h1[64], h2[64], h3[64], h4[64];
    snprintf(h1, sizeof h1, "=== Level %d Complete ===", prev_level);
    snprintf(h2, sizeof h2,
             "Time:      %02d:%02d.%d",
             (int)(total_time/60), (int)total_time % 60,
             (int)((total_time - (int)total_time)*10));
    snprintf(h3, sizeof h3, "Bullets:   %d remaining", bullets_left);
    snprintf(h4, sizeof h4, "Next Level: %d", g->maze.level);

    return show_message_screen(
        ui,
        STATE_SETUP_LEVEL,
        6,
        h1,
        h2,
        h3,
        h4,
        "",
        "Press Enter to start next level or 'q' to quit"
    );
}

static GameState do_game_over(GameContext *g, UI *ui) {
    char o1[64], o2[64];
    snprintf(o1, sizeof o1,
             "Game Over! You reached level %d", g->maze.level);
    snprintf(o2, sizeof o2,
             "Total time: %02d:%02d",
             (int)g->maze.time_secs/60,
             (int)g->maze.time_secs%60);

    // clear the maze grid after game over
    if (g->grid) {
        free_maze(g->grid);
        g->grid = NULL;
    }

    return show_message_screen(
        ui,
        STATE_MENU,
        4,
        o1,
        o2,
        "",
        "Press Enter to return to menu or 'q' to quit"
    );
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
            do {
                state = play(&g, ui);
            } while (state == STATE_PLAY_LEVEL);
            break;

          case STATE_TRANSITION:
            state = do_transition(&g, ui);
            break;

          case STATE_GAME_OVER:
            state = do_game_over(&g, ui);
            break;

          case STATE_EXIT:
            break;

          default:
            state = STATE_EXIT;
        }
    }

    if (g.grid) free_maze(g.grid);
    free(g.slot);
}

void game_setup_grid(GameContext *g) {
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
}

void game_clear_grid(GameContext *g) {
    if (g->grid) {
        free_maze(g->grid);
        g->grid = NULL;
    }
}
