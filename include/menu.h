#ifndef MENU_H
#define MENU_H

#include "input.h"

typedef int MenuOption;
MenuOption run_menu(const char *title, const char *labels[], int count);

#endif // MENU_H
