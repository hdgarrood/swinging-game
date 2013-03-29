#include <stdbool.h>
#include <SDL/SDL.h>

#ifndef GAME_H
#define GAME_H

struct game {
    bool quit;
};

void init_game(struct game *game);
void free_game(struct game *game);
void game_main_loop(struct game *game);

#endif
