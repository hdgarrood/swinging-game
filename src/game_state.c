#include <SDL/SDL.h>

#include "debug_draw.h"
#include "game.h"
#include "game_state.h"
#include "macros.h"

game_state
*game_state_new()
{
    game_state *state = malloc(sizeof(game_state));

	state->handle_events = NULL;
	state->do_logic = NULL;
	state->draw = NULL;
    state->free = NULL;
	state->data = malloc(sizeof(union state_data));

    return state;
}

void
game_state_free(game_state *state)
{
    if (state->free != NULL)
        state->free(state);
	else
		debug_putsf("WARNING: state->free missing for %p", state);
}

void
game_state_handle_events(game_state *state)
{
    debug_puts("about to handle events");
    state->handle_events(state);
    debug_puts("done handling events");
}

void
game_state_do_logic(game_state *state)
{
    state->do_logic(state);
}

void
game_state_draw(game_state *state, SDL_Surface *screen)
{
    state->draw(state, screen);
}
