#include "gameplay/play.h"

#include "gameplay/player.h"
// #include "gameplay/monster.h"
#include "gameplay/wall.h"
#include "menu/pause_menu.h"  

#include "ui/maze_ui.h" 
#include "util/timer.h"   

#include <stdbool.h>      
#include <string.h>

static Projectile   projectiles[MAX_PROJECTILES];
static int          projectile_count = 0;

// Returns true if the player is standing on the exit cell
static bool is_at_exit(const GameContext *g) {
    return (g->player.x == g->maze.exit_x) &&
           (g->player.y == g->maze.exit_y);
}

GameState play(GameContext *g, UI *ui) {
    timer_resume();

    // update elapsed time & UI
    g->maze.time_secs = timer_get_elapsed();
    g->maze.player_x     = g->player.x;
    g->maze.player_y     = g->player.y;
    g->maze.bullets       = g->player.bullets;

    // get one input
    InputAction act = ui->poll_input();

    // movement keys: update player position
    if (act == INP_UP || act == INP_DOWN ||
        act == INP_LEFT || act == INP_RIGHT) {
        player_move(&g->player,
                    act,
                    g->grid,
                    g->cfg.height,
                    g->cfg.width);
        // immediately check for exit
        if (is_at_exit(g))
            return STATE_TRANSITION;
    }

    if (act == INP_SHOOT) {
        player_shoot(&g->player, projectiles, &projectile_count);
    }

    //sync projectile buffer into UI
    memcpy(g->maze.projectiles,
           projectiles,
           projectile_count * sizeof projectiles[0]);
    g->maze.projectile_count = projectile_count;

    // Move all projectiles one step 
    projectile_update(
        projectiles, &projectile_count,
        g->grid, g->cfg.height, g->cfg.width,
        /*on_hit_wall=*/ wall_destroy,
        /*on_hit_monster=*/NULL
    );
    
    ui->clear_screen();
    draw_maze(g->grid,
              g->cfg.height,
              g->cfg.width,
              &g->maze);

    // handle pause/quit immediately
    if (act == INP_PLAY) {
        timer_pause();
        PauseOption choice = pause_menu(ui);
        switch ((PauseOption)choice) {
            case PAUSE_CONTINUE:
                timer_resume();
                return STATE_PLAY_LEVEL;
            case PAUSE_MAINMENU:
                return STATE_MENU;           // break out to main menu
            case PAUSE_QUIT:
                return STATE_EXIT;           // exit game
            case PAUSE_COUNT:    
            default:
                return STATE_PLAY_LEVEL;

        }
    }
    if (act == INP_QUIT) {
        return STATE_EXIT;
    }

    // // apply gameplay logic
    // player_apply_action(&g->maze, g->grid, act);
    // monster_move_all   (&g->maze, g->grid);
    // projectile_update  (&g->maze, g->grid, &g->maze.time_secs);

    // // collision & level‐complete checks
    // if (collision_player_hit(&g->maze, g->grid)) {
    //     return STATE_MENU;              // go back to menu on death
    // }
    // if (collision_level_complete(&g->maze, g->grid)) {
    //     return STATE_TRANSITION;        // advance level
    // }

    // frame‐delay before next turn
    ui->sleep_ms(g->frame_delay_ms);
    return STATE_PLAY_LEVEL;

}
