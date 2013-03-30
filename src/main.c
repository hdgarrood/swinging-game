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

cpSpace
*create_space()
{
    cpVect gravity = cpv(0, 150);

    cpSpace *space = cpSpaceNew();
    cpSpaceSetGravity(space, gravity);

    // create the ground
    cpShape *ground = cpSegmentShapeNew(space->staticBody,
                                        cpv(0, 300),
                                        cpv(640, 480),
                                        0);
    cpShapeSetElasticity(ground, 1.0);
    cpShapeSetFriction(ground, 1);
    cpSpaceAddShape(space, ground);

    // create the ball
    cpFloat radius = 40;
    cpFloat mass = 10;
    cpFloat moment = cpMomentForCircle(mass, 0, radius, cpvzero);

    cpBody *ballBody = cpSpaceAddBody(space, cpBodyNew(mass, moment));
    cpBodySetPos(ballBody, cpv(40, 40));

    cpShape *ballShape = cpSpaceAddShape(space,
                                         cpCircleShapeNew(ballBody,
                                                          radius,
                                                          cpvzero));
    cpShapeSetElasticity(ballShape, 0.7);
    cpShapeSetFriction(ballShape, 0.7);

    return space;
}

void
catch_signal_and_die(int signal)
{
    printf("Caught signal %d\n", signal);
    exit(1);
}

/*
 * use sigaction to register a SIGINT handler, which causes the program to
 * exit.
 */
void
setup_signals()
{
    struct sigaction sigint_handler;
    sigint_handler.sa_handler = catch_signal_and_die;
    sigemptyset(&sigint_handler.sa_mask);
    sigint_handler.sa_flags = 0;
    sigaction(SIGINT, &sigint_handler, NULL);
}

int main()
{
    setup_signals();
    SDL_Init(SDL_INIT_EVERYTHING);

    struct game *game = make_game();
    debug_println("%s", "entering main loop...");
    game_main_loop(game);
    debug_println("%s", "exiting main loop...");

    SDL_Quit();

    return 0;
}
