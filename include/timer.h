#ifndef TIMER_H
#define TIMER_H

void timer_start(void);

void timer_pause(void);

void timer_resume(void);

double timer_get_elapsed(void);

void timer_set_start_time(double seconds);

#endif // TIMER_H
