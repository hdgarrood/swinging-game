#include <SDL/SDL.h>

#include "Drawing.h"

/*
 General-purpose drawing functions. These should only assume use of SDL; no
 physics libraries or anything in here.
 All functions should take a DrawOptions as their first parameter.
 All functions take care of locking the surface for you.
*/

#define sgn(x) ((x<0)?-1:((x>0)?1:0)) /* macro to return the sign of a
                                         number */

/* Given a DrawOptions, return a Uint32 for its colour and surface. */
static Uint32
GetColourAsUint32(const DrawOptions opts)
{
    return SDL_MapRGB(opts.surface->format,
                      opts.colour.r,
                      opts.colour.g,
                      opts.colour.b);
}

// Set pixel (x,y) on the SDL_Surface opts.surface to opts.colour
static void
SetPixel(const DrawOptions opts,
         const int x,
         const int y)
{
    if (x >= opts.surface->w || x < 0 ||
        y >= opts.surface->h || y < 0)
    {
        printf("thwarting an attempt to set pixel (%d, %d)\n",
                x, y);
        return;
    }

    if (SDL_MUSTLOCK(opts.surface))
        SDL_LockSurface(opts.surface);

    Uint32 *pixels = (Uint32*)opts.surface->pixels;

    int index = (opts.surface->w * y) + x;
    int max = (opts.surface->w * opts.surface->h);

    pixels[index] = GetColourAsUint32(opts);

    if (SDL_MUSTLOCK(opts.surface))
        SDL_UnlockSurface(opts.surface);
}

// Draw a rectangle, with the given x, y, width, and height.
void
SDLDraw_Rect(const DrawOptions opts,
             const int x,
             const int y,
             const int w,
             const int h)
{
    SDL_Rect rect = {x, y, w, h};
    SDL_FillRect(opts.surface, &rect, GetColourAsUint32(opts));
}

// lifted from:
//   http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm#Algorithm_with_Integer_Arithmetic
// FIXME : doesn't work in all quadrants
void
SDLDraw_Line(const DrawOptions opts,
             const int x0,
             const int y0,
             const int x1,
             const int y1)
{
    int dx     = x1-x0;      /* the horizontal distance of the line */
    int dy     = y1-y0;      /* the vertical distance of the line */
    int dxabs  = abs(dx);
    int dyabs  = abs(dy);
    int sdx    = sgn(dx);
    int sdy    = sgn(dy);
    int x      = dyabs>>1;
    int y      = dxabs>>1;
    int px     = x0;
    int py     = y0;

    SetPixel(opts, px, py);

    printf("dxabs = %d, dyabs = %d\n", dxabs, dyabs);
    if (dxabs >= dyabs) /* the line is more horizontal than vertical */
    {
        printf("took first path\n");
        for (int i = 0; i < dxabs; i++)
        {
            y += dyabs;
            if (y >= dxabs)
            {
                y -= dxabs;
                py += sdy;
            }
            px += sdx;
            SetPixel(opts, px, py);
        }
    }
    else /* the line is more vertical than horizontal */
    {
        printf("took second path\n");
        for (int i = 0; i < dyabs; i++)
        {
            x += dxabs;
            if (x >= dyabs)
            {
                x -= dyabs;
                px += sdx;
            }
            py += sdy;
            SetPixel(opts, px, py);
        }
    }
}

// TODO: filled circles
void
SDLDraw_Circle(const DrawOptions opts,
               const int x0,
               const int y0,
               const int radius)
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
