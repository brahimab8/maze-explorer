#include "menu/settings_menu.h"
#include "settings/settings.h"
#include "ui/ui.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// Display and handle the settings submenu
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

            // Print symbol settings as chars
            if (desc[i].setter == settings_set_player_symbol ||
                desc[i].setter == settings_set_exit_symbol) {
                snprintf(labels[i], 64, "%-15s  '%c'",
                         desc[i].name, (char)*desc[i].value_ptr);
            } else {
                // Regular integer
                snprintf(labels[i], 64, "%-15s  %d",
                         desc[i].name, *desc[i].value_ptr);
            }
        }
        labels[count] = strdup("Back");
        if (!labels[count]) goto cleanup_all;

        // Show menu
        int choice = ui->run_menu("Settings", (const char**)labels, total);
        if (choice == (int)count) break; 

        // Prompt for new value
        char buf[64];
        if (desc[choice].setter == settings_set_player_symbol ||
            desc[choice].setter == settings_set_exit_symbol) 
        {
            ui->print("Enter %s (any single character): ",
                      desc[choice].name);
            ui->read_line("", buf, sizeof buf);
            // take only the first character
            buf[0] = buf[0] ? buf[0] : *desc[choice].value_ptr;
        } else {
            snprintf(buf, sizeof buf, "Enter %s (%d–%d): ",
                     desc[choice].name,
                     desc[choice].min,
                     desc[choice].max);
            ui->read_line(buf, buf, sizeof buf);
        }

        int val = (desc[choice].setter == settings_set_player_symbol ||
                   desc[choice].setter == settings_set_exit_symbol)
                  ? (unsigned char)buf[0]
                  : atoi(buf);

        if (!desc[choice].setter(val)) {
            ui->print("Invalid value for %s. Must be %d–%d.\n",
                      desc[choice].name,
                      desc[choice].min,
                      desc[choice].max);
            ui->sleep_ms(1000);
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
