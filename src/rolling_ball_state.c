#include <SDL/SDL.h>
#include <chipmunk/chipmunk.h>

#include "game.h"
#include "game_state.h"
#include "debug_draw.h"
#include "constants.h"
#include "macros.h"
#include "utils.h"

static cpSpace *create_space(game_state *state);
static void handle_events(game_state *state);
static void do_logic(game_state *state);
static void draw(game_state *state, SDL_Surface *screen);

game_state
*rolling_ball_state_new()
{
    debug_puts("making a rolling ball state");
    game_state *state = game_state_new();

    state->handle_events = &handle_events;
    state->do_logic = &do_logic;
    state->draw = &draw;

    rolling_ball_state_data *data = malloc(sizeof(rolling_ball_state_data));
    state->data->rolling_ball_data = data;
    data->space = create_space(state);
    debug_putsf("made a space: %p", data->space);
    data->constraint = NULL;
    data->mouse_body = NULL;

    debug_putsf("made a rolling ball state: %p", state);

    return state;
}

static void
handle_events(game_state *state)
{
    debug_puts("inside handle_events");
    rolling_ball_state_data *data = state->data->rolling_ball_data;

    cpBodyResetForces(data->ball);

    if (data->mouse_body == NULL)
    {
        debug_puts("mouse body is NULL");
        if (state->game->mouse_down)
        {
            debug_puts("creating mouse body");

            /* create the mouse body */
            cpBody *mouse_body = cpBodyNewStatic();
            cpBodySetPos(mouse_body, state->game->mouse_pos);

            /* data->constraint = cpSlideJointNew( */
            /*         mouse_body, data->ball, */
            /*         cpvzero, cpvzero, */
            /*         0, */
            /*         cpvdist(cpBodyGetPos(mouse_body), cpBodyGetPos(data->ball))); */
            /* cpSpaceAddConstraint(data->space, data->constraint); */

            data->mouse_body = mouse_body;
        }
    }
    else
    {
        if (state->game->mouse_down)
        {
            /* move towards the pivot */
            cpFloat direction = angle_between(cpBodyGetPos(data->ball),
                                              cpBodyGetPos(data->mouse_body));
            cpFloat magnitude = 3500;
            cpVect force = cpvmult(cpvforangle(direction), magnitude);
            cpBodyApplyForce(data->ball, force, cpvzero);
        }
        else
        {
            /* cpSpaceRemoveConstraint(data->space, data->constraint); */
            /* debug_puts("freeing mouse body"); */
            /* cpConstraintFree(data->constraint); */
            /* data->constraint = NULL; */
            cpBodyFree(data->mouse_body);
            data->mouse_body = NULL;
        }
    }
}

static cpSpace
*create_space(game_state *state)
{
    rolling_ball_state_data *data = state->data->rolling_ball_data;
    cpVect gravity = cpv(0, 250);

    cpSpace *space = cpSpaceNew();
    cpSpaceSetGravity(space, gravity);

    // create the ground
    cpVect ground_left = cpv(0, 300);
    cpVect ground_right = cpv(640, 480);
    cpFloat ground_angle = cpvtoangle(cpvsub(ground_right, ground_left));

    cpShape *ground = cpSegmentShapeNew(
            space->staticBody, ground_left, ground_right, 0);

    cpShapeSetElasticity(ground, 1.0);
    cpShapeSetFriction(ground, 1);
    cpShapeSetLayers(ground, L_GROUND);
    cpSpaceAddShape(space, ground);

    // create the ball
    cpFloat radius = 30;
    cpFloat mass = 10;
    cpFloat moment = cpMomentForCircle(mass, 0, radius, cpvzero);

    cpBody *ball = cpSpaceAddBody(space, cpBodyNew(mass, moment));
    cpBodySetPos(ball, cpv(40, 40));

    cpShape *ball_shape = cpSpaceAddShape(space,
            cpCircleShapeNew(ball, radius, cpvzero));

    cpShapeSetElasticity(ball_shape, 0.7);
    cpShapeSetFriction(ball_shape, 0.7);
    cpShapeSetLayers(ball_shape, L_PLAYER);

    data->ball = ball;
    return space;
}

static void
do_logic(game_state *state)
{
    rolling_ball_state_data *data = state->data->rolling_ball_data;

    debug_puts("doing logic");
    cpVect pos = cpBodyGetVel(data->ball);
    debug_putsf("ball speed is (%f, %f)", pos.x, pos.y);

    cpSpaceStep(data->space, TARGET_SEC_PER_FRAME);
}

static void
draw(game_state *state, SDL_Surface *screen)
{
    rolling_ball_state_data *data = state->data->rolling_ball_data;

    draw_background(screen);

    if (HAS_DEBUG)
        debug_draw_space(data->space, screen);

    SDL_Flip(screen);
}
