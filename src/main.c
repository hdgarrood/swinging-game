#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include <SDL/SDL.h>
#include <chipmunk/chipmunk.h>

#include "utils.h"
#include "game.h"
#include "macros.h"

/* SDL_Surface *load_image(std::string filename) */
/* { */
/*     SDL_Surface *loadedImage = NULL; */
/*     SDL_Surface *optimizedImage = NULL; */

/*     loadedImage = IMG_Load(filename.c_str()); */

/*     if (loadedImage != NULL) */
/*     { */
/*         optimizedImage = SDL_DisplayFormat(loadedImage); */
/*         SDL_FreeSurface(loadedImage); */
/*     } */
/*     return optimizedImage; */
/* } */

/* void apply_surface(int x, int y, SDL_Surface *source, SDL_Surface *dest) */
/* { */
/*     SDL_Rect offset; */

/*     offset.x = x; */
/*     offset.y = y; */

/*     SDL_BlitSurface(source, NULL, dest, &offset); */
/* } */

int main()
{
    SDL_Init(SDL_INIT_EVERYTHING);

    struct game *game = make_game();
    debug_putsf("%s", "entering main loop...");
    game_main_loop(game);
    debug_putsf("%s", "exiting main loop...");
    free_game(game);

    SDL_Quit();

    return 0;
}
