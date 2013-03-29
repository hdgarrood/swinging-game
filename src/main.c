#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include <SDL/SDL.h>
#include <chipmunk/chipmunk.h>

#include "drawing.h"
#include "debug_draw.h"
#include "timer.h"

#define DEBUG 1

#define debug_println(fmt, ...) \
        do { if (DEBUG) fprintf(stderr, "%s:%d in %s(): " fmt "\n", __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); } while (0)

#define debug_print(fmt, ...) \
        do { if (DEBUG) fprintf(stderr, "%s:%d in %s(): " fmt, __FILE__, \
                                __LINE__, __func__, __VA_ARGS__); } while (0)

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP    = 32;

const int TARGET_FPS    = 60;
const cpFloat TARGET_MS_PER_FRAME  = 16.67;
const cpFloat TARGET_SEC_PER_FRAME = 0.01667;

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
create_space()
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

double
array_average(int array[], size_t size)
{
    int sum = 0;

    for (int i = 0; i < size; i++)
        sum += array[i];

    return ((double)sum / size);
}

void
display_fps(struct timer *fps_timer)
{
    /* store up to 60 previous step times */
    static int step_times[60];
    static int index = 0;
    char title[50];
    int fps = 0;

    step_times[index] = timer_get_ticks(fps_timer);

    timer_reset(fps_timer);
    timer_start(fps_timer);
    index = (index + 1) % 60;

    if (index == 59)
    {
        int average_fps = 1000 / array_average(step_times, 60);
        sprintf(title, "FPS: %d", average_fps);
        SDL_WM_SetCaption(title, NULL);
    }
}

int main()
{
    setup_signals();

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Surface *screen = NULL;
    screen = SDL_SetVideoMode(SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SCREEN_BPP,
            SDL_SWSURFACE);

    cpSpace *space = create_space();

    struct timer *fps_timer = make_timer();
    timer_start(fps_timer);

    for (cpFloat time=0; time < 7; time += TARGET_SEC_PER_FRAME)
    {
        struct timer *step_timer = make_timer();
        timer_start(step_timer);


        cpSpaceStep(space, TARGET_SEC_PER_FRAME);

        FillBackground(screen);
        DrawSpace(space, screen);
        SDL_Flip(screen);

        int remaining_ms = TARGET_MS_PER_FRAME - timer_get_ticks(step_timer);
        if (remaining_ms > 0)
            SDL_Delay(remaining_ms);

        free_timer(step_timer);

        set_fps_caption(fps_timer);
    }

    free_timer(fps_timer);

    cpSpaceFree(space);

    SDL_FreeSurface(screen);
    SDL_Quit();

    return 0;
}
