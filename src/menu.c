#include "menu.h"
#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const char *MAIN_MENU_LABELS[MAIN_MENU_COUNT] = {
    "Play", "Load Game", "Settings", "Quit"
};

const char *PAUSE_MENU_LABELS[PAUSE_MENU_COUNT] = {
    "Continue", "Main Menu", "Quit"
};

MenuOption run_menu(const char *title, const char *labels[], int count) {
    int sel = 0;
    while (1) {
        system("clear");
        printf("=== %s ===\n\n", title);
        for (int i = 0; i < count; ++i) {
            printf(" %c %s\n", i == sel ? '>' : ' ', labels[i]);
        }
        puts("\nUse W/S to move; Enter to select; P=Play/Continue, Q=Quit.");

        InputAction act = input_poll();
        if (act == INP_PLAY)   return 0;         
        if (act == INP_QUIT)   return count-1;
        if (act == INP_UP)     sel = (sel - 1 + count) % count;
        if (act == INP_DOWN)   sel = (sel + 1) % count;
        if (act == INP_SELECT) return sel;

        usleep(100000);
    }
}
