#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>

#include "game/game.h"
#include "gameplay/play.h"
#include "gameplay/item.h"

#include "ui/ui.h"

// Fake UI & stubs from your game_loop tests:
extern InputAction next_input;
extern void stub_sleep_ms(int ms);
extern InputAction stub_poll_input(void);
extern void stub_clear_screen(void);
extern int stub_run_menu(const char *, const char *[], int);

static void make_fake_ui(UI *ui) {
    memset(ui, 0, sizeof *ui);
    ui->clear_screen = stub_clear_screen;
    ui->poll_input   = stub_poll_input;
    ui->run_menu     = stub_run_menu;
    ui->sleep_ms     = stub_sleep_ms;
}

// --- Test: moving onto the exit ----------------------------------------

static void test_move_into_exit(void **state) {
    (void)state;
    GameContext g = {0};

    // Configure a 2×2 maze, exit at (1,1), player at (1,0):
    g.cfg.height = g.cfg.width = 2;
    g.maze.exit_x = 1;
    g.maze.exit_y = 1;
    g.player.x   = 1;
    g.player.y   = 0;

    // Build a minimal 2×2 open grid:
    static Cell row0[2] = {{0}, {0}};
    static Cell row1[2] = {{0}, {0}};
    Cell *grid2[2]     = { row0, row1 };
    g.grid = grid2;

    UI ui;
    make_fake_ui(&ui);

    next_input = INP_DOWN;  // step onto exit
    int result = play(&g, &ui);
    assert_int_equal(result, STATE_TRANSITION);
}

// ---- Test: picking up an item ------------------------------------------------

static void test_item_pickup(void **state) {
    (void)state;
    GameContext g = {0};

    // Configure a 1×1 maze, item at (0,0), player starts there
    g.cfg.height    = g.cfg.width = 1;
    g.cfg.item_bonus = 5;

    static Item items[1];
    items[0].x      = 0;
    items[0].y      = 0;
    items[0].active = true;
    g.maze.items[0] = items[0];
    g.maze.item_count = 1;

    g.player.x       = 0;
    g.player.y       = 0;
    g.player.bullets = 1;

    // Build a minimal 1×1 open grid:
    static Cell only_row[1] = {{0}};
    Cell *grid1[1]         = { only_row };
    g.grid = grid1;

    UI ui;
    make_fake_ui(&ui);

    next_input = INP_NONE;   // no movement/shoot/pause
    int result = play(&g, &ui);

    // Should remain in PLAY_LEVEL, deactivate the item, and add bullets
    assert_int_equal(result, STATE_PLAY_LEVEL);
    assert_false(g.maze.items[0].active);
    assert_int_equal(g.player.bullets, 1 + 5);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_move_into_exit),
        cmocka_unit_test(test_item_pickup),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
