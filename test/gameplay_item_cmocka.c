#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdlib.h>  

#include "gameplay/item.h"

// Maze size for tests
#define TEST_ROWS  10
#define TEST_COLS  15

static void test_items_init_basic(void **state) {
    (void)state;

    Item items[MAX_ITEMS];
    int  count = -1;
    // fix the seed so rand() is predictable
    srand(12345);

    // forbid spawning at or near (3,4)
    const int sx = 3, sy = 4;
    const char sym = 'X';

    items_init(items, &count, TEST_ROWS, TEST_COLS, sx, sy, sym);

    // Should place exactly MAX_ITEMS
    assert_int_equal(count, MAX_ITEMS);

    for (int i = 0; i < count; i++) {
        Item *it = &items[i];

        // Must be active and correct symbol
        assert_true(it->active);
        assert_int_equal(it->symbol, sym);

        // Within bounds
        assert_in_range(it->x, 0, TEST_COLS - 1);
        assert_in_range(it->y, 0, TEST_ROWS - 1);

        // At least one cell away from (sx,sy)
        int dx = it->x - sx;
        int dy = it->y - sy;
        if (dx < 0) dx = -dx;
        if (dy < 0) dy = -dy;
        assert_true(!(dx <= 1 && dy <= 1));

        // No duplicate positions
        for (int j = 0; j < i; j++) {
            if (items[j].x == it->x && items[j].y == it->y) {
                fail_msg("Duplicate item at (%d,%d)", it->x, it->y);
            }
        }
    }
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_items_init_basic),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
