#include "menu/pause_menu.h"

PauseOption pause_menu(UI *ui) {
    static const char *labels[PAUSE_COUNT] = {
        "Continue",
        "Main Menu",
        "Quit"
    };
    int raw = ui->run_menu("Paused", labels, PAUSE_COUNT);
    if (raw < 0 || raw >= PAUSE_COUNT) raw = PAUSE_CONTINUE;
    return (PauseOption)raw;
}
