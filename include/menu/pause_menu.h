#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include "ui/ui.h"

typedef enum {
    PAUSE_CONTINUE = 0,
    PAUSE_MAINMENU = 1,
    PAUSE_QUIT     = 2,
    PAUSE_COUNT
} PauseOption;

PauseOption pause_menu(UI *ui);

#endif // PAUSE_MENU_H
