#include <SDL/SDL.h>

#ifndef DRAWING_H
#define DRAWING_H

struct draw_options {
    SDL_Surface* surface;
    SDL_Colour   colour;
};

void sdldraw_rect(struct draw_options, int, int, int, int);
void sdldraw_line(struct draw_options, int, int, int, int);
void sdldraw_circle(struct draw_options, int, int, int);

#endif
