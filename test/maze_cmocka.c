#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>
#include <time.h>

#include "engine/maze.h"

//------- Helpers (flood‐fill and wall‐count) --------------------
static int flood_count(Cell **grid, int rows, int cols, int r, int c, char *seen) {
    if (r < 0 || r >= rows || c < 0 || c >= cols) return 0;
    int idx = r * cols + c;
    if (seen[idx]) return 0;
    seen[idx] = 1;
    int cnt = 1;
    if (!grid[r][c].wallN) cnt += flood_count(grid, rows, cols, r-1, c, seen);
    if (!grid[r][c].wallS) cnt += flood_count(grid, rows, cols, r+1, c, seen);
    if (!grid[r][c].wallW) cnt += flood_count(grid, rows, cols, r, c-1, seen);
    if (!grid[r][c].wallE) cnt += flood_count(grid, rows, cols, r, c+1, seen);
    return cnt;
}

//------- Test: after carve, all cells reachable --------------------
static void test_connectivity(void **state) {
    (void)state;
    const int rows = 10, cols = 10;
    Cell **grid;
    srand(0);
    init_maze(rows, cols, &grid);
    carve(grid, rows, cols, 0, 0);

    char *seen = calloc(rows * cols, 1);
    assert_non_null(seen);
    int cnt = flood_count(grid, rows, cols, 0, 0, seen);
    assert_int_equal(cnt, rows * cols);
    free(seen);
    free_maze(grid);
}

//------- Test: exactly (rows*cols - 1) walls removed --------------------
static void test_wall_removal_count(void **state) {
    (void)state;
    const int rows = 10, cols = 10;
    Cell **grid;
    srand(0);
    init_maze(rows, cols, &grid);
    carve(grid, rows, cols, 0, 0);

    int total_bits = 0;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            total_bits += grid[r][c].wallN
                        + grid[r][c].wallS
                        + grid[r][c].wallW
                        + grid[r][c].wallE;
        }
    }
    int initial = rows * cols * 4;
    int removed = initial - total_bits;
    assert_int_equal(removed, 2 * (rows * cols - 1));

    free_maze(grid);
}

//------- Test runner -------------------------------------------------------
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_connectivity),
        cmocka_unit_test(test_wall_removal_count),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
