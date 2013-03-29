#include <SDL/SDL.h>

#ifndef DRAWING_H
#define DRAWING_H

typedef struct DrawOptions_ {
    SDL_Surface* surface;
    SDL_Colour   colour;
} DrawOptions;

void SDLDraw_Rect(DrawOptions, int, int, int, int);
void SDLDraw_Line(DrawOptions, int, int, int, int);
void SDLDraw_Circle(DrawOptions, int, int, int);

#endif
