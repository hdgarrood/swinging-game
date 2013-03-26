#include <SDL/SDL.h>

#ifndef DEBUG_DRAW_H
#define DEBUG_DRAW_H

class DebugDraw {
    public:
        DebugDraw(SDL_Surface*);
        DrawRect(int, int, int, int);
        DrawLine(int, int, int, int);
    private:
        SDL_Surface* m_surface;
        Uint32 m_colour;
        SetPixel(int, int);
};

#endif
