#include "util/config_file.h"
#include "settings/settings.h"

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
        
        if (strcmp(key, "Width") == 0) {
            settings_set_width(v);
        }
        else if (strcmp(key, "Height") == 0) {
            settings_set_height(v);
        }
        else if (strcmp(key, "InitialShots") == 0) {
            settings_set_initial_shots(v);
        }
        else if (strcmp(key, "FPS") == 0) {
            settings_set_fps(v);
        }
        else if (strcmp(key,"PlayerSymbol")==0 && val[0]){
            settings_set_player_symbol(val[0]);
        }
        else if (strcmp(key,"ExitSymbol")==0 && val[0]){
            settings_set_exit_symbol(val[0]);
        }
        else if (strcmp(key, "ProjectileSymbol") == 0 && val[0]) {
            settings_set_projectile_symbol((unsigned char)val[0]);
        }
        else if (strcmp(key, "MonsterSymbol") == 0 && val[0]) {
            settings_set_monster_symbol((unsigned char)val[0]);
        }
        else if (strcmp(key, "ItemSymbol") == 0 && val[0]) {
            settings_set_item_symbol((unsigned char)val[0]);
        }
        else if (strcmp(key, "ItemBonus") == 0) {
            settings_set_item_bonus(v);
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

    fprintf(f, "# Maze Explorer configuration\n");
    fprintf(f, "Width=%d\n",          cfg.width);
    fprintf(f, "Height=%d\n",         cfg.height);
    fprintf(f, "InitialShots=%d\n",   cfg.initial_shots);
    fprintf(f, "FPS=%d\n",            cfg.fps);
    fprintf(f, "PlayerSymbol=%c\n",   cfg.player_symbol);
    fprintf(f, "ExitSymbol=%c\n",     cfg.exit_symbol);
    fprintf(f, "ProjectileSymbol=%c\n", cfg.projectile_symbol);
    fprintf(f, "MonsterSymbol=%c\n",  cfg.monster_symbol);
    fprintf(f, "ItemSymbol=%c\n",     cfg.item_symbol);
    fprintf(f, "ItemBonus=%d\n",      cfg.item_bonus);

    fclose(f);
    return true;
}
