#include <SDL/SDL.h>
#include <chipmunk/chipmunk.h>

#include "timer.h"
#include "game.h"
#include "game_state.h"
#include "utils.h"
#include "constants.h"
#include "macros.h"

game
*make_game()
{
    game *new_game = malloc(sizeof(game));
    init_game(new_game);
    return new_game;
}

void
init_game(game *game)
{
    game->quit_requested = false;
    game->mouse_down = false;
    game->state = switch_state_new();

    /* let the state know about its game */
    game->state->game = game;
}

void
free_game(game *game)
{
    game_state_free(game->state);
    free(game);
}

void
game_get_input(game *game)
{
    int x, y;
    Uint8 mouse_state = SDL_GetMouseState(&x, &y);

    game->mouse_pos = cpv(x, y);
    game->mouse_down = (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT));

    if (SDL_QuitRequested())
        game->quit_requested = true;

    /* discard all other events */
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        debug_putsf("ignoring an event of type: %d", event.type);
    }
}

void
game_main_loop(game *game)
{
    SDL_Surface *screen = SDL_SetVideoMode(
            SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

    timer *fps_timer = timer_new();

    while (!game->quit_requested)
    {
        debug_puts("starting a loop");

        game_get_input(game);
        debug_puts("got input.");
        game_state_handle_events(game->state);
        debug_puts("handled events.");

        game_state_do_logic(game->state);
        debug_puts("done logic.");

        game_state_draw(game->state, screen);
        debug_puts("done drawing.");

        cap_framerate(fps_timer, TARGET_FPS);
    }

    timer_free(fps_timer);
    SDL_FreeSurface(screen);
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
cap_framerate(timer *fps_timer, int target_fps)
{
    if (!fps_timer->started)
    {
        /* this is the first time this function has been called */
        timer_start(fps_timer);
        return;
    }

    int target_ms_per_frame = 1000 / target_fps;
    int remaining_ms = target_ms_per_frame - timer_get_ticks(fps_timer);
    if (remaining_ms > 0)
        SDL_Delay(remaining_ms);

    int total_step_time = timer_get_ticks(fps_timer);
    display_fps(total_step_time);

    timer_reset(fps_timer);
    timer_start(fps_timer);
}

void
display_fps(int frame_time)
{
    /* store up to 60 previous frame times */
    static int frame_times[60];
    static int index = 0;

    frame_times[index] = frame_time;
    index = (index + 1) % 60;

    if (index == 0)
    {
        char title[10];
        int average_fps = 1000 / array_average(frame_times, 60);

        sprintf(title, "FPS: %d", average_fps);
        SDL_WM_SetCaption(title, NULL);
    }
}
