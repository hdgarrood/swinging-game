#include <SDL/SDL.h>

#include "debug_draw.h"
#include "game.h"
#include "game_state.h"
#include "macros.h"

struct game_state
*make_game_state()
{
    struct game_state *state = malloc(sizeof(struct game_state));
    return state;
}

void
free_game_state(struct game_state *state)
{
    state->free_state(state);
}

void
game_state_handle_events(struct game_state *state)
{
    debug_puts("about to handle events");
    state->handle_events(state);
    debug_puts("done handling events");
}

void
game_state_do_logic(struct game_state *state)
{
    state->do_logic(state);
}

void
game_state_draw(struct game_state *state, SDL_Surface *screen)
{
    state->draw(state, screen);
}
