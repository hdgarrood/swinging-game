#ifndef GAME_H_S7M9DYGG
#define GAME_H_S7M9DYGG

#include <stdbool.h>
#include <SDL/SDL.h>
#include <chipmunk/chipmunk.h>
#include "timer.h"

typedef struct game_state game_state;

typedef struct {
    bool quit_requested;
    cpVect mouse_pos;
    bool mouse_down;
    game_state *state;
} game;

game *make_game();
void init_game(game *);
void free_game(game *);
void game_main_loop(game *);
void cap_framerate(timer *, int);
void display_fps(int);

#endif /* end of include guard: GAME_H_S7M9DYGG */
