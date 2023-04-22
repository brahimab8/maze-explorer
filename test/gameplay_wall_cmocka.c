#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h> 

#include "engine/maze.h"      // Cell
#include "gameplay/wall.h"    // wall_destroy_cb()
#include "engine/movement.h"  // Direction

// Helper: make a 3×3 grid with all walls present
static Cell **make_grid3(void) {
    int rows = 3, cols = 3;
    Cell *pool = calloc(rows * cols, sizeof *pool);
    Cell **grid = malloc(rows * sizeof *grid);
    for (int r = 0; r < rows; ++r) {
        grid[r] = pool + r*cols;
        for (int c = 0; c < cols; ++c) {
            grid[r][c].visited = 0;
            grid[r][c].wallN = grid[r][c].wallS =
            grid[r][c].wallE = grid[r][c].wallW = 1;
        }
    }
    return grid;
}

static void test_wall_destroy_up(void **state) {
    (void)state;
    Cell **g = make_grid3();
    // destroy the wall north of cell (1,1) using the callback
    wall_destroy_cb(NULL, g, 3, 3, 1, 1, DIR_UP);
    assert_false(g[1][1].wallN);
    assert_false(g[0][1].wallS);
    free(g[0]);
    free(g);
}

static void test_wall_destroy_down(void **state) {
    (void)state;
    Cell **g = make_grid3();
    wall_destroy_cb(NULL, g, 3, 3, 1, 1, DIR_DOWN);
    assert_false(g[1][1].wallS);
    assert_false(g[2][1].wallN);
    free(g[0]);
    free(g);
}

static void test_wall_destroy_left(void **state) {
    (void)state;
    Cell **g = make_grid3();
    wall_destroy_cb(NULL, g, 3, 3, 1, 1, DIR_LEFT);
    assert_false(g[1][1].wallW);
    assert_false(g[1][0].wallE);
    free(g[0]);
    free(g);
}

static void test_wall_destroy_right(void **state) {
    (void)state;
    Cell **g = make_grid3();
    wall_destroy_cb(NULL, g, 3, 3, 1, 1, DIR_RIGHT);
    assert_false(g[1][1].wallE);
    assert_false(g[1][2].wallW);
    free(g[0]);
    free(g);
}

static void test_wall_destroy_oob(void **state) {
    (void)state;
    Cell **g = make_grid3();
    // destroying outside should be no-op and not crash
    wall_destroy_cb(NULL, g, 3, 3, -1, 1, DIR_LEFT);
    wall_destroy_cb(NULL, g, 3, 3, 1, 3, DIR_DOWN);
    // all walls remain intact
    for (int r = 0; r < 3; ++r)
      for (int c = 0; c < 3; ++c)
        assert_true(g[r][c].wallN && g[r][c].wallS &&
                    g[r][c].wallE && g[r][c].wallW);
    free(g[0]);
    free(g);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_wall_destroy_up),
        cmocka_unit_test(test_wall_destroy_down),
        cmocka_unit_test(test_wall_destroy_left),
        cmocka_unit_test(test_wall_destroy_right),
        cmocka_unit_test(test_wall_destroy_oob),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
