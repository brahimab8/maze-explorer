#include "config.h"
#include "settings.h"
#include "menu.h"
#include "maze.h"
#include "maze_ui.h"
#include "timer.h"
#include "input.h"

#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>


typedef enum {
    STATE_INIT,
    STATE_MENU,
    STATE_PLAY,
    STATE_EXIT
} GameState;


int main(void) {
    srand((unsigned)time(NULL));
    input_init();

    // Load runtime settings
    GameSettings cfg;
    settings_get(&cfg);

    // Initialize UI state
    MazeUI ui;
    ui.level     = 1;
    ui.bullets   = cfg.initial_shots;
    input_get_line("Enter your name: ", ui.player_name, sizeof(ui.player_name));
    if (ui.player_name[0] == '\0')
        strncpy(ui.player_name, "Player", sizeof(ui.player_name));
    
    int rows = cfg.height;
    int cols = cfg.width;
    Cell **grid;

    GameState state        = STATE_INIT;
    bool      running      = true;
    bool      timer_active = false;

    while (running) {
        InputAction act = input_poll();

        switch (state) {
            case STATE_INIT:
                init_maze(rows, cols, &grid);
                carve(grid, rows, cols, 0, 0);
                state = STATE_MENU;
                break;

            case STATE_MENU: {
                // Show the main menu
                int choice = run_menu("Main Menu", MAIN_MENU_LABELS, MAIN_MENU_COUNT);

                switch (choice) {
                    case 0:  // Play
                        if (!timer_active) {
                            timer_start();
                            timer_active = true;
                        } else {
                            timer_resume();
                        }
                        state = STATE_PLAY;
                        break;
                    case 1:  // Load Game
                        // TODO: load saved game
                        break;
                    case 2:  // Settings
                        // TODO: show settings submenu
                        break;
                    case 3:  // Quit
                        state = STATE_EXIT;
                        break;
                }
                break;
            }
                
            case STATE_PLAY:
                ui.time_secs = timer_get_elapsed();
                draw_maze(grid, rows, cols, &ui);

                if (act == INP_QUIT) {
                    state = STATE_EXIT;
                } else if (act == INP_PLAY) {
                    timer_pause();
                    // Invoke the pause menu
                    int p = run_menu("Paused", PAUSE_MENU_LABELS, PAUSE_MENU_COUNT);
                    switch (p) {
                        case 0:  // Continue
                            timer_resume();
                            break;
                        case 1:  // Main Menu
                            state = STATE_MENU;
                            break;
                        case 2:  // Quit
                            state = STATE_EXIT;
                            break;
                    }
                } else {
                    // TODO: movement logic
                }
            

                usleep(FRAME_US);
                break;

            case STATE_EXIT:
                running = false;
                break;
        }
    }

    free_maze(grid);
    return 0;
}
