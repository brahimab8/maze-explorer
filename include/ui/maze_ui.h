#ifndef MAZE_UI_H
#define MAZE_UI_H

#include "gameplay/projectile.h"
#include "gameplay/monster.h"

typedef struct Cell Cell;

// UI state, rendered alongside the maze
typedef struct {
    char   player_name[16];
    int    level;
    int    bullets;
    double time_secs;

    int    player_x, player_y;   
    char   player_symbol;    

    int    exit_x,   exit_y;     
    char   exit_symbol;    

    Projectile projectiles[MAX_PROJECTILES];
    int          projectile_count;
    char         projectile_symbol;

    Monster monsters[MAX_MONSTERS];
    int     monster_count;
    char    monster_symbol;
} MazeUI;

// Draw the ASCII maze and the UI panel.
void draw_maze(Cell **grid, int rows, int cols, const MazeUI *ui);

#endif // MAZE_UI_H
