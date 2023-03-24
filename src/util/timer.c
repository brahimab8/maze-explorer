#define _POSIX_C_SOURCE 199309L
#include "util/timer.h"
#include <time.h>
#include <stdbool.h>

static struct timespec _start_time;
static double          _offset;     // seconds already elapsed before _start_time
static bool            _running;    // true if clock is ticking

// Compute (new − old) in seconds
static double timespec_diff(const struct timespec *new, const struct timespec *old) {
    long sec  = new->tv_sec  - old->tv_sec;
    long nsec = new->tv_nsec - old->tv_nsec;
    if (nsec < 0) {
        sec  -= 1;
        nsec += 1000000000L;
    }
    return (double)sec + (double)nsec / 1e9;
}


void timer_start(void) {
    clock_gettime(CLOCK_MONOTONIC, &_start_time);
    _offset   = 0.0;
    _running  = true;
}

void timer_pause(void) {
    if (!_running) return;
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    _offset  += timespec_diff(&now, &_start_time);
    _running  = false;
}

void timer_resume(void) {
    if (_running) return;
    clock_gettime(CLOCK_MONOTONIC, &_start_time);
    _running = true;
}

double timer_get_elapsed(void) {
    if (_running) {
        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        return _offset + timespec_diff(&now, &_start_time);
    } else {
        return _offset;
    }
}

void timer_set_start_time(double seconds) {
    // Stop the clock, set offset to the desired start time.
    _running = false;
    _offset  = seconds;
}
