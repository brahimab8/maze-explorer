#ifndef MAIN_MENU_H
#define MAIN_MENU_H

typedef enum {
    MAIN_CONTINUE,   // resume current game
    MAIN_NEW,        // start a new game
    MAIN_LOAD,       // load from slot
    MAIN_SETTINGS,   // open settings menu
    MAIN_QUIT,       // exit game
    MAIN_COUNT
} MainOption;

#endif // MAIN_MENU_H
