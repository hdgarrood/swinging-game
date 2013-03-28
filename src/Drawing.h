#include <SDL/SDL.h>

#ifndef DRAWING_H
#define DRAWING_H

typedef struct DrawOptions_ {
    SDL_Surface* surface;
    SDL_Colour   colour;
} DrawOptions;

void SDLDraw_Rect(const DrawOptions, const int, const int, const int, const int);
void SDLDraw_Line(const DrawOptions, const int, const int, const int, const int);
void SDLDraw_Circle(const DrawOptions, const int, const int, const int);

#endif
