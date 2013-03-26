#include <SDL/SDL.h>

#ifndef DRAWING_H
#define DRAWING_H

struct DrawOptions {
    SDL_Surface* surface;
    Uint32       colour;
};

void Draw_Rect(DrawOptions, int, int, int, int);
void Draw_Line(DrawOptions, int, int, int, int);
void Draw_Circle(DrawOptions, int, int, int);

#endif
