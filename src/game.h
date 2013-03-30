#include <stdbool.h>
#include <SDL/SDL.h>
#include <chipmunk/chipmunk.h>

#include "game_state.h"
#include "timer.h"

#ifndef GAME_H
#define GAME_H

struct game {
    bool quit_requested;
    cpVect mouse_pos;
    bool mouse_down;
    struct game_state *state;
};

struct game *make_game();
void init_game(struct game *);
void free_game(struct game *);
void game_main_loop(struct game *);
void cap_framerate(struct timer *, int);
void display_fps(int);

#endif
