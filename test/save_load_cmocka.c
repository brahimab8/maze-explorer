#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "util/save_game.h"
#include "ui/maze_ui.h"

//── Utility to remove either a "saves/" directory or a stray "saves" file ─────
static void cleanup_saves(void) {
    struct stat st;
    if (stat("saves", &st) < 0)
        return;  // nothing there

    if (S_ISDIR(st.st_mode)) {
        DIR *d = opendir("saves");
        if (d) {
            struct dirent *ent;
            char path[PATH_MAX];
            while ((ent = readdir(d)) != NULL) {
                if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, ".."))
                    continue;
                snprintf(path, sizeof path, "saves/%s", ent->d_name);
                unlink(path);
            }
            closedir(d);
        }
        rmdir("saves");
    } else {
        // it was a plain file named "saves"
        unlink("saves");
    }
}

//── Fixtures ────────────────────────────────────────────────────────────────
static int setup_saves_dir(void **state) {
    (void)state;
    cleanup_saves();
    if (mkdir("saves", 0755) != 0) {
        // if mkdir fails for any reason, bail out
        return -1;
    }
    return 0;
}

static int teardown_saves(void **state) {
    (void)state;
    cleanup_saves();
    return 0;
}

//── Tests ──────────────────────────────────────────────────────────────────

// 1) Round-trip save → load must succeed
static void test_save_and_load_roundtrip(void **state) {
    (void)state;
    MazeUI mz      = { .level = 42, .bullets = 7 };
    int    rows    = 8, cols = 9;
    double elapsed = 3.14;

    assert_true(save_game_slot("slot1", &mz, rows, cols, elapsed));

    MazeUI out_mz = {0};
    int    out_r = 0, out_c = 0;
    double out_e = 0;
    assert_true(load_game_slot("slot1", &out_mz, &out_r, &out_c, &out_e));

    assert_int_equal(out_mz.level,   42);
    assert_int_equal(out_mz.bullets, 7);
    assert_int_equal(out_r,          rows);
    assert_int_equal(out_c,          cols);
    assert_float_equal(out_e,        elapsed, 1e-6);
}

// 2) Trying to load a non-existent slot must return false
static void test_load_nonexistent_slot(void **state) {
    (void)state;
    MazeUI out = {0};
    int    r, c;
    double e;
    assert_false(load_game_slot("no_such", &out, &r, &c, &e));
}

// 3) If "saves" exists as a file (not a directory), save_game_slot must fail
static int setup_saves_as_file(void **state) {
    (void)state;
    cleanup_saves();
    FILE *f = fopen("saves", "w");
    if (!f) return -1;
    fclose(f);
    return 0;
}

static void test_save_mkdir_fail(void **state) {
    (void)state;
    MazeUI dummy = { .level = 1, .bullets = 1 };
    // mkdir("saves") will hit EEXIST but path isn’t a directory
    assert_false(save_game_slot("slot2", &dummy, 1, 1, 0.0));
}

//── Test runner ────────────────────────────────────────────────────────────
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(
            test_save_and_load_roundtrip,
            setup_saves_dir, teardown_saves),
        cmocka_unit_test_setup_teardown(
            test_load_nonexistent_slot,
            setup_saves_dir, teardown_saves),
        cmocka_unit_test_setup_teardown(
            test_save_mkdir_fail,
            setup_saves_as_file, teardown_saves),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
