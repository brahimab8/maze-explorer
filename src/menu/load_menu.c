#include "menu/load_menu.h"
#include "ui/ui.h"

#include <dirent.h>
#include <string.h>
#include <stdlib.h>

// UI‐driven load menu. Scans “saves/” for “*.sav” files,
// presents them via ui->run_menu(), and returns the selected slot.
char *run_load_menu_ui(UI *ui) {
    DIR *d = opendir("saves");
    if (!d) return NULL;

    char *slots[64];
    int count = 0;
    struct dirent *ent;
    while ((ent = readdir(d)) && count < 64) {
        size_t len = strlen(ent->d_name);
        if (len > 4 && strcmp(ent->d_name + len - 4, ".sav") == 0) {
            slots[count] = strdup(ent->d_name);
            slots[count][len - 4] = '\0';  // chop off “.sav”
            count++;
        }
    }
    closedir(d);
    if (count == 0) return NULL;

    const char **labels = malloc(sizeof(*labels) * count);
    for (int i = 0; i < count; i++) {
        labels[i] = slots[i];
    }

    int choice = ui->run_menu("Load Game", labels, count);

    char *picked = NULL;
    if (choice >= 0 && choice < count) {
        picked = strdup(slots[choice]);
    }
    for (int i = 0; i < count; i++) {
        free(slots[i]);
    }
    free(labels);
    return picked;
}
