#ifndef MENU_H
#define MENU_H

#include <stdbool.h>

// Generic menu selection index
typedef int MenuOption;

extern const char *MAIN_MENU_LABELS[];
extern const char *PAUSE_MENU_LABELS[];

enum { MAIN_MENU_COUNT = 4, PAUSE_MENU_COUNT = 3 };

MenuOption run_menu(const char *title, const char *labels[], int count);

#endif // MENU_H
