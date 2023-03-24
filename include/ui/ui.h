#ifndef UI_H
#define UI_H

#include "ui/maze_ui.h"
#include "input/input.h"  
#include <stdarg.h>    // for va_list in print()

typedef struct UI {
    void        (*clear_screen)(void);
    void        (*print)(const char *fmt, ...);
    void        (*sleep_ms)(int ms);
    void        (*seed_rng)(unsigned seed);
    int         (*rand_int)(int n);
    InputAction (*poll_input)(void);
    void        (*read_line)(const char *prompt, char *buf, int len);
    void        (*save_slot)(const char *slot,
                              const MazeUI *ui,
                              int rows, int cols,
                              double elapsed);
    int         (*run_menu)(const char *title,
                             const char *labels[],
                             int count);
    char *      (*run_load_menu)(struct UI *ui);
    void        (*run_settings_menu)(struct UI *ui);
} UI;

/** Bind UI to POSIX calls (ui_posix.c) **/
void ui_init(UI *ui);

#endif // UI_H
