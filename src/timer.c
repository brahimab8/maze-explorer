#define _POSIX_C_SOURCE 199309L
#include "timer.h"
#include <time.h>
#include <stdbool.h>

static struct timespec _base_time;
static struct timespec _pause_time;
static double _elapsed_before_pause = 0.0;
static bool _running = false;
static bool _paused = false;

static double timespec_diff(const struct timespec *a, const struct timespec *b) {
    // returns (a - b) in seconds
    double sec  = (double)(a->tv_sec  - b->tv_sec);
    double nsec = (double)(a->tv_nsec - b->tv_nsec) / 1e9;
    return sec + nsec;
}

void timer_start(void) {
    clock_gettime(CLOCK_MONOTONIC, &_base_time);
    _elapsed_before_pause = 0.0;
    _running = true;
    _paused = false;
}

void timer_pause(void) {
    if (!_running || _paused) return;
    clock_gettime(CLOCK_MONOTONIC, &_pause_time);
    _elapsed_before_pause += timespec_diff(&_pause_time, &_base_time);
    _paused = true;
    _running = false;
}

void timer_resume(void) {
    if (!_running && _paused) {
        clock_gettime(CLOCK_MONOTONIC, &_base_time);
        _paused = false;
        _running = true;
    }
}

double timer_get_elapsed(void) {
    if (_paused) {
        return _elapsed_before_pause;
    } else if (_running) {
        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        return _elapsed_before_pause + timespec_diff(&now, &_base_time);
    } else {
        return _elapsed_before_pause;
    }
}
