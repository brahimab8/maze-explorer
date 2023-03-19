#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "settings.h"

// --- Test cases ---------------------------------------------------------
static void test_defaults(void **state) {
    (void)state;
    GameSettings cfg;
    settings_get(&cfg);

    assert_int_equal(cfg.width,           MAZE_DEFAULT_WIDTH);
    assert_int_equal(cfg.height,          MAZE_DEFAULT_HEIGHT);
    assert_int_equal(cfg.initial_shots,   INITIAL_SHOTS);
}

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

static void test_valid_initial_shots(void **state) {
    (void)state;
    GameSettings cfg;

    assert_true(settings_set_initial_shots(0));
    settings_get(&cfg);
    assert_int_equal(cfg.initial_shots, 0);

    assert_true(settings_set_initial_shots(42));
    settings_get(&cfg);
    assert_int_equal(cfg.initial_shots, 42);
}

static void test_invalid_initial_shots(void **state) {
    (void)state;
    GameSettings cfg;
    settings_get(&cfg);
    int original = cfg.initial_shots;

    assert_false(settings_set_initial_shots(-1));
    settings_get(&cfg);
    assert_int_equal(cfg.initial_shots, original);
}

// --- Test runner ---------------------------------------------------------
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_defaults),
        cmocka_unit_test(test_valid_width),
        cmocka_unit_test(test_invalid_width),
        cmocka_unit_test(test_valid_height),
        cmocka_unit_test(test_invalid_height),
        cmocka_unit_test(test_valid_initial_shots),
        cmocka_unit_test(test_invalid_initial_shots),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
