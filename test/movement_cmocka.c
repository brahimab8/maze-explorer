#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "engine/maze.h"       // for Cell
#include "engine/movement.h"   // for can_step(), Direction

// helper to build a 1×2 grid: two cells side by side, no walls between them
static Cell **make_two_cells(void) {
    // allocate a 1×2 grid
    Cell *pool = calloc(1*2, sizeof *pool);
    Cell **grid = malloc(1 * sizeof *grid);
    grid[0] = pool;

    // initialize both cells: all walls present
    for (int i = 0; i < 2; i++) {
        pool[i].visited = 0;
        pool[i].wallN = pool[i].wallS = pool[i].wallE = pool[i].wallW = 1;
    }
    // knock down the wall between them
    pool[0].wallE = 0;
    pool[1].wallW = 0;

    return grid;
}

static void test_can_step_inside_no_wall(void **state) {
    (void)state;
    Cell **grid = make_two_cells();
    // from x=0,y=0 we should be able to step right into cell 1
    assert_true(can_step(grid, /*rows=*/1, /*cols=*/2, /*x=*/0, /*y=*/0, DIR_RIGHT));
    // but not left (out of bounds)
    assert_false(can_step(grid, 1, 2, 0, 0, DIR_LEFT));
    free(grid[0]);
    free(grid);
}

static void test_can_step_blocked_by_wall(void **state) {
    (void)state;
    Cell **grid = make_two_cells();
    // add back the connecting wall
    grid[0][0].wallE = 1;
    grid[0][1].wallW = 1;
    assert_false(can_step(grid, 1, 2, 0, 0, DIR_RIGHT));
    free(grid[0]);
    free(grid);
}

static void test_can_step_bounds(void **state) {
    (void)state;
    Cell **grid = make_two_cells();
    // stepping up/down is always out‐of‐bounds on a single‐row maze
    assert_false(can_step(grid, 1, 2, 0, 0, DIR_UP));
    assert_false(can_step(grid, 1, 2, 0, 0, DIR_DOWN));
    free(grid[0]);
    free(grid);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_can_step_inside_no_wall),
        cmocka_unit_test(test_can_step_blocked_by_wall),
        cmocka_unit_test(test_can_step_bounds),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
