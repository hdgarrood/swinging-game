#ifndef TIMER_H_XEC50VDN
#define TIMER_H_XEC50VDN

#include <stdbool.h>

typedef struct timer {
    bool started;
    bool paused;
    int start_ticks;
    int paused_ticks;
} timer;

timer *timer_new();
void timer_free(timer *t);
void timer_reset(timer *t);
void timer_start(timer *t);
void timer_pause(timer *t);
void timer_unpause(timer *t);
int timer_get_ticks(timer *t);

#endif /* end of include guard: TIMER_H_XEC50VDN */
