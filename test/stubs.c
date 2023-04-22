#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Menu widgets */
#include "menu/menu.h"               // run_menu()
#include "menu/load_menu.h"          // run_load_menu_ui()
#include "menu/settings_menu.h"      // run_settings_menu_ui()

/* Persistence */
#include "util/save_game.h"   // save_game_slot(), load_game_slot()

/* Engine & UI */
#include "engine/maze.h"             // Cell, free_maze()
#include "ui/maze_ui.h"              // MazeUI
#include "ui/ui.h"                   // UI

/* Configuration & settings */
#include "config.h"                  // global config
#include "settings/settings.h"       // GameSettings

/* Game logic */
#include "game/game.h"               // GameContext, GameState

// Shared stub state for run_menu
int menu_calls = 0;
static int  _next_choice = 0;
static char *_next_slot   = NULL;

void set_next_menu_choice(int c)       { _next_choice = c; }
void set_next_load_slot(const char *s)  { _next_slot   = (char*)s; }

//---Wrappers -----------------------------------------------------
int __wrap_run_menu(const char *t,const char*lbls[],int cnt) {
    (void)t;(void)lbls;(void)cnt;
    menu_calls++;
    return _next_choice;
}

char* __wrap_run_load_menu(UI *ui){ (void)ui; 
    return _next_slot ? strdup(_next_slot) : NULL;
}

bool __wrap_load_game_slot(const char *slot,
                           MazeUI *ui,int *r,int *c,double *e){
    (void)slot;
    ui->level     = 2;
    ui->bullets   = 5;
    *r            = 8;
    *c            = 9;
    *e            = 1.23;
    return true;
}

bool __wrap_save_game_slot(const char*slot,
                           const MazeUI*ui,int r,int c,double e){
    (void)slot; (void)ui; (void)r; (void)c; (void)e;
    return true;
}

void __wrap_draw_maze(Cell **grid,
                      int rows,
                      int cols,
                      const MazeUI *ui)
{
    (void)grid; (void)rows; (void)cols; (void)ui;
    /* no-op */
}

void __wrap_free_maze(Cell **g){ (void)g; }
void __wrap_run_settings_menu(UI *ui){(void)ui; }
void __wrap_input_get_line(const char*p,char*b,size_t l){
    (void)p;(void)l; b[0]='\0';
}

//---Stub out game_init so frame_delay_ms never divides by zero
void __wrap_game_init(GameContext *g,
                      const GameSettings *initial_cfg,
                      UI *ui)
{
    (void)ui;
    // Copy over the provided initial_cfg
    g->cfg = *initial_cfg;

    // Compute frame_delay_ms safely
    int fps = g->cfg.fps;
    if (fps < MIN_FPS || fps > MAX_FPS) {
        fps = DEFAULT_FPS;
    }
    g->frame_delay_ms = 1000 / fps;

    // Initialize MazeUI to sane defaults
    g->maze.level     = 1;
    g->maze.bullets   = g->cfg.initial_shots;
    g->maze.time_secs = 0.0;

    // No maze built yet
    g->grid = NULL;

    // Provide a dummy slot name
    g->slot = strdup("stub");
}

void __wrap_monsters_init(Monster *mons, int *count,
                          int level, int rows, int cols) {
    (void)mons;
    (void)count;
    (void)level;
    (void)rows;
    (void)cols;
    *count = 0;
}

void __wrap_items_init(Item *items, int *count,
                       int rows, int cols,
                       int avoid_x, int avoid_y,
                       char symbol) {
    (void)items;
    (void)count;
    (void)rows;
    (void)cols;
    (void)avoid_x;
    (void)avoid_y;
    (void)symbol;
    *count = 0;
}


/* -----------------------------------------------------------------------------
   A tiny input‐queue + fallback default for InputAction
   ----------------------------------------------------------------------------- */

#define INPUT_Q_SIZE  32
static InputAction _input_q[INPUT_Q_SIZE];
static int         _q_head = 0, _q_tail = 0;

// default action once the queue is drained
static InputAction _default_input = INP_NONE;
void set_default_input(InputAction a) {
    _default_input = a;
}

void reset_input_queue(void) {
    _q_head = _q_tail = 0;
}

void enqueue_input(InputAction a) {
    int next = (_q_tail + 1) % INPUT_Q_SIZE;
    if (next == _q_head) {
        // drop oldest
        _q_head = (next + 1) % INPUT_Q_SIZE;
    }
    _input_q[_q_tail] = a;
    _q_tail = next;
}

InputAction __wrap_input_poll(void) {
    if (_q_head != _q_tail) {
        InputAction a = _input_q[_q_head];
        _q_head = (_q_head + 1) % INPUT_Q_SIZE;
        return a;
    }
    // queue empty → return our tests’ default
    return _default_input;
}

// -----------------------------------------------------------------------------
// Suppress waits & clear-screens
// -----------------------------------------------------------------------------
int __wrap_system(const char *cmd){
    (void)cmd;
    return 0;
}

typedef unsigned int useconds_t;

int __wrap_usleep(useconds_t usec){
    (void)usec;
    return 0;
}



// gameplay tests stubs
InputAction next_input = INP_NONE;
void stub_clear_screen(void) {
    /* no-op */
}

InputAction stub_poll_input(void) {
    return next_input;
}

int stub_run_menu(const char *title, const char *labels[], int count) {
    return __wrap_run_menu(title, labels, count);
}

void stub_sleep_ms(int ms) {
    (void)ms;
}
