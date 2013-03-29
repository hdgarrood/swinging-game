#include <stdbool.h>

#ifndef TIMER_H
#define TIMER_H

struct timer {
	bool started;
	bool paused;
	int start_ticks;
	int paused_ticks;
};

struct timer *make_timer();
void free_timer(struct timer *t);
void timer_reset(struct timer *t);
void timer_start(struct timer *t);
void timer_pause(struct timer *t);
void timer_unpause(struct timer *t);
int timer_get_ticks(struct timer *t);

#endif
