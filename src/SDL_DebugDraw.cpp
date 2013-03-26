#include <Box2D/Box2D.h>
#include <SDL/SDL.h>

#include "SDL_DebugDraw.h"
#include "Drawing.h"
#include "Constants.h"

int ScaleForDrawing(float val)
{
    return (int)floor(val * PPM);
}

SDL_DebugDraw::SDL_DebugDraw(SDL_Surface* surface, Uint32 colour)
{
    m_drawOpts.surface = surface;
    m_drawOpts.colour = colour;
}

void SDL_DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
}

void SDL_DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
}

// ignore the passed b2Color, we just use our own m_drawOpts.colour instead
void SDL_DebugDraw::DrawCircle
(const b2Vec2& center, float32 radius, const b2Color& color)
{
    int x = ScaleForDrawing(center.x),
        y = ScaleForDrawing(center.y),
        r = ScaleForDrawing(radius);

    Draw_Circle(m_drawOpts, x, y, r);
}

void SDL_DebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
}
void SDL_DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
}
void SDL_DebugDraw::DrawTransform(const b2Transform& xf)
{
}
void SDL_DebugDraw::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
}
void SDL_DebugDraw::DrawString(int x, int y, const char* string, ...)
{
}
void SDL_DebugDraw::DrawAABB(b2AABB* aabb, const b2Color& color)
{
}
