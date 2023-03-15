#include "config_file.h"
#include "settings.h"
#include "config.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bool config_load(const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f)
        return false; // no config file → use defaults

    char line[128];
    while (fgets(line, sizeof line, f))
    {
        // Skip empty or comment lines
        if (line[0] == '\n' || line[0] == '#')
            continue;

        // split key=value
        char *eq = strchr(line, '=');
        if (!eq)
            continue;
        *eq = '\0';
        char *key = line;
        char *val = eq + 1;
        // strip newline
        char *nl = strpbrk(val, "\r\n");
        if (nl)
            *nl = '\0';

        int v = atoi(val);
        if (strcmp(key, "FPS") == 0)
        {
            settings_set_fps(v);
        }
    }
    fclose(f);
    return true;
}

bool config_save(const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f)
        return false;

    GameSettings cfg;
    settings_get(&cfg);

    // Persist FPS
    fprintf(f, "FPS=%d\n", cfg.fps);

    fclose(f);
    return true;
}
