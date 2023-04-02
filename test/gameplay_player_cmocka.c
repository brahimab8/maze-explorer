#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "gameplay/player.h"

// stub can_step to be injected from engine/movement.c
extern bool __wrap_can_step(Cell **grid, int rows, int cols,
                            int x, int y, Direction d);

// fake grid pointer.
static Cell *fake_pool;
static Cell **fake_grid = (Cell**)&fake_pool;

static bool can_step_allowed;
bool __wrap_can_step(Cell **grid, int rows, int cols,
                     int x, int y, Direction d) {
    (void)grid; (void)rows; (void)cols; (void)x; (void)y; (void)d;
    return can_step_allowed;
}

static void test_player_init(void **state) {
    (void)state;
    Player p;
    player_init(&p, 3, 4, 7, '#');
    assert_int_equal(p.x, 3);
    assert_int_equal(p.y, 4);
    assert_int_equal(p.dir, DIR_DOWN);
    assert_int_equal(p.bullets, 7);
    assert_int_equal(p.symbol, '#');
}

static void test_player_move_blocked(void **state) {
    (void)state;
    Player p = { .x = 1, .y = 1, .dir = DIR_DOWN, .bullets = 0, .symbol = 0 };
    can_step_allowed = false;
    player_move(&p, INP_UP, fake_grid, 5, 5);
    assert_int_equal(p.x, 1);
    assert_int_equal(p.y, 1);
    // direction should still update
    assert_int_equal(p.dir, DIR_UP);
}

static void test_player_move_allowed(void **state) {
    (void)state;
    Player p = { .x = 1, .y = 1, .dir = DIR_DOWN, .bullets = 0, .symbol = 0 };
    can_step_allowed = true;
    player_move(&p, INP_LEFT, fake_grid, 5, 5);
    assert_int_equal(p.x, 0);
    assert_int_equal(p.y, 1);
    assert_int_equal(p.dir, DIR_LEFT);
}

static void test_player_shoot(void **state) {
    (void)state;
    Player p = { .x=0,.y=0,.dir=DIR_DOWN,.bullets=2,.symbol='@' };
    assert_true(player_shoot(&p));
    assert_int_equal(p.bullets, 1);
    assert_true(player_shoot(&p));
    assert_int_equal(p.bullets, 0);
    assert_false(player_shoot(&p));
    assert_int_equal(p.bullets, 0);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_player_init),
        cmocka_unit_test(test_player_move_blocked),
        cmocka_unit_test(test_player_move_allowed),
        cmocka_unit_test(test_player_shoot),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
