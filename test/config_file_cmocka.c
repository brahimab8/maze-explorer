#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <unistd.h>

#include "util/config_file.h"
#include "settings/settings.h"

// temporary path for the test config file
static const char *CFG = "test.cfg";

// remove the file after each test
static int teardown_cfg(void **state) {
    (void)state;
    unlink(CFG);
    return 0;
}

// round-trip: set every setting, save, reset, load, then verify
static void test_roundtrip_all_settings(void **state) {
    (void)state;
    settings_reset();

    int w      = MAZE_MIN_SIZE + 2;
    int h      = MAZE_MIN_SIZE + 3;
    int shots  = MIN_INITIAL_SHOTS + 4;
    int fps    = MIN_FPS + 5;
    char ps    = DEFAULT_PLAYER_SYMBOL == 'A' ? 'B' : 'A';
    char es    = DEFAULT_EXIT_SYMBOL   == 'X' ? 'Y' : 'X';
    char proj  = DEFAULT_PROJECTILE_SYMBOL == '1' ? '2' : '1';
    char mon   = DEFAULT_MONSTER_SYMBOL    == 'M' ? 'N' : 'M';
    char it    = DEFAULT_ITEM_SYMBOL       == '!' ? '@' : '!';
    int  bonus = MIN_ITEM_BONUS + 6;

    assert_true(settings_set_width(w));
    assert_true(settings_set_height(h));
    assert_true(settings_set_initial_shots(shots));
    assert_true(settings_set_fps(fps));
    assert_true(settings_set_player_symbol(ps));
    assert_true(settings_set_exit_symbol(es));
    assert_true(settings_set_projectile_symbol(proj));
    assert_true(settings_set_monster_symbol(mon));
    assert_true(settings_set_item_symbol(it));
    assert_true(settings_set_item_bonus(bonus));

    assert_true(config_save(CFG));

    // clear out all settings
    settings_reset();
    // load back
    assert_true(config_load(CFG));

    GameSettings c;
    settings_get(&c);
    assert_int_equal(c.width,             w);
    assert_int_equal(c.height,            h);
    assert_int_equal(c.initial_shots,     shots);
    assert_int_equal(c.fps,               fps);
    assert_int_equal(c.player_symbol,     ps);
    assert_int_equal(c.exit_symbol,       es);
    assert_int_equal(c.projectile_symbol, proj);
    assert_int_equal(c.monster_symbol,    mon);
    assert_int_equal(c.item_symbol,       it);
    assert_int_equal(c.item_bonus,        bonus);
}

// missing file: load should return false and leave defaults in place
static void test_missing_file_keeps_defaults(void **state) {
    (void)state;
    settings_reset();

    assert_false(config_load("nonexistent.cfg"));

    GameSettings d;
    settings_get(&d);
    assert_int_equal(d.width,         MAZE_DEFAULT_WIDTH);
    assert_int_equal(d.height,        MAZE_DEFAULT_HEIGHT);
    assert_int_equal(d.initial_shots, INITIAL_SHOTS);
    assert_int_equal(d.fps,           DEFAULT_FPS);
    assert_int_equal(d.player_symbol, DEFAULT_PLAYER_SYMBOL);
    assert_int_equal(d.exit_symbol,   DEFAULT_EXIT_SYMBOL);
    assert_int_equal(d.item_bonus,    DEFAULT_ITEM_BONUS);
}

// partial and malformed: only recognized lines take effect
static void test_partial_and_invalid_lines(void **state) {
    (void)state;
    settings_reset();

    FILE *f = fopen(CFG, "w");
    assert_non_null(f);
    fprintf(f,
        "# comment line\n"
        "Width=%d\n"
        "BadKey=123\n"
        "Height=%d\n"
        "InitialShots=%d\n"
        "FPS=%d\n"
        "PlayerSymbol=Z\n"
        "ExitSymbol=Q\n"
        "ProjectileSymbol=Y\n"
        "MonsterSymbol=X\n"
        "ItemSymbol=P\n"
        "ItemBonus=%d\n"
        "MalformedLineWithoutEquals\n",
        MAZE_MIN_SIZE + 7,
        MAZE_MIN_SIZE + 8,
        MIN_INITIAL_SHOTS + 9,
        MIN_FPS + 10,
        MIN_ITEM_BONUS + 11
    );
    fclose(f);

    // loader returns true even if some lines are unrecognized
    assert_true(config_load(CFG));

    GameSettings c;
    settings_get(&c);
    assert_int_equal(c.width,             MAZE_MIN_SIZE + 7);
    assert_int_equal(c.height,            MAZE_MIN_SIZE + 8);
    assert_int_equal(c.initial_shots,     MIN_INITIAL_SHOTS + 9);
    assert_int_equal(c.fps,               MIN_FPS + 10);
    assert_int_equal(c.player_symbol,     'Z');
    assert_int_equal(c.exit_symbol,       'Q');
    assert_int_equal(c.projectile_symbol, 'Y');
    assert_int_equal(c.monster_symbol,    'X');
    assert_int_equal(c.item_symbol,       'P');
    assert_int_equal(c.item_bonus,        MIN_ITEM_BONUS + 11);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_teardown(test_roundtrip_all_settings,    teardown_cfg),
        cmocka_unit_test_teardown(test_missing_file_keeps_defaults, teardown_cfg),
        cmocka_unit_test_teardown(test_partial_and_invalid_lines,  teardown_cfg),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
