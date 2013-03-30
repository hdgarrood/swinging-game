#include <stdbool.h>

#include <SDL/SDL.h>
#include <chipmunk/chipmunk.h>

#ifndef GAME_STATE_H
#define GAME_STATE_H

struct game_state {
    /* members common to all game states */
    struct game *game;
    void (*handle_events)(struct game_state *);
    void (*do_logic)(struct game_state *);
    void (*draw)(struct game_state *, SDL_Surface *);
    void (*free_state)(struct game_state *);

    /* members specific to a certain state
     * TODO: put these in a union or something
     */
    cpSpace *space;
    cpBody *ball;
};

struct game_state *make_game_state();
void free_game_state(struct game_state *state);
void game_state_handle_events(struct game_state *state);
void game_state_do_logic(struct game_state *state);
void game_state_draw(struct game_state *state, SDL_Surface *screen);

#endif
