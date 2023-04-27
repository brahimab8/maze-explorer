#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "gameplay/monster.h"
#include "engine/maze.h" 

#define ROWS 10
#define COLS 12

/*---- monster_count_for_level ----*/
static void test_monster_count_levels(void **state) {
    (void)state;
    for (int lvl = 1; lvl <= 5; ++lvl) {
        assert_int_equal(monster_count_for_level(lvl), 1);
    }
    for (int lvl = 6; lvl <= 10; ++lvl) {
        assert_int_equal(monster_count_for_level(lvl), 2);
    }
    int cap = monster_count_for_level(1000);
    assert_true(cap <= MAX_MONSTERS);
}

/*---- monster_base_cooldown_for_level ----*/
static void test_base_cooldown_cycle(void **state) {
    (void)state;
    int expected[] = {5,4,3,2,1,5,4,3,2,1};
    for (int lvl = 1; lvl <= 10; ++lvl) {
        assert_int_equal(
            monster_base_cooldown_for_level(lvl),
            expected[lvl - 1]
        );
    }
}

/*---- monsters_init ----*/
static void test_monsters_init_positions_and_cooldowns(void **state) {
    (void)state;
    srand(12345);

    Monster mons[MAX_MONSTERS];
    int count = 0;
    int lvl = 7; // 1 + (7-1)/5 = 2 monsters
    monsters_init(mons, &count, lvl, ROWS, COLS);
    assert_int_equal(count, monster_count_for_level(lvl));

    int base_cd = monster_base_cooldown_for_level(lvl) * 5;
    for (int i = 0; i < count; ++i) {
        assert_int_equal(mons[i].base_cooldown, base_cd);
        assert_int_equal(mons[i].cooldown,      base_cd);
        // at least 3 cells away in either axis
        assert_true(abs(mons[i].x) >= 3 || abs(mons[i].y) >= 3);
        assert_in_range(mons[i].x, 0, COLS - 1);
        assert_in_range(mons[i].y, 0, ROWS - 1);
    }
}

/*---- monsters_step ----*/
static void test_monsters_step_moves_toward_player(void **state) {
    (void)state;
    // build a ROWS×COLS grid of zeroed Cells (empty)
    static Cell row_data[ROWS][COLS];
    memset(row_data, 0, sizeof row_data);
    Cell *grid[ROWS];
    for (int r = 0; r < ROWS; ++r) {
        grid[r] = row_data[r];
    }

    // one monster at (2,2), ready to move
    Monster mons[1] = {{ .x = 2,
                         .y = 2,
                         .base_cooldown = 1,
                         .cooldown = 0 }};
    int px = 5, py = 2;  // player position

    monsters_step(mons, 1, px, py, grid, ROWS, COLS);

    // monster should move one step right toward (5,2)
    assert_int_equal(mons[0].x, 3);
    assert_int_equal(mons[0].y, 2);
    // cooldown reset
    assert_int_equal(mons[0].cooldown, mons[0].base_cooldown);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_monster_count_levels),
        cmocka_unit_test(test_base_cooldown_cycle),
        cmocka_unit_test(test_monsters_init_positions_and_cooldowns),
        cmocka_unit_test(test_monsters_step_moves_toward_player),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
