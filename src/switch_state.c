#include <SDL/SDL.h>
#include <chipmunk/chipmunk.h>

#include "game.h"
#include "game_state.h"
#include "ent_switch.h"
#include "debug_draw.h"
#include "constants.h"
#include "macros.h"
#include "utils.h"

static void set_function_pointers(game_state *state);
static void allocate_state_data(game_state *state);
static void create_physics_objects(game_state *state);

static void handle_events(game_state *state);
static void do_logic(game_state *state);
static void draw(game_state *state, SDL_Surface *screen);
static void free_switch_state(game_state *state);

game_state
*switch_state_new()
{
    game_state *state = game_state_new();
    set_function_pointers(state);
    allocate_state_data(state);
    create_physics_objects(state);
    return state;
}

static void
set_function_pointers(game_state *state)
{
    state->handle_events = &handle_events;
    state->do_logic = &do_logic;
    state->draw = &draw;
    state->free = &free_switch_state;
}

static void
allocate_state_data(game_state *state)
{
    state->data->switch_data = malloc(sizeof(switch_state_data));
}

static void
create_physics_objects(game_state *state)
{
    switch_state_data *data = state->data->switch_data;

    cpSpace *space = cpSpaceNew();
    cpSpaceSetGravity(space, cpv(0, 250));

    /* add borders */
    cpShape *shape = NULL;
    shape = cpSpaceAddShape(space, cpSegmentShapeNew(space->staticBody,
                cpv(0, 0),
                cpv(0, SCREEN_HEIGHT),
                0));
    cpShapeSetLayers(shape, L_GROUND);

    shape =cpSpaceAddShape(space, cpSegmentShapeNew(space->staticBody,
                cpv(0, SCREEN_HEIGHT),
                cpv(SCREEN_WIDTH, SCREEN_HEIGHT),
                0));
    cpShapeSetLayers(shape, L_GROUND);

    shape = cpSpaceAddShape(space, cpSegmentShapeNew(space->staticBody,
                cpv(SCREEN_WIDTH, SCREEN_HEIGHT),
                cpv(SCREEN_WIDTH, 0),
                0));
    cpShapeSetLayers(shape, L_GROUND);

    shape = cpSpaceAddShape(space, cpSegmentShapeNew(space->staticBody,
                cpv(SCREEN_WIDTH, 0),
                cpv(0, 0),
                0));
    cpShapeSetLayers(shape, L_GROUND);

    ent_switch *sw = ent_switch_new(space,
        cpv(SCREEN_WIDTH / 2, SCREEN_HEIGHT), 0);

    data->space = space;
    data->sw = sw;
}

static void
handle_events(game_state *state)
{
}

static void
do_logic(game_state *state)
{
    switch_state_data *data = state->data->switch_data;

    if (data->mouse_down_last_step && state->game->mouse_down == false)
    {
        /* clicked and released; create a circle */
        cpFloat radius = 15;
        cpFloat mass = 10;
        cpFloat moment = cpMomentForCircle(mass, 0, radius, cpvzero);

        cpBody *ball = cpSpaceAddBody(data->space, cpBodyNew(mass, moment));
        cpBodySetPos(ball, state->game->mouse_pos);

        cpShape *ball_shape = cpSpaceAddShape(data->space,
                cpCircleShapeNew(ball, radius, cpvzero));

        cpShapeSetElasticity(ball_shape, 0.7);
        cpShapeSetFriction(ball_shape, 0.7);
        cpShapeSetLayers(ball_shape, L_PLAYER);
    }

    cpSpaceStep(data->space, TARGET_SEC_PER_FRAME);
    data->mouse_down_last_step = state->game->mouse_down;
}

static void
draw(game_state *state, SDL_Surface *screen)
{
    switch_state_data *data = state->data->switch_data;

    draw_background(screen);

    if (HAS_DEBUG)
        debug_draw_space(data->space, screen);

    SDL_Flip(screen);
}

static void
free_switch_state(game_state *state)
{
}
