#include <SDL/SDL.h>

#include "debug_draw.h"
#include "game.h"
#include "game_state.h"
#include "macros.h"

cpBody *ball= NULL;

static cpSpace
*create_space()
{
    cpVect gravity = cpv(0, 150);

    cpSpace *space = cpSpaceNew();
    cpSpaceSetGravity(space, gravity);

    // create the ground
    cpShape *ground = cpSegmentShapeNew(space->staticBody,
                                        cpv(0, 300),
                                        cpv(640, 480),
                                        0);
    cpShapeSetElasticity(ground, 1.0);
    cpShapeSetFriction(ground, 1);
    cpSpaceAddShape(space, ground);

    // create the ball
    cpFloat radius = 40;
    cpFloat mass = 10;
    cpFloat moment = cpMomentForCircle(mass, 0, radius, cpvzero);

    ball = cpSpaceAddBody(space, cpBodyNew(mass, moment));
    cpBodySetPos(ball, cpv(40, 40));

    cpShape *ball_shape = cpSpaceAddShape(
            space,
            cpCircleShapeNew(ball,
                radius,
                cpvzero));

    cpShapeSetElasticity(ball_shape, 0.7);
    cpShapeSetFriction(ball_shape, 0.7);

    return space;
}

struct game_state
*make_game_state()
{
    struct game_state *state = malloc(sizeof(struct game_state));
    state->space = create_space();
    return state;
}

void
free_game_state(struct game_state *state)
{
    cpSpaceFree(state->space);
    free(state);
}

void
game_state_handle_events(struct game_state *state)
{
    /* retrieve mouse button 1 state and mouse position */
    int x, y;
    Uint8 mouse_state = SDL_GetMouseState(&x, &y);

    state->mouse_x = x;
    state->mouse_y = y;
    state->mouse_down = (mouse_state & SDL_BUTTON(1));

    if (SDL_QuitRequested())
        state->game->quit = true;
}

void
game_state_do_logic(struct game_state *state)
{
    cpFloat target_sec_per_frame = 1.0 / state->game->target_fps;
    debug_println("target sec per frame is %f", target_sec_per_frame);

    cpVect pos = cpBodyGetVel(ball);
    debug_println("ball speed is (%f, %f)", pos.x, pos.y);
    cpSpaceStep(state->space, target_sec_per_frame);
}

void
game_state_draw(struct game_state *state, SDL_Surface *screen)
{
    draw_background(screen);

    if (DEBUG)
        debug_draw_space(state->space, screen);

    SDL_Flip(screen);
}
