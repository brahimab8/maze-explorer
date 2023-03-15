#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>              // for nanosleep(), struct timespec
#include <unistd.h>            // for system()
#include <stdlib.h>            // for srand()

#include "ui.h"
#include "input.h"             // input_poll(), input_get_line()
#include "save_game.h"         // save_game_slot()
#include "menu.h"              // run_menu()
#include "load_menu.h"         // run_load_menu_ui()
#include "settings_menu.h"     // run_settings_menu_ui()

static void posix_clear(void) {
    system("clear");
}

static void posix_print(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
}

static void posix_sleep(int ms) {
    struct timespec ts;
    ts.tv_sec  = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

static void posix_seed(unsigned s) {
    srand(s);
}

static int posix_rand(int n) {
    return rand() % n;
}

static InputAction posix_poll(void) {
    return input_poll();
}

static void posix_read(const char *prompt, char *buf, int len) {
    input_get_line(prompt, buf, len);
}

static void posix_save(const char *slot,
                       const MazeUI *ui,
                       int rows, int cols,
                       double elapsed_secs)
{
    save_game_slot(slot, ui, rows, cols, elapsed_secs);
}

static int posix_run_menu(const char *title,
                          const char *labels[],
                          int count)
{
    return run_menu(title, labels, count);
}

static char *posix_run_load_menu(UI *ui) {
    return run_load_menu_ui(ui);
}

static void posix_run_settings_menu(UI *ui) {
    run_settings_menu_ui(ui);
}

void ui_init(UI *ui) {
    ui->clear_screen       = posix_clear;
    ui->print              = posix_print;
    ui->sleep_ms           = posix_sleep;
    ui->seed_rng           = posix_seed;
    ui->rand_int           = posix_rand;
    ui->poll_input         = posix_poll;
    ui->read_line          = posix_read;
    ui->save_slot          = posix_save;
    ui->run_menu           = posix_run_menu;
    ui->run_load_menu      = posix_run_load_menu;
    ui->run_settings_menu  = posix_run_settings_menu;
}
