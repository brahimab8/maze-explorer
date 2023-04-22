#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "gameplay/collision.h"   // kill_monster_at, on_hit_monster_cb
#include "game/game.h"            // GameContext, Monster, MazeUI

/* Helper: fill ctx.maze.monsters[] from a small array */
static GameContext make_ctx_with_monsters(const Monster src[], int count) {
    GameContext ctx = {0};
    ctx.maze.monster_count = count;
    for (int i = 0; i < count; i++) {
        ctx.maze.monsters[i] = src[i];
    }
    return ctx;
}

static void test_kill_existing_monster(void **state) {
    (void)state;
    Monster src[3] = {
        { .x = 1, .y = 1 },
        { .x = 2, .y = 2 },
        { .x = 3, .y = 3 },
    };
    GameContext ctx = make_ctx_with_monsters(src, 3);

    assert_true(kill_monster_at(&ctx, 2, 2));
    assert_int_equal(ctx.maze.monster_count, 2);

    bool saw11 = false, saw22 = false, saw33 = false;
    for (int i = 0; i < ctx.maze.monster_count; i++) {
        if (ctx.maze.monsters[i].x == 1 && ctx.maze.monsters[i].y == 1) saw11 = true;
        if (ctx.maze.monsters[i].x == 2 && ctx.maze.monsters[i].y == 2) saw22 = true;
        if (ctx.maze.monsters[i].x == 3 && ctx.maze.monsters[i].y == 3) saw33 = true;
    }
    assert_true(saw11);
    assert_false(saw22);
    assert_true(saw33);
}

static void test_kill_nonexistent_monster(void **state) {
    (void)state;
    Monster src[2] = {
        { .x = 5, .y = 5 },
        { .x = 6, .y = 6 },
    };
    GameContext ctx = make_ctx_with_monsters(src, 2);

    assert_false(kill_monster_at(&ctx, 0, 0));
    assert_int_equal(ctx.maze.monster_count, 2);
}

static void test_on_hit_monster_cb(void **state) {
    (void)state;
    Monster src[1] = { { .x = 8, .y = 8 } };
    GameContext ctx = make_ctx_with_monsters(src, 1);

    assert_true(on_hit_monster_cb(&ctx, 8, 8));
    assert_int_equal(ctx.maze.monster_count, 0);

    /* Calling again should do nothing */
    assert_false(on_hit_monster_cb(&ctx, 8, 8));
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_kill_existing_monster),
        cmocka_unit_test(test_kill_nonexistent_monster),
        cmocka_unit_test(test_on_hit_monster_cb),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
