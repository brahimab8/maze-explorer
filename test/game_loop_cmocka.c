#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>
#include <cmocka.h>

#include "game/game.h"
#include "ui/ui.h"

/*–– Shared stubs & wrappers from stubs.c ––*/
extern void   set_next_menu_choice(int choice);
extern void   set_next_load_slot(const char *slot);
extern int    menu_calls;
extern int    __wrap_run_menu(const char *title,
                              const char *labels[],
                              int count);
extern char*  __wrap_run_load_menu(UI *ui);
extern void   __wrap_run_settings_menu(UI *ui);
extern void   __wrap_draw_maze(Cell **grid,
                               int rows,
                               int cols,
                               const MazeUI *ui);

/*–– Fake UI callbacks ––*/
static void stub_seed_rng(unsigned s)            { (void)s; }
static void stub_read_line(const char *p, char *b, int l) {
    (void)p; (void)l; b[0] = '\0';
}
static void stub_save_slot(const char *slot,
                           const MazeUI *mz,
                           int r, int c, double e)
{ (void)slot; (void)mz; (void)r; (void)c; (void)e; }
static void stub_clear_screen(void)               { /* no-op */ }
static void stub_print(const char *fmt, ...)      { (void)fmt; /* no-op */ }
static InputAction stub_poll_input_quit(void)     { return INP_QUIT; }
static void stub_sleep_ms(int ms)                 { (void)ms; }

/*–– Tests ––*/

// 1) Quit immediately from menu (no grid)
static void test_run_game_quit_from_menu(void **state) {
    (void)state;
    menu_calls = 0;
    /* raw index 3 → “Quit” when no grid */
    set_next_menu_choice(3);

    GameSettings cfg = { .width=4, .height=4, .initial_shots=2 };
    UI ui = {0};
    ui.seed_rng          = stub_seed_rng;
    ui.read_line         = stub_read_line;
    ui.save_slot         = stub_save_slot;
    ui.clear_screen      = stub_clear_screen;
    ui.print             = stub_print;
    ui.run_menu          = __wrap_run_menu;
    ui.run_load_menu     = __wrap_run_load_menu;
    ui.run_settings_menu = __wrap_run_settings_menu;
    ui.poll_input        = stub_poll_input_quit;
    ui.sleep_ms          = stub_sleep_ms;

    run_game(&cfg, &ui);
    assert_int_equal(menu_calls, 1);
}

// 2) New Game → immediate quit in play
static void test_run_game_new_then_quit_in_play(void **state) {
    (void)state;
    menu_calls = 0;
    /* raw 0 → “New Game” (when no grid) */
    set_next_menu_choice(0);

    GameSettings cfg = { .width=2, .height=2, .initial_shots=1 };
    UI ui = {0};
    ui.seed_rng          = stub_seed_rng;
    ui.read_line         = stub_read_line;
    ui.save_slot         = stub_save_slot;
    ui.clear_screen      = stub_clear_screen;
    ui.print             = stub_print;
    ui.run_menu          = __wrap_run_menu;
    ui.run_load_menu     = __wrap_run_load_menu;
    ui.run_settings_menu = __wrap_run_settings_menu;
    ui.poll_input        = stub_poll_input_quit;
    ui.sleep_ms          = stub_sleep_ms;

    run_game(&cfg, &ui);
    assert_int_equal(menu_calls, 1);
}

// 3) Load Game → immediate quit in play (grid pre-exists after load)
static void test_run_game_load_then_quit(void **state) {
    (void)state;
    menu_calls = 0;

    /* First menu: raw index 1 → “Load Game” (when no grid) */
    set_next_menu_choice(1);
    /* Have run_load_menu() return a valid slot */
    set_next_load_slot("test_slot");

    GameSettings cfg = { .width=3, .height=3, .initial_shots=4 };
    UI ui = {0};
    ui.seed_rng          = stub_seed_rng;
    ui.read_line         = stub_read_line;
    ui.save_slot         = stub_save_slot;
    ui.clear_screen      = stub_clear_screen;
    ui.print             = stub_print;
    ui.run_menu          = __wrap_run_menu;
    ui.run_load_menu     = __wrap_run_load_menu;
    ui.run_settings_menu = __wrap_run_settings_menu;
    ui.poll_input        = stub_poll_input_quit;
    ui.sleep_ms          = stub_sleep_ms;

    run_game(&cfg, &ui);
    assert_int_equal(menu_calls, 1);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_run_game_quit_from_menu),
        cmocka_unit_test(test_run_game_new_then_quit_in_play),
        cmocka_unit_test(test_run_game_load_then_quit),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
