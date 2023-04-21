#include "game/game.h"
#include "engine/maze.h"
#include "gameplay/play.h"
#include "util/save_game.h"
#include "util/timer.h"

#include <time.h>
#include <stdlib.h>
#include <string.h>

// local monster storage
static Monster monsters[MAX_MONSTERS];
static int     monster_count = 0;

// local item storage
static Item     items[MAX_ITEMS];
static int      item_count = 0;

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
        /* avoid player */   0, 0,
        /* symbol */        g->cfg.item_symbol
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

    int prev_level = g->maze.level;
    double total_time = g->maze.time_secs;
    int bullets_left = g->maze.bullets;

    g->maze.level++;

    // save progress
    ui->save_slot(g->slot, &g->maze,
                  g->cfg.height, g->cfg.width,
                  total_time);

    ui->clear_screen();

    // summary header
    ui->print("=== Level %d Complete ===\n", prev_level);
    ui->print("Time:      %02d:%02d.%d\n",
              (int)(total_time/60),
              (int)total_time % 60,
              (int)((total_time - (int)total_time)*10));
    ui->print("Bullets:   %d remaining\n", bullets_left);
    ui->print("Next Level: %d\n\n", g->maze.level);
    ui->print("Press Enter to start Level %d…\n", g->maze.level);

    // wait until the user presses Enter
    while (ui->poll_input() != INP_SELECT) {
        ui->sleep_ms(20);
    }

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
            do {
                state = play(&g, ui);
            } while (state == STATE_PLAY_LEVEL);
            break;

          case STATE_TRANSITION:
            state = do_transition(&g, ui);
            break;
        
          case STATE_GAME_OVER:
            ui->clear_screen();
            ui->print("Game Over!\nYou reached level %d total time: %02d:%02d\nPress any key to return to menu…",
                    g.maze.level,
                    (int)g.maze.time_secs/60,
                    (int)g.maze.time_secs%60);
            g.grid = NULL;

            while (ui->poll_input() == INP_NONE) {
                ui->sleep_ms(50);
            }
            state = STATE_MENU;
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
    // Clear any existing grid
    if (g->grid) {
        free_maze(g->grid);
        g->grid = NULL;
    }

    // Allocate & carve a new maze
    init_maze(g->cfg.height, g->cfg.width, &g->grid);
    carve   (g->grid, g->cfg.height, g->cfg.width, 0, 0);

    // Reset ammo and timer
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

