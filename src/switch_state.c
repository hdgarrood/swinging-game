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
create_physics_objects(game_state *state)
{
    switch_state_data *data = state->data->switch_data;

    cpSpace *space = cpSpaceNew();



    data->space = space;
}

static void
handle_events(game_state *state)
{
}

static void
do_logic(game_state *state)
{
    switch_state_data *data = state->data->switch_data;
    cpSpaceStep(data->space, TARGET_SEC_PER_FRAME);
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
