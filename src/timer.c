#include <SDL/SDL.h>

#include "timer.h"

struct timer*
make_timer()
{
	struct timer *t = malloc(sizeof(struct timer));
	timer_reset(t);
	return t;
}

void
free_timer(struct timer *t)
{
	free(t);
}

void
timer_reset(struct timer *t)
{
	t->started = false;
	t->paused = false;
	t->start_ticks = 0;
	t->paused_ticks = 0;
}

void
timer_start(struct timer *t)
{
	if (t->started)
		return;

	t->start_ticks = SDL_GetTicks();
	t->started = true;
}

void
timer_pause(struct timer *t)
{
	if (t->paused || !t->started)
		return;

	t->paused_ticks = SDL_GetTicks();
	t->paused = true;
}

void
timer_unpause(struct timer *t)
{
	if (!t->paused)
		return;

	int time_paused = SDL_GetTicks() - t->paused_ticks;
	t->start_ticks += time_paused;
	t->paused = false;
}

int
timer_get_ticks(struct timer *t)
{
	if (t->started)
		return (SDL_GetTicks() - t->start_ticks);
	else
		return 0;
}
