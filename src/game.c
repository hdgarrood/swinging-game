#include <SDL/SDL.h>
#include <chipmunk/chipmunk.h>

#include "timer.h"
#include "game.h"

struct game *make_game()
{
    struct game *new_game = malloc(sizeof(struct game));
    init_game(new_game);
    return new_game;
}

void
init_game(struct game *game)
{
}

void
free_game(struct game *game)
{
    free(game);
}

void
game_main_loop(struct game *game)
{
    while (!game->quit)
    {
        struct timer *step_timer = make_timer();
        timer_start(step_timer);
    }
}
