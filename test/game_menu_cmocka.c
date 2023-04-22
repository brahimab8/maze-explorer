#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <cmocka.h>

#include "game/game.h"
#include "menu/main_menu.h"     /* for MAIN_CONTINUE, MAIN_NEW, etc. */
#include "util/save_game.h"     /* for save_game_slot() */

/* Wrapper implementations from stubs.c */
extern int  __wrap_run_menu(const char *title, const char *labels[], int count);
extern char *__wrap_run_load_menu(void);
extern void __wrap_run_settings_menu(void);

/* Helpers to drive the wrappers */
extern void set_next_menu_choice(int choice);
extern void set_next_load_slot(const char *slot_name);

/* no-op sleep for the fake UI */
static void noop_sleep(int ms) { (void)ms; }

/* adapter functions matching UI signatures */
static void ui_print(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
}

static int ui_rand_int(int max) {
    return rand() % max;
}

static void ui_read_line(const char *prompt, char *buf, int len) {
    input_get_line(prompt, buf, (size_t)len);
}

static void ui_save_slot(const char *slot,
                         const MazeUI *ui_ctx,
                         int rows, int cols,
                         double elapsed_secs) {
    (void)save_game_slot(slot, ui_ctx, rows, cols, elapsed_secs);
}

static char *ui_run_load_menu(UI *unused) {
    (void)unused;
    return __wrap_run_load_menu();
}

static void ui_run_settings_menu(UI *unused) {
    (void)unused;
    __wrap_run_settings_menu();
}

/* build a UI that just forwards into the wrapped symbols */
static UI make_fake_ui(void) {
    UI ui = {0};
    ui.clear_screen       = (void(*)(void))        system;
    ui.print              = ui_print;
    ui.sleep_ms           = noop_sleep;
    ui.seed_rng           = (void(*)(unsigned))     srand;
    ui.rand_int           = ui_rand_int;
    ui.poll_input         = input_poll;             /* wrapped */
    ui.read_line          = ui_read_line;
    ui.save_slot          = ui_save_slot;
    ui.run_menu           = __wrap_run_menu;        /* wrapped */
    ui.run_load_menu      = ui_run_load_menu;
    ui.run_settings_menu  = ui_run_settings_menu;
    return ui;
}

static void test_continue_shown_when_grid_exists(void **state) {
    (void)state;
    GameContext g = { .grid = (Cell**)1 };
    set_next_menu_choice(0);        /* index 0 → CONTINUE */
    set_next_load_slot(NULL);
    UI ui = make_fake_ui();
    assert_int_equal(game_menu(&g, &ui), STATE_PLAY_LEVEL);
}

static void test_no_continue_when_no_grid(void **state) {
    (void)state;
    GameContext g = {0};
    set_next_menu_choice(0);        /* index 0 → NEW Game */
    set_next_load_slot(NULL);
    UI ui = make_fake_ui();
    assert_int_equal(game_menu(&g, &ui), STATE_SETUP_LEVEL);
}

static void test_new_game_resets_and_saves(void **state) {
    (void)state;
    GameContext g = {
        .grid    = (Cell**)1,
        .maze      = { .level = 5, .time_secs = 7.0 },
        .slot    = strdup("slotA")
    };
    set_next_menu_choice(1);        /* index 1 → NEW Game */
    set_next_load_slot(NULL);
    UI ui = make_fake_ui();

    assert_int_equal(game_menu(&g, &ui), STATE_SETUP_LEVEL);
    assert_null(g.grid);
    assert_int_equal(g.maze.level, 1);
    assert_true(g.maze.time_secs == 0.0);
    free(g.slot);
}

static void test_load_game_updates_context(void **state) {
    (void)state;
    GameContext g = {0};
    set_next_menu_choice(1);        /* index 1 → LOAD Game */
    set_next_load_slot("slot1");
    UI ui = make_fake_ui();

    assert_int_equal(game_menu(&g, &ui), STATE_SETUP_LEVEL);
    assert_string_equal(g.slot, "slot1");
    assert_int_equal(g.cfg.height, 8);
    assert_int_equal(g.cfg.width,  9);
    assert_int_equal(g.maze.level,   2);
    assert_int_equal(g.maze.bullets, 5);
    assert_true(g.maze.time_secs == 1.23);
    free(g.slot);
}

static void test_settings_clears_grid_without_exiting(void **state) {
    (void)state;
    GameContext g = {
        .grid        = (Cell**)1,
        .slot        = strdup("slotB"),
        .maze.time_secs = 4.5
    };
    set_next_menu_choice(3);        /* index 3 → Settings */
    set_next_load_slot(NULL);
    UI ui = make_fake_ui();

    assert_int_equal(game_menu(&g, &ui), STATE_MENU);
    assert_null(g.grid);
    free(g.slot);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_continue_shown_when_grid_exists),
        cmocka_unit_test(test_no_continue_when_no_grid),
        cmocka_unit_test(test_new_game_resets_and_saves),
        cmocka_unit_test(test_load_game_updates_context),
        cmocka_unit_test(test_settings_clears_grid_without_exiting),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
