#include <SDL/SDL.h>
#include <chipmunk/chipmunk.h>

#ifndef DEBUG_DRAW_H
#define DEBUG_DRAW_H

void draw_background(SDL_Surface* screen);
void debug_draw_space(cpSpace* space, SDL_Surface* screen);

#endif
