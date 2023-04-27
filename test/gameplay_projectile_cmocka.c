// test/gameplay_projectile_cmocka.c

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "ui/ui.h"
#include "gameplay/projectile.h"

// stub out can_step from engine/movement.c
extern bool __wrap_can_step(Cell **grid, int rows, int cols,
                            int x, int y, Direction d);
static bool can_step_allowed;
bool __wrap_can_step(Cell **grid, int rows, int cols,
                     int x, int y, Direction d) {
    (void)grid; (void)rows; (void)cols;
    (void)x; (void)y; (void)d;
    return can_step_allowed;
}

// Helpers to track wall-hit callback
static int    wall_hit_called;
static void  *wall_hit_user;
static Cell  *wall_hit_grid;
static int    wall_hit_rows, wall_hit_cols;
static int    wall_hit_x, wall_hit_y;
static Direction wall_hit_dir;
static void stub_on_hit_wall(void *ud, Cell **g,
                             int rows, int cols,
                             int x, int y,
                             Direction d)
{
    wall_hit_called  = 1;
    wall_hit_user    = ud;
    wall_hit_grid    = (Cell*)g;
    wall_hit_rows    = rows;
    wall_hit_cols    = cols;
    wall_hit_x       = x;
    wall_hit_y       = y;
    wall_hit_dir     = d;
}

// Stub monster-hit callback
static int monster_hit_x, monster_hit_y;
static bool stub_on_hit_monster(void *ud, int x, int y) {
    (void)ud;
    monster_hit_x = x;
    monster_hit_y = y;
    // return true to remove projectile
    return true;
}

// Dummy grid pointer
static Cell *dummy_pool;
static Cell **dummy_grid = (Cell**)&dummy_pool;

// Correct setup signature for CMocka
static int setUp(void **state) {
    (void)state;
    can_step_allowed = true;
    wall_hit_called  = 0;
    monster_hit_x    = monster_hit_y = -1;
    return 0;
}

// ─── projectile_fire ───────────────────────────────────────────────────────
static void test_projectile_fire_and_limit(void **state) {
    (void)state;
    Projectile list[MAX_PROJECTILES + 2];
    int count = 0;

    // Fire up to MAX_PROJECTILES
    for (int i = 0; i < MAX_PROJECTILES; i++) {
        projectile_fire(list, &count, i, i+1, DIR_LEFT);
        assert_int_equal(count, i+1);
        assert_true(list[i].active);
        assert_int_equal(list[i].x, i);
        assert_int_equal(list[i].y, i+1);
        assert_int_equal(list[i].dir, DIR_LEFT);
    }
    // One more should be ignored
    projectile_fire(list, &count, 999, 999, DIR_UP);
    assert_int_equal(count, MAX_PROJECTILES);
}

// ─── projectile_update: move in-bounds ─────────────────────────────────────
static void test_projectile_moves(void **state) {
    (void)state;
    Projectile list[1];
    int count = 1;
    list[0] = (Projectile){ .x = 1, .y = 1, .dir = DIR_RIGHT, .active = true };

    projectile_update(list, &count, dummy_grid, 5, 5,
                      NULL, NULL, NULL);

    // now moved to (2,1), still active
    assert_int_equal(count, 1);
    assert_int_equal(list[0].x, 2);
    assert_int_equal(list[0].y, 1);
}

// ─── projectile_update: out-of-bounds ──────────────────────────────────────
static void test_projectile_out_of_bounds(void **state) {
    (void)state;
    Projectile list[1];
    int count = 1;
    // at edge, will step off-grid
    list[0] = (Projectile){ .x = 4, .y = 0, .dir = DIR_RIGHT, .active = true };

    projectile_update(list, &count, dummy_grid, 5, 5,
                      NULL, NULL, NULL);

    // removed
    assert_int_equal(count, 0);
}

// ─── projectile_update: hit wall ──────────────────────────────────────────
static void test_projectile_hits_wall(void **state) {
    (void)state;
    Projectile list[1];
    int count = 1;
    list[0] = (Projectile){ .x = 2, .y = 2, .dir = DIR_DOWN, .active = true };

    can_step_allowed = false;  // simulate wall in front
    projectile_update(list, &count, dummy_grid, 5, 5,
                      stub_on_hit_wall,
                      NULL,
                      (void*)0xdeadbeef);

    // projectile removed
    assert_int_equal(count, 0);
    // wall callback invoked
    assert_true(wall_hit_called);
    assert_ptr_equal(wall_hit_user, (void*)0xdeadbeef);
    assert_int_equal(wall_hit_x, 2);
    assert_int_equal(wall_hit_y, 2);
    assert_int_equal(wall_hit_dir, DIR_DOWN);
}

// ─── projectile_update: hit monster ────────────────────────────────────────
static void test_projectile_hits_monster(void **state) {
    (void)state;
    Projectile list[1];
    int count = 1;
    list[0] = (Projectile){ .x = 0, .y = 1, .dir = DIR_UP, .active = true };

    // allow stepping
    can_step_allowed = true;
    projectile_update(list, &count, dummy_grid, 5, 5,
                      NULL,
                      stub_on_hit_monster,
                      (void*)0xfeedface);

    // monster callback should remove
    assert_int_equal(count, 0);
    // moved from (0,1) up into (0,0)
    assert_int_equal(monster_hit_x, 0);
    assert_int_equal(monster_hit_y, 0);
}

// ─── projectile_update: stays when nothing happens ─────────────────────────
static void test_projectile_persists_when_clear(void **state) {
    (void)state;
    Projectile list[1];
    int count = 1;
    list[0] = (Projectile){ .x = 1, .y = 3, .dir = DIR_LEFT, .active = true };

    can_step_allowed = true;
    projectile_update(list, &count, dummy_grid, 5, 5,
                      NULL, NULL, NULL);

    // moved to (0,3) and still active
    assert_int_equal(count, 1);
    assert_int_equal(list[0].x, 0);
    assert_int_equal(list[0].y, 3);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_projectile_fire_and_limit),
        cmocka_unit_test_setup(test_projectile_moves,   setUp),
        cmocka_unit_test_setup(test_projectile_out_of_bounds, setUp),
        cmocka_unit_test_setup(test_projectile_hits_wall,    setUp),
        cmocka_unit_test_setup(test_projectile_hits_monster, setUp),
        cmocka_unit_test_setup(test_projectile_persists_when_clear, setUp),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
