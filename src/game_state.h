#ifndef GAME_STATE_H_SEAIAXNJ
#define GAME_STATE_H_SEAIAXNJ

#include <stdbool.h>
#include <SDL/SDL.h>
#include <chipmunk/chipmunk.h>
#include "game.h"
#include "ent_switch.h"

typedef enum {
    ROLLING_BALL_STATE,
} e_state_type;

/* rolling ball state type */
typedef struct {
    cpSpace *space;
    cpBody *ball;
    cpBody *mouse_body;
    cpConstraint *constraint;
} rolling_ball_state_data;

/* switch state type */
typedef struct {
    cpSpace *space;
    cpBody *ball;
    ent_switch *sw;
} switch_state_data;


/* generic game state type */
typedef struct game_state {
    void (*handle_events)(game_state *);
    void (*do_logic)(game_state *);
    void (*draw)(game_state *, SDL_Surface *);
    void (*free)(game_state *);

    /* the game which this game_state belongs to */
    game *game;

    /* the type of state */
    e_state_type type;

    /* anything specific to a certain state goes here */
    union state_data {
        rolling_ball_state_data *rolling_ball_data;
        switch_state_data *switch_data;
    } *data;
} game_state;

/* rolling ball state functions */
game_state *rolling_ball_state_new();
void rolling_ball_state_free(game_state *);

/* generic game state functions */
game_state *game_state_new();
void game_state_handle_events(game_state *);
void game_state_do_logic(game_state *);
void game_state_draw(game_state *, SDL_Surface *);
void game_state_free(game_state *);

#endif /* end of include guard: GAME_STATE_H_SEAIAXNJ */
