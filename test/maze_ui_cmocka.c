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
// Test
// ----------------------------------------------------------------------------
static void test_draw_maze_output(void **state) {
    (void)state;

    // build a 2×2 maze and carve it
    int rows = 2, cols = 2;
    Cell **grid;
    init_maze(rows, cols, &grid);
    carve(grid, rows, cols, 0, 0);

    // prepare UI
    MazeUI ui;
    strcpy(ui.player_name, "Test");
    ui.level     = 5;
    ui.bullets   = 3;
    ui.time_secs = 75.4;   // 1m15.4s

    // capture and verify
    char *out = capture_draw(grid, rows, cols, &ui);

    assert_non_null(strstr(out, "+---+---+"));        // top border
    assert_non_null(strstr(out, "Player: Test"));     // name
    assert_non_null(strstr(out, "Level: 5"));         // level
    assert_non_null(strstr(out, "Bullets: 3"));       // bullets
    assert_non_null(strstr(out, "Time: 01:15.4"));    // formatted time

    free(out);
    free_maze(grid);
}

// ----------------------------------------------------------------------------
// Test runner
// ----------------------------------------------------------------------------
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_draw_maze_output),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
