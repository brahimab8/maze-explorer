#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "settings/settings.h"
#include "config.h"

// Reset all settings to defaults before each test
static int setup(void **state) {
    (void)state;
    settings_reset();
    return 0;
}

// ---- Width ----
static void test_valid_width(void **state) {
    (void)state;
    GameSettings cfg;

    assert_true(settings_set_width(MAZE_MIN_SIZE));
    settings_get(&cfg);
    assert_int_equal(cfg.width, MAZE_MIN_SIZE);

    assert_true(settings_set_width(MAZE_MAX_SIZE));
    settings_get(&cfg);
    assert_int_equal(cfg.width, MAZE_MAX_SIZE);
}

static void test_invalid_width(void **state) {
    (void)state;
    GameSettings cfg;
    settings_get(&cfg);
    int original = cfg.width;

    assert_false(settings_set_width(MAZE_MIN_SIZE - 1));
    settings_get(&cfg);
    assert_int_equal(cfg.width, original);

    assert_false(settings_set_width(MAZE_MAX_SIZE + 1));
    settings_get(&cfg);
    assert_int_equal(cfg.width, original);
}

// ---- Height ----
static void test_valid_height(void **state) {
    (void)state;
    GameSettings cfg;

    assert_true(settings_set_height(MAZE_MIN_SIZE));
    settings_get(&cfg);
    assert_int_equal(cfg.height, MAZE_MIN_SIZE);

    assert_true(settings_set_height(MAZE_MAX_SIZE));
    settings_get(&cfg);
    assert_int_equal(cfg.height, MAZE_MAX_SIZE);
}

static void test_invalid_height(void **state) {
    (void)state;
    GameSettings cfg;
    settings_get(&cfg);
    int original = cfg.height;

    assert_false(settings_set_height(MAZE_MIN_SIZE - 1));
    settings_get(&cfg);
    assert_int_equal(cfg.height, original);

    assert_false(settings_set_height(MAZE_MAX_SIZE + 1));
    settings_get(&cfg);
    assert_int_equal(cfg.height, original);
}

// ---- Initial Shots ----
static void test_valid_initial_shots(void **state) {
    (void)state;
    GameSettings cfg;

    assert_true(settings_set_initial_shots(MIN_INITIAL_SHOTS));
    settings_get(&cfg);
    assert_int_equal(cfg.initial_shots, MIN_INITIAL_SHOTS);

    assert_true(settings_set_initial_shots(MAX_INITIAL_SHOTS));
    settings_get(&cfg);
    assert_int_equal(cfg.initial_shots, MAX_INITIAL_SHOTS);
}

static void test_invalid_initial_shots(void **state) {
    (void)state;
    GameSettings cfg;
    settings_get(&cfg);
    int original = cfg.initial_shots;

    assert_false(settings_set_initial_shots(MIN_INITIAL_SHOTS - 1));
    settings_get(&cfg);
    assert_int_equal(cfg.initial_shots, original);

    assert_false(settings_set_initial_shots(MAX_INITIAL_SHOTS + 1));
    settings_get(&cfg);
    assert_int_equal(cfg.initial_shots, original);
}

// ---- FPS ----
static void test_valid_fps(void **state) {
    (void)state;
    GameSettings cfg;

    assert_true(settings_set_fps(MIN_FPS));
    settings_get(&cfg);
    assert_int_equal(cfg.fps, MIN_FPS);

    assert_true(settings_set_fps(MAX_FPS));
    settings_get(&cfg);
    assert_int_equal(cfg.fps, MAX_FPS);
}

static void test_invalid_fps(void **state) {
    (void)state;
    GameSettings cfg;
    settings_get(&cfg);
    int original = cfg.fps;

    assert_false(settings_set_fps(MIN_FPS - 1));
    settings_get(&cfg);
    assert_int_equal(cfg.fps, original);

    assert_false(settings_set_fps(MAX_FPS + 1));
    settings_get(&cfg);
    assert_int_equal(cfg.fps, original);
}

// ---- Symbol Setters ----
static void test_valid_symbol_setters(void **state) {
    (void)state;

    assert_true(settings_set_player_symbol('A'));
    assert_true(settings_set_exit_symbol   ('B'));
    assert_true(settings_set_projectile_symbol('C'));
    assert_true(settings_set_monster_symbol  ('D'));
    assert_true(settings_set_item_symbol     ('E'));

    assert_int_equal(settings_get_player_symbol(),    'A');
    assert_int_equal(settings_get_exit_symbol(),      'B');
    assert_int_equal(settings_get_projectile_symbol(),'C');
    assert_int_equal(settings_get_monster_symbol(),   'D');
    assert_int_equal(settings_get_item_symbol(),      'E');
}

static void test_invalid_symbol_setters(void **state) {
    (void)state;
    GameSettings cfg;
    settings_get(&cfg);

    // below printable ASCII
    assert_false(settings_set_player_symbol(31));
    // above printable ASCII
    assert_false(settings_set_player_symbol(127));

    settings_get(&cfg);
    assert_int_equal(cfg.player_symbol, DEFAULT_PLAYER_SYMBOL);
}

// ---- Item Bonus ----
static void test_valid_item_bonus(void **state) {
    (void)state;
    GameSettings cfg;

    assert_true(settings_set_item_bonus(MIN_ITEM_BONUS));
    settings_get(&cfg);
    assert_int_equal(cfg.item_bonus, MIN_ITEM_BONUS);

    assert_true(settings_set_item_bonus(MAX_ITEM_BONUS));
    settings_get(&cfg);
    assert_int_equal(cfg.item_bonus, MAX_ITEM_BONUS);
}

static void test_invalid_item_bonus(void **state) {
    (void)state;
    GameSettings cfg;
    settings_get(&cfg);
    int original = cfg.item_bonus;

    assert_false(settings_set_item_bonus(MIN_ITEM_BONUS - 1));
    settings_get(&cfg);
    assert_int_equal(cfg.item_bonus, original);

    assert_false(settings_set_item_bonus(MAX_ITEM_BONUS + 1));
    settings_get(&cfg);
    assert_int_equal(cfg.item_bonus, original);
}

// --- Run all tests -------------------------------------------------------
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(test_valid_width,             setup),
        cmocka_unit_test_setup(test_invalid_width,           setup),
        cmocka_unit_test_setup(test_valid_height,            setup),
        cmocka_unit_test_setup(test_invalid_height,          setup),
        cmocka_unit_test_setup(test_valid_initial_shots,     setup),
        cmocka_unit_test_setup(test_invalid_initial_shots,   setup),
        cmocka_unit_test_setup(test_valid_fps,               setup),
        cmocka_unit_test_setup(test_invalid_fps,             setup),
        cmocka_unit_test_setup(test_valid_symbol_setters,    setup),
        cmocka_unit_test_setup(test_invalid_symbol_setters,  setup),
        cmocka_unit_test_setup(test_valid_item_bonus,        setup),
        cmocka_unit_test_setup(test_invalid_item_bonus,      setup),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
