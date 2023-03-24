#include "game/game.h"
#include "menu/pause_menu.h"
#include "util/timer.h"     
#include "ui/ui.h"
#include <unistd.h> 

GameState game_play(GameContext *g, UI *ui) {
    // Update elapsed time in the UI
    g->maze.time_secs = timer_get_elapsed();

    // Draw current maze state
    draw_maze(g->grid, g->cfg.height, g->cfg.width, &g->maze);

    // Handle input
    InputAction act = ui->poll_input();

    if (act == INP_PLAY) {
        timer_pause();

        static const char *labels[PAUSE_COUNT] = {
            "Continue",
            "Main Menu",
            "Quit"
        };
        int raw = ui->run_menu("Paused", labels, PAUSE_COUNT);
        switch ((PauseOption)raw) {
            case PAUSE_CONTINUE:
                timer_resume();
                return STATE_PLAY_LEVEL;
            case PAUSE_MAINMENU:
                return STATE_MENU;
            case PAUSE_QUIT:
                return STATE_EXIT;
            default:
                return STATE_PLAY_LEVEL;
        }
    }
    else if (act == INP_QUIT) {
        return STATE_EXIT;
    }

    // TODO: Maze movement logic
    
    ui->sleep_ms(g->frame_delay_ms);

    return STATE_PLAY_LEVEL;
}
