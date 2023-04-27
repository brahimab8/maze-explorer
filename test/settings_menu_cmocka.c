#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>  
#include <string.h>
#include <cmocka.h>

#include "menu/settings_menu.h"   // run_settings_menu_ui()
#include "settings/settings.h"    // settings_reset(), getters, DEFAULT_PLAYER_SYMBOL
#include "ui/ui.h"                // UI

// -----------------------------------------------------------------------------
// Globals to tell our stub what to do next
// -----------------------------------------------------------------------------

// Which menu item shall we pick on the next run_menu() call?
// For symbol test: first call PICK_PLAYER_SYMBOL, second call PICK_BACK
// For numeric test: first call PICK_FPS, second call PICK_BACK
static int g_next_run_menu_choice[2];
static int g_run_menu_call;

// Our read_line stub should supply either "Z" or "9999"
static const char *g_next_input_line;

// -----------------------------------------------------------------------------
// UI stubs
// -----------------------------------------------------------------------------

static int stub_run_menu(const char *title,
                         const char *labels[],
                         int count)
{
    (void)title; (void)labels; (void)count;
    // Return the pre-configured choice
    return g_next_run_menu_choice[g_run_menu_call++];
}

static void stub_read_line(const char *prompt, char *buf, int len) {
    (void)prompt;
    // Copy our pre-configured line (e.g. "Z" or "9999")
    strncpy(buf, g_next_input_line, len);
    buf[len-1] = '\0';
}

static void stub_sleep_ms(int ms)     { (void)ms; }
static void stub_print(const char *f, ...) { (void)f; }

// Build a UI that uses our stubs
static UI make_fake_ui(void) {
    UI ui = {0};
    ui.clear_screen       = (void(*)(void))system;  // now declared in stdlib.h
    ui.print              = stub_print;
    ui.read_line          = stub_read_line;
    ui.sleep_ms           = stub_sleep_ms;
    ui.run_menu           = stub_run_menu;
    return ui;
}

// -----------------------------------------------------------------------------
// Tests
// -----------------------------------------------------------------------------

static void test_change_player_symbol(void **state) {
    (void)state;

    // 1) Reset settings
    settings_reset();
    // 2) Prepare our menu choices:
    //    index 4 == Player Symbol, index count== “Back”
    int count = (int)settings_descriptor_count() + 1;
    g_run_menu_call = 0;
    g_next_run_menu_choice[0] = 4;      // pick “Player Symbol”
    g_next_run_menu_choice[1] = count-1; // then pick “Back”

    // 3) Next read_line returns "Z"
    g_next_input_line = "Z";

    // 4) Run it
    UI ui = make_fake_ui();
    run_settings_menu_ui(&ui);

    // 5) Verify
    assert_int_equal(settings_get_player_symbol(), 'Z');
}

static void test_invalid_numeric_setting(void **state) {
    (void)state;

    // 1) Reset settings
    settings_reset();
    // 2) Prepare menu:
    //    index 3 == FPS, then Back
    int count = (int)settings_descriptor_count() + 1;
    g_run_menu_call = 0;
    g_next_run_menu_choice[0] = 3;      // pick “FPS”
    g_next_run_menu_choice[1] = count-1; // then “Back”

    // 3) Next read_line returns "9999" (out of range)
    g_next_input_line = "9999";

    // 4) Run it
    UI ui = make_fake_ui();
    run_settings_menu_ui(&ui);

    // 5) FPS should remain default
    assert_int_equal(settings_get_fps(), DEFAULT_FPS);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_change_player_symbol),
        cmocka_unit_test(test_invalid_numeric_setting),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
