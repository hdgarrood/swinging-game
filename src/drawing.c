#include <SDL/SDL.h>

#include "drawing.h"

/*
 General-purpose drawing functions. These should only assume use of SDL; no
 physics libraries or anything in here.
 All functions should take a struct draw_options as their first parameter.
 All functions take care of locking the surface for you.
*/

#define sgn(x) ((x<0)?-1:((x>0)?1:0)) /* macro to return the sign of a
                                         number */

/* Given a struct draw_options, return a Uint32 for its colour and surface. */
static Uint32
get_sdl_colour_as_uint32(struct draw_options opts)
{
    return SDL_MapRGB(opts.surface->format,
                      opts.colour.r,
                      opts.colour.g,
                      opts.colour.b);
}

// Set pixel (x,y) on the SDL_Surface opts.surface to opts.colour
static void
set_pixel(struct draw_options opts, int x, int y)
{
    /* ensure we aren't outside the surface */
    if (x >= opts.surface->w ||
            x < 0 ||
            y >= opts.surface->h ||
            y < 0)
        return;

    if (SDL_MUSTLOCK(opts.surface))
        SDL_LockSurface(opts.surface);

    Uint32 *pixels = (Uint32*)opts.surface->pixels;

    int index = (opts.surface->w * y) + x;
    int max = (opts.surface->w * opts.surface->h);

    pixels[index] = get_sdl_colour_as_uint32(opts);

    if (SDL_MUSTLOCK(opts.surface))
        SDL_UnlockSurface(opts.surface);
}

// Draw a rectangle, with the given x, y, width, and height.
void
sdldraw_rect(struct draw_options opts, int x, int y, int w, int h)
{
    SDL_Rect rect = {x, y, w, h};
    SDL_FillRect(opts.surface, &rect, get_sdl_colour_as_uint32(opts));
}

void
sdldraw_line(struct draw_options opts, int x0, int y0, int x1, int y1)
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

    set_pixel(opts, px, py);

    if (dxabs >= dyabs) /* the line is more horizontal than vertical */
    {
        for (int i = 0; i < dxabs; i++)
        {
            y += dyabs;
            if (y >= dxabs)
            {
                y -= dxabs;
                py += sdy;
            }
            px += sdx;
            set_pixel(opts, px, py);
        }
    }
    else /* the line is more vertical than horizontal */
    {
        for (int i = 0; i < dyabs; i++)
        {
            x += dxabs;
            if (x >= dyabs)
            {
                x -= dyabs;
                px += sdx;
            }
            py += sdy;
            set_pixel(opts, px, py);
        }
    }
}

// TODO: filled circles
void
sdldraw_circle(struct draw_options opts, int x0, int y0, int radius)
{
    int f = 1 - radius;
    int ddF_x = 1;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    set_pixel(opts, x0, y0 + radius);
    set_pixel(opts, x0, y0 - radius);
    set_pixel(opts, x0 + radius, y0);
    set_pixel(opts, x0 - radius, y0);

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
        set_pixel(opts, x0 + x, y0 + y);
        set_pixel(opts, x0 - x, y0 + y);
        set_pixel(opts, x0 + x, y0 - y);
        set_pixel(opts, x0 - x, y0 - y);
        set_pixel(opts, x0 + y, y0 + x);
        set_pixel(opts, x0 - y, y0 + x);
        set_pixel(opts, x0 + y, y0 - x);
        set_pixel(opts, x0 - y, y0 - x);
    }
}
