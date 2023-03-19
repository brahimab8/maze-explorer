#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "timer.h"
#include <unistd.h>
#include <math.h>

// Helper for “approximately equal”
static void assert_near(double got, double want, double tol) {
    assert_true(fabs(got - want) <= tol);
}

// --- Test cases -------------------------------------------------------------
static void test_immediate_elapsed_zero(void **state) {
    (void)state;
    timer_start();
    double e = timer_get_elapsed();
    assert_near(e, 0.0, 0.01);
}

static void test_sleep_100ms(void **state) {
    (void)state;
    timer_start();
    usleep(100000);
    double e = timer_get_elapsed();
    assert_near(e, 0.1, 0.02);
}

static void test_pause_resume(void **state) {
    (void)state;
    // run: 50ms, pause 100ms, resume 50ms ⇒ ~100ms total
    timer_start();
    usleep(50000);
    timer_pause();
    usleep(100000);
    timer_resume();
    usleep(50000);
    double e = timer_get_elapsed();
    assert_near(e, 0.1, 0.02);
}

// --- Test runner ---------------------------------------------------------
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_immediate_elapsed_zero),
        cmocka_unit_test(test_sleep_100ms),
        cmocka_unit_test(test_pause_resume),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
