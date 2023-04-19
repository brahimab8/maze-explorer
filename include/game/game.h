#ifndef GAME_H
#define GAME_H

#include "settings/settings.h"  
#include "gameplay/player.h" 
#include "gameplay/monster.h"
#include "gameplay/projectile.h"
#include "gameplay/item.h" 
#include "ui/maze_ui.h"          
#include "ui/ui.h"          
#include <stdbool.h>  

typedef struct Cell Cell;

/** Finite‐state machine states **/
typedef enum {
    STATE_MENU,
    STATE_SETUP_LEVEL,
    STATE_PLAY_LEVEL,
    STATE_TRANSITION,
    STATE_GAME_OVER, 
    STATE_EXIT
} GameState;

/** All mutable data for one game session **/
typedef struct {
    GameSettings cfg;
    MazeUI       maze;
    Cell       **grid;
    Player       player;
    Projectile   projectiles[MAX_PROJECTILES];
    int          projectile_count;
    Monster      monsters[MAX_MONSTERS];
    int          monster_count;
    Item         items[MAX_ITEMS];
    int          item_count;
    
    char        *slot;
    int          frame_delay_ms;
} GameContext;

/* Core game functions */
void game_init(GameContext *g,
               const GameSettings *initial_cfg,
               UI *ui);
GameState game_menu(GameContext *g, UI *ui);
void run_game(const GameSettings *initial_cfg, UI *ui);


void game_clear_grid(GameContext *g);
void game_setup_grid(GameContext *g);

#endif // GAME_H
