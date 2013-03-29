#include <SDL/SDL.h>
#include <chipmunk/chipmunk.h>

#include "timer.h"
#include "game.h"

struct game *make_game()
{
    struct game *new_game = malloc(sizeof(struct game));
    init_game(new_game);
    return new_game;
}

void
init_game(struct game *game)
{
}

void
free_game(struct game *game)
{
    free(game);
}

void
game_main_loop(struct game *game)
{
    while (!game->quit)
    {
        game_handle_events(game);
        game_do_logic(game);
        game_draw(game);

        cap_framerate(game->target_fps);
    }
}

void
game_handle_events(struct game *game)
{
}

void
game_do_logic(struct game *game)
{
    cpSpaceStep(space, TARGET_SEC_PER_FRAME);
}

void
game_draw(struct game *game, SDL_Surface *screen)
{
    draw_background(screen);

    if (DEBUG)
        debug_draw_space(game->space, screen);

    SDL_Flip(screen);
}

/*
 * Cap the framerate to (hopefully) keep game speed constant.
 *
 * This function expects to be called once per step.
 * On the first call, it sets up a timer.
 * On subsequent calls, if the time since the last call is less than the target
 * fps, this function will block until that amount of time has passed.
 * Otherwise it will return immediately.
 */
void
cap_framerate(int target_fps)
{
    static struct timer *framerate_timer = make_timer();

    if (!timer.started)
    {
        /* this is the first time this function has been called */
        timer_start(framerate_timer);
        return;
    }

    int target_ms_per_frame = 1000 / target_fps;
    int remaining_ms = target_ms_per_frame - timer_get_ticks(framerate_timer);
    if (remaining_ms > 0)
        SDL_Delay(remaining_ms);

    timer_reset(framerate_timer);
    timer_start(framerate_timer);
}
