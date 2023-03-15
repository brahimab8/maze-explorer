#ifndef SETTINGS_MENU_H
#define SETTINGS_MENU_H

#include "ui.h"

/**
 * Show the settings submenu via UI callbacks.
 * Calls ui->run_menu, ui->read_line, ui->print, ui->sleep_ms.
 */
void run_settings_menu_ui(UI *ui);

#endif // SETTINGS_MENU_H
