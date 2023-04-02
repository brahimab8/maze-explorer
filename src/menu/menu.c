#include "menu/menu.h"
#include "ui/ui.h" 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

MenuOption run_menu(const char *title, const char *labels[], int count) {
    int sel = 0;
    while (1) {
        system("clear");
        printf("=== %s ===\n\n", title);
        for (int i = 0; i < count; ++i) {
            printf(" %c %s\n", i == sel ? '>' : ' ', labels[i]);
        }
        puts("\nUse W/S to move; Enter to select; P=Play, Q=Quit.");

        InputAction act = input_poll();
        if (act == INP_UP)       sel = (sel - 1 + count) % count;
        else if (act == INP_DOWN) sel = (sel + 1) % count;
        else if (act == INP_SELECT) return sel;
        else if (act == INP_PLAY)   return 0;          // always “first” option
        else if (act == INP_QUIT)   return count - 1;  // always “last” option

        usleep(100000);
    }
}
