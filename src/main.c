#include "config.h"
#include "settings.h"
#include "config_file.h"
#include "game.h"
#include "ui.h"

int main(void) {
    config_load(CONFIG_FILE_PATH);

    GameSettings cfg;
    settings_get(&cfg);

    UI ui;
    ui_init(&ui);

    run_game(&cfg, &ui);

    config_save(CONFIG_FILE_PATH);

    return 0;
}
