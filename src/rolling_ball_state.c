#include <SDL/SDL.h>
#include <chipmunk/chipmunk.h>

#include "game.h"
#include "game_state.h"
#include "debug_draw.h"
#include "constants.h"
#include "macros.h"

static cpSpace *create_space(struct game_state *state);
static void handle_events(struct game_state *state);
static void do_logic(struct game_state *state);
static void draw(struct game_state *state, SDL_Surface *screen);

struct game_state
*make_rolling_ball_state()
{
    debug_puts("making a rolling ball state");
    struct game_state *state = make_game_state();

    state->handle_events = &handle_events;
    state->do_logic = &do_logic;
    state->draw = &draw;

    state->space = create_space(state);
    state->constraint = NULL;
    state->mouse_body = NULL;

    return state;
}

static void
handle_events(struct game_state *state)
{
    debug_puts("inside handle_events");

    if (state->mouse_body == NULL)
    {
        debug_puts("mouse body is NULL");
        if (state->game->mouse_down)
        {
            debug_puts("creating mouse body");
            cpBody *mouse_body = cpBodyNewStatic();
            state->constraint = cpPinJointNew(mouse_body, state->ball,
                                              cpvzero, cpvzero);

            state->mouse_body = mouse_body;
        }
    }
    else
    {
        if (!state->game->mouse_down)
        {
            debug_puts("freeing mouse body");
            cpConstraintFree(state->constraint);
            state->constraint = NULL;
            cpBodyFree(state->mouse_body);
            state->mouse_body = NULL;
        }
    }
}

static cpSpace
*create_space(struct game_state *state)
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

    cpBody *ball = cpSpaceAddBody(space, cpBodyNew(mass, moment));
    cpBodySetPos(ball, cpv(40, 40));

    cpShape *ball_shape = cpSpaceAddShape(space,
            cpCircleShapeNew(ball, radius, cpvzero));

    cpShapeSetElasticity(ball_shape, 0.7);
    cpShapeSetFriction(ball_shape, 0.7);

    state->ball = ball;
    return space;
}

static void
do_logic(struct game_state *state)
{
    debug_puts("doing logic");
    cpVect pos = cpBodyGetVel(state->ball);
    debug_putsf("ball speed is (%f, %f)", pos.x, pos.y);
    cpSpaceStep(state->space, TARGET_SEC_PER_FRAME);
}

static void
draw(struct game_state *state, SDL_Surface *screen)
{
    draw_background(screen);

    if (HAS_DEBUG)
        debug_draw_space(state->space, screen);

    SDL_Flip(screen);
}
