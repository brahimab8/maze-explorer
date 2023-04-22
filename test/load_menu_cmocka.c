#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "menu/load_menu.h"
#include "ui/ui.h"

//— stubs for ui->run_menu
static int stub_choice;
static int stub_run_menu(const char *title, const char *labels[], int count) {
    (void)labels;
    (void)count;
    
    assert_string_equal(title, "Load Game");
    // You can also spot-check labels[0], labels[1], etc. here if you like.
    return stub_choice;
}

//— create a “saves/” directory with two .sav files and one other file
static int setup_saves_dir(void **state) {
    (void)state;
    system("rm -rf saves");

    if (mkdir("saves", 0755) != 0 && errno != EEXIST)
        return -1;
    FILE *f = fopen("saves/first.sav", "w");   if (!f) return -1; fclose(f);
    f = fopen("saves/second.sav", "w");         if (!f) return -1; fclose(f);
    f = fopen("saves/ignore.txt", "w");         if (!f) return -1; fclose(f);
    return 0;
}

//— remove all of that
static int teardown_saves_dir(void **state) {
    (void)state;
    unlink("saves/first.sav");
    unlink("saves/second.sav");
    unlink("saves/ignore.txt");
    rmdir("saves");
    return 0;
}

static void test_load_menu_ui_picks_second(void **state) {
    (void)state;
    UI ui = {0};
    ui.run_menu = stub_run_menu;
    stub_choice = 1;  // simulate picking the second slot

    char *picked = run_load_menu_ui(&ui);
    assert_non_null(picked);
    assert_string_equal(picked, "second");
    free(picked);
}

static void test_load_menu_ui_empty_dir(void **state) {
    (void)state;
    // first tear down (removes and recreates empty)
    teardown_saves_dir(NULL);
    mkdir("saves", 0755);

    UI ui = {0};
    ui.run_menu = stub_run_menu;
    char *picked = run_load_menu_ui(&ui);
    assert_null(picked);

    // clean up
    rmdir("saves");
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(
            test_load_menu_ui_picks_second,
            setup_saves_dir,
            teardown_saves_dir
        ),
        cmocka_unit_test_setup_teardown(
            test_load_menu_ui_empty_dir,
            setup_saves_dir,
            teardown_saves_dir
        ),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
