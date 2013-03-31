#ifndef DEBUG_DRAW_H_ZR50TIJM
#define DEBUG_DRAW_H_ZR50TIJM

#include <SDL/SDL.h>
#include <chipmunk/chipmunk.h>

void draw_background(SDL_Surface* screen);
void debug_draw_space(cpSpace* space, SDL_Surface* screen);

#endif /* end of include guard: DEBUG_DRAW_H_ZR50TIJM */
