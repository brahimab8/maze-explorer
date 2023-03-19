#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <string.h>
#include <cmocka.h>

#include "game.h"
#include "ui.h"
#include "input.h"
#include "config.h"
#include "pause_menu.h"

//--- Fake UI callbacks --------------------------------------------------
static void stub_seed_rng(unsigned s) {
    (void)s;
}

static void stub_read_line(const char *prompt, char *buf, int len) {
    (void)prompt; (void)len;
    // simulate user pressing Enter immediately (empty name)
    buf[0] = '\0';
}

static void stub_save_slot(const char *slot,
                           const MazeUI *mz,
                           int rows, int cols,
                           double elapsed) {
    // no-op, assume success
    (void)slot; (void)mz; (void)rows; (void)cols; (void)elapsed;
}

// Helpers for game_play tests
static InputAction next_input;
static int next_pause_choice;
static void stub_sleep_ms(int ms) { (void)ms; }
static InputAction stub_poll_input(void) { return next_input; }
static int stub_run_menu(const char *title, const char *labels[], int count) {
    (void)title; (void)labels; (void)count;
    return next_pause_choice;
}

//-- Test: game_init ------------------------------------------------------
static void test_game_init_defaults(void **state) {
    (void)state;

    GameSettings cfg = { 
        .width = 5, 
        .height = 7, 
        .initial_shots = 3,
        .fps           = DEFAULT_FPS
    };

    UI ui = {0};
    ui.seed_rng  = stub_seed_rng;
    ui.read_line = stub_read_line;
    ui.save_slot = stub_save_slot;

    GameContext g;
    game_init(&g, &cfg, &ui);

    // Settings copied
    assert_int_equal(g.cfg.width,         5);
    assert_int_equal(g.cfg.height,        7);
    assert_int_equal(g.cfg.initial_shots, 3);
    assert_int_equal(g.frame_delay_ms, 1000 / DEFAULT_FPS);

    // Initial UI state
    assert_int_equal(g.maze.level,   1);
    assert_int_equal(g.maze.bullets, cfg.initial_shots);
    assert_string_equal(g.maze.player_name, "Player");
    assert_true(g.maze.time_secs == 0.0);

    // No maze yet
    assert_null(g.grid);

    // Default slot = player name
    assert_non_null(g.slot);
    assert_string_equal(g.slot, "Player");
    free(g.slot);
}

//-- Test: game_play ------------------------------------------------------
static void make_fake_ui(UI *ui) {
    memset(ui, 0, sizeof *ui);
    ui->poll_input = stub_poll_input;
    ui->run_menu   = stub_run_menu;
    ui->sleep_ms   = stub_sleep_ms;
}

static void test_game_play_quit(void **state) {
    (void)state;
    GameContext g = {0};
    // minimal maze context
    g.cfg.height = g.cfg.width = 0;
    g.maze.time_secs = 0.0;

    UI ui;
    make_fake_ui(&ui);
    next_input = INP_QUIT;

    assert_int_equal(game_play(&g, &ui), STATE_EXIT);
}

static void test_game_play_pause_continue(void **state) {
    (void)state;
    GameContext g = {0};
    g.cfg.height = g.cfg.width = 0;
    g.maze.time_secs = 1.23;

    UI ui;
    make_fake_ui(&ui);
    next_input = INP_PLAY;
    next_pause_choice = PAUSE_CONTINUE;

    assert_int_equal(game_play(&g, &ui), STATE_PLAY_LEVEL);
}

static void test_game_play_pause_mainmenu(void **state) {
    (void)state;
    GameContext g = {0};
    g.cfg.height = g.cfg.width = 0;
    g.maze.time_secs = 1.23;

    UI ui;
    make_fake_ui(&ui);
    next_input = INP_PLAY;
    next_pause_choice = PAUSE_MAINMENU;

    assert_int_equal(game_play(&g, &ui), STATE_MENU);
}

static void test_game_play_pause_quit(void **state) {
    (void)state;
    GameContext g = {0};
    g.cfg.height = g.cfg.width = 0;
    g.maze.time_secs = 1.23;

    UI ui;
    make_fake_ui(&ui);
    next_input = INP_PLAY;
    next_pause_choice = PAUSE_QUIT;

    assert_int_equal(game_play(&g, &ui), STATE_EXIT);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_game_init_defaults),
        cmocka_unit_test(test_game_play_quit),
        cmocka_unit_test(test_game_play_pause_continue),
        cmocka_unit_test(test_game_play_pause_mainmenu),
        cmocka_unit_test(test_game_play_pause_quit),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
