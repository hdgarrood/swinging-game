#include <SDL/SDL.h>
#include <chipmunk/chipmunk.h>

#ifndef GAME_STATE_H
#define GAME_STATE_H

struct game_state {
    cpSpace *space;
    struct game *game;
};

struct game_state *make_game_state();
void free_game_state(struct game_state *state);
void game_state_handle_events(struct game_state *state);
void game_state_do_logic(struct game_state *state);
void game_state_draw(struct game_state *state, SDL_Surface *screen);

#endif
