#include <algorithm>
#include <math.h>
#include <SDL/SDL.h>

#include "Drawing.h"

// General-purpose drawing functions. These should only assume use of SDL;
// Box2D should not be mentioned.
//
// All functions should take a DrawOptions as their first parameter.
//
// All functions take care of locking the surface for you.

void SetPixel(DrawOptions opts, int x, int y)
{
    if (SDL_MUSTLOCK(opts.surface))
        SDL_LockSurface(opts.surface);

    Uint32 *pixels = (Uint32*)opts.surface->pixels;
    pixels[(opts.surface->w * y) + x] = opts.colour;

    if (SDL_MUSTLOCK(opts.surface))
        SDL_UnlockSurface(opts.surface);
}

void Draw_Rect(DrawOptions opts, int x, int y, int w, int h)
{
    SDL_Rect rect = {x, y, w, h};
    SDL_FillRect(opts.surface, &rect, opts.colour);
}

// lifted from:
// http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm#Algorithm_with_Integer_Arithmetic
void Draw_Line(DrawOptions opts, int x0, int y0, int x1, int y1)
{
    if (fabs(y1 - y0) > fabs(x1 - x0))
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = y1 - y0;
    int D = 2*dy - dx;
    int y = y0;

    SetPixel(opts, x0, y0);

    for (int x = x0 + 1; x != x1; x++)
    {
        if (D > 0)
        {
            y++;
            SetPixel(opts, x, y);
            D += ((2 * dy) - (2 * dx));
        }
        else
        {
            SetPixel(opts, x, y);
            D += 2 * dy;
        }
    }
}

void Draw_Circle(DrawOptions opts, int x0, int y0, int radius)
{
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    SetPixel(opts, x0, y0 + radius);
    SetPixel(opts, x0, y0 - radius);
    SetPixel(opts, x0 + radius, y0);
    SetPixel(opts, x0 - radius, y0);

    while(x < y)
    {
        if (f >= 0)
        {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
        SetPixel(opts, x0 + x, y0 + y);
        SetPixel(opts, x0 - x, y0 + y);
        SetPixel(opts, x0 + x, y0 - y);
        SetPixel(opts, x0 - x, y0 - y);
        SetPixel(opts, x0 + y, y0 + x);
        SetPixel(opts, x0 - y, y0 + x);
        SetPixel(opts, x0 + y, y0 - x);
        SetPixel(opts, x0 - y, y0 - x);
    }
}
