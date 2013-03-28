#include <SDL/SDL.h>
#include <chipmunk/chipmunk.h>

#ifndef DEBUG_DRAW_H
#define DEBUG_DRAW_H

void FillBackground(SDL_Surface* screen);
void DrawSpace(cpSpace* space, SDL_Surface* screen);

#endif
