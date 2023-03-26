#include "gameplay/play.h"

// #include "gameplay/player.h"
// #include "gameplay/monster.h"
// #include "gameplay/projectile.h"
// #include "gameplay/collision.h"
#include "menu/pause_menu.h"  

#include "ui/maze_ui.h" 
#include "util/timer.h"      

GameState play(GameContext *g, UI *ui) {
    timer_resume();

    // update elapsed time
    g->maze.time_secs = timer_get_elapsed();

    // redraw everything
    ui->clear_screen();
    draw_maze(g->grid,
                g->cfg.height,
                g->cfg.width,
                &g->maze);

    // get one input
    InputAction act = ui->poll_input();

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
