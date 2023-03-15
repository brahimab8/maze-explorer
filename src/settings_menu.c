#include "settings_menu.h"
#include "settings.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// Display and handle the settings submenu, using only UI callbacks.
void run_settings_menu_ui(UI *ui) {
    const SettingDesc *desc = settings_descriptors();
    size_t count = settings_descriptor_count();
    size_t total = count + 1;  // +1 for “Back”

    char **labels = malloc(sizeof(*labels) * total);
    if (!labels) return;

    while (1) {
        // Build dynamic labels
        for (size_t i = 0; i < count; i++) {
            labels[i] = malloc(64);
            if (!labels[i]) goto cleanup_all;
            snprintf(labels[i], 64, "%-15s  %d",
                     desc[i].name, *desc[i].value_ptr);
        }
        labels[count] = strdup("Back");
        if (!labels[count]) goto cleanup_all;

        // Show menu
        int choice = ui->run_menu("Settings", (const char**)labels, total);

        if (choice == (int)count) {
            // Back → exit
            break;
        }

        // Prompt for new value
        char buf[64];
        snprintf(buf, sizeof buf, "Enter %s (%d–%d): ",
                 desc[choice].name, desc[choice].min, desc[choice].max);
        ui->read_line(buf, buf, sizeof buf);

        int val = atoi(buf);
        if (!desc[choice].setter(val)) {
            ui->print("Invalid value for %s. Must be %d–%d.\n",
                      desc[choice].name, desc[choice].min, desc[choice].max);
            ui->sleep_ms(1000);  // pause 1s so user can see
        }

        // Free this iteration’s labels before rebuilding
        for (size_t i = 0; i < total; i++) {
            free(labels[i]);
        }
    }

cleanup_all:
    for (size_t i = 0; i < total; i++) {
        free(labels[i]);
    }
    free(labels);
}
