#include <SDL/SDL.h>
#include <chipmunk/chipmunk.h>

#include "Drawing.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

void
FillBackground(SDL_Surface* screen)
{
    DrawOptions background_opts;

    background_opts.surface = screen;
	SDL_Colour colour = { 0, 0, 0 };
    background_opts.colour = colour;

    SDLDraw_Rect(background_opts, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void
DrawSpace(cpSpace* space, SDL_Surface* screen)
{
}
