#include <SDL/SDL.h>

DebugDraw::DebugDraw(SDL_Surface *surface)
{
    m_surface = surface;
    m_colour = SDL_MapRGBA(surface->format, 0x33, 0x33, 0x33, 0xFF);
}

void DebugDraw::SetPixel(int x, int y)
{
    Uint32 *pixels = (Uint32*)m_surface->pixels;
    pixels[(surface->w * y) + x] = m_colour;
}

void DebugDraw::DrawRect(int x, int y, int w, int h)
{
    SDL_Rect rect = {x, y, w, h};
    SDL_FillRect(m_surface, &rect, m_colour);
}

// lifted from:
// http://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm#Algorithm_with_Integer_Arithmetic
void DebugDraw::DrawLine(int x0, int y0, int x1, int y1)
{
    const bool steep = (fabs(y1 - y0) > fabs(x1 - x0));
    if (steep)
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

    SetPixel(x0, y0);
    int y = y0;

    for (int x = x0 + 1; x != x1; x++)
    {
        if (D > 0)
        {
            y++;
            SetPixel(x, y);
            D += ((2 * dy) - (2 * dx));
        }
        else
        {
            SetPixel(x, y);
            D += 2 * dy;
        }
    }
}
