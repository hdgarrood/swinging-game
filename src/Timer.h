#include <stdbool.h>

#ifndef TIMER_H
#define TIMER_H

typedef struct timer {
	bool started;
	bool paused;
	int start_ticks;
	int paused_ticks;
} timer;

void timer_reset(timer *t);
void timer_start(timer *t);
void timer_pause(timer *t);
void timer_unpause(timer *t);
int timer_get_ticks(timer *t);

#endif
