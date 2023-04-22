#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "engine/maze.h"
#include "ui/maze_ui.h"

// ----------------------------------------------------------------------------
// Helper: capture stdout from draw_maze()
// ----------------------------------------------------------------------------
static char *capture_draw(Cell **grid, int rows, int cols, const MazeUI *ui) {
    // create a temporary file
    FILE *tmp = tmpfile();
    assert_non_null(tmp);
    int tmpfd = fileno(tmp);

    // back up stdout
    int stdout_fd = dup(STDOUT_FILENO);
    assert_true(stdout_fd >= 0);
    fflush(stdout);

    // redirect stdout to tmp
    dup2(tmpfd, STDOUT_FILENO);

    // call the function under test
    draw_maze(grid, rows, cols, ui);
    fflush(stdout);

    // restore stdout
    dup2(stdout_fd, STDOUT_FILENO);
    close(stdout_fd);

    // read back from tmp
    fseek(tmp, 0, SEEK_END);
    long len = ftell(tmp);
    rewind(tmp);

    char *buf = malloc(len + 1);
    assert_non_null(buf);
    fread(buf, 1, len, tmp);
    buf[len] = '\0';
    fclose(tmp);
    return buf;
}

// ----------------------------------------------------------------------------
// Test 1: simple 2×2, just player & exit & info
// ----------------------------------------------------------------------------
static void test_draw_maze_basic(void **state) {
    (void)state;
    int rows = 2, cols = 2;
    Cell **grid;
    init_maze(rows, cols, &grid);
    carve(grid, rows, cols, 0, 0);

    MazeUI ui = {0};
    strcpy(ui.player_name, "Test");
    ui.level        = 5;
    ui.bullets      = 3;
    ui.time_secs    = 75.4;
    ui.player_symbol     = '@';
    ui.exit_symbol       = 'X';
    ui.projectile_symbol = '*';
    ui.monster_symbol    = 'M';
    ui.item_symbol       = '!';
    ui.player_x = 0; ui.player_y = 0;
    ui.exit_x   = 1; ui.exit_y   = 1;
    ui.projectile_count = 0;
    ui.monster_count    = 0;
    ui.item_count       = 0;

    char *out = capture_draw(grid, rows, cols, &ui);
    assert_non_null(strstr(out, "+---+---+"));
    assert_non_null(strstr(out, "Player: Test"));
    assert_non_null(strstr(out, "Level: 5"));
    assert_non_null(strstr(out, "Bullets: 3"));
    assert_non_null(strstr(out, "Time: 01:15.4"));
    assert_non_null(strstr(out, "@"));  // player
    assert_non_null(strstr(out, "X"));  // exit
    free(out);
    free_maze(grid);
}

// ----------------------------------------------------------------------------
// Test 2: add one projectile, one monster and one item each on distinct rows
// ----------------------------------------------------------------------------
static void test_draw_maze_with_entities(void **state) {
    (void)state;
    int rows = 3, cols = 3;
    Cell **grid;
    init_maze(rows, cols, &grid);
    carve(grid, rows, cols, 0, 0);

    MazeUI ui = {0};
    // info fields
    strcpy(ui.player_name, "Ent");
    ui.level        = 1;
    ui.bullets      = 0;
    ui.time_secs    = 12.3;
    // symbols
    ui.player_symbol     = 'P';
    ui.exit_symbol       = 'E';
    ui.projectile_symbol = '*';
    ui.monster_symbol    = 'M';
    ui.item_symbol       = '!';
    // positions: player top, exit bottom
    ui.player_x = 1; ui.player_y = 0;
    ui.exit_x   = 2; ui.exit_y   = 2;
    // one projectile at (0,1)
    ui.projectile_count = 1;
    ui.projectiles[0] = (Projectile){ .x = 0, .y = 1, .active = true };
    // one monster at (2,1)
    ui.monster_count = 1;
    ui.monsters[0]   = (Monster){ .x = 2, .y = 1 };
    // one item at (1,2)
    ui.item_count = 1;
    ui.items[0]   = (Item){ .x = 1, .y = 2, .active = true };

    char *out = capture_draw(grid, rows, cols, &ui);

    // Make sure entities show up
    // projectile at row y=1, col x=0 → "*"
    assert_non_null(strstr(out, "*"));
    // monster at y=1, col x=2 → "M"
    assert_non_null(strstr(out, "M"));
    // item at y=2, col x=1 → "!"
    assert_non_null(strstr(out, "!"));
    // player and exit still present
    assert_non_null(strstr(out, "P"));
    assert_non_null(strstr(out, "E"));
    free(out);
    free_maze(grid);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_draw_maze_basic),
        cmocka_unit_test(test_draw_maze_with_entities),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}