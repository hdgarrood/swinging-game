#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include <SDL/SDL.h>
#include <chipmunk/chipmunk.h>

#include "Drawing.h"
#include "DebugDraw.h"
#include "Timer.h"

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP    = 32;
const int TARGET_FPS    = 60;

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

cpSpace*
CreateSpace()
{
    cpVect gravity = cpv(0, 150);

    cpSpace* space = cpSpaceNew();
    cpSpaceSetGravity(space, gravity);

    // create the ground
    cpShape* ground = cpSegmentShapeNew(space->staticBody,
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

    cpBody* ballBody = cpSpaceAddBody(space, cpBodyNew(mass, moment));
    cpBodySetPos(ballBody, cpv(40, 40));

    cpShape* ballShape = cpSpaceAddShape(space,
                                         cpCircleShapeNew(ballBody,
                                                          radius,
                                                          cpvzero));
    cpShapeSetElasticity(ballShape, 0.7);
    cpShapeSetFriction(ballShape, 0.7);

    return space;
}

void
CatchSignalAndDie(int s)
{
    printf("Caught signal %d\n", s);
    exit(1);
}

void
SetupSignals()
{
    /* die on SIGINT */
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = CatchSignalAndDie;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
}

int main(int argc, char *argv[])
{
    SetupSignals();

    puts("About to SDL_Init");
    SDL_Init(SDL_INIT_EVERYTHING);

    puts("About to create screen surface");
    SDL_Surface *screen = NULL;
    screen = SDL_SetVideoMode(SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SCREEN_BPP,
            SDL_SWSURFACE);

    puts("About to create space");
    cpSpace* space = CreateSpace();

    cpFloat time_step = 1.0 / TARGET_FPS;
    timer my_timer;
    timer *timer_ptr = &my_timer;

    for (cpFloat time=0; time < 7; time += time_step)
    {
        puts("about to reset timer");
        timer_reset(timer_ptr);
        puts("about to start timer");
        timer_start(timer_ptr);

        puts("About to step");
        cpSpaceStep(space, time_step);

        puts("About to draw world");
        FillBackground(screen);
        DrawSpace(space, screen);
        SDL_Flip(screen);

        printf("that took: %d\n", timer_get_ticks(timer_ptr));
        /* if (remaining_ms > 0) */
        SDL_Delay(16);
    }

    cpSpaceFree(space);

    SDL_FreeSurface(screen);
    SDL_Quit();

    return 0;
}
