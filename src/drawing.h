#ifndef DRAWING_H_H3STOXSN
#define DRAWING_H_H3STOXSN

#include <SDL/SDL.h>

struct draw_options {
    SDL_Surface* surface;
    SDL_Colour   colour;
};

void sdldraw_rect(struct draw_options, int, int, int, int);
void sdldraw_line(struct draw_options, int, int, int, int);
void sdldraw_circle(struct draw_options, int, int, int);

#endif /* end of include guard: DRAWING_H_H3STOXSN */
