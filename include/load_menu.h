#ifndef LOAD_MENU_H
#define LOAD_MENU_H

#include "ui.h"

/**
 * Present the “Load Game” menu using the UI abstraction.
 * Returns a freshly‐allocated slot name (without “.sav”), or NULL if none.
 */
char *run_load_menu_ui(UI *ui);

#endif // LOAD_MENU_H
