#include <SDL/SDL.h>
#include <chipmunk/chipmunk.h>

#include "Drawing.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

void
FillBackground(SDL_Surface* screen)
{
    DrawOptions background_opts;

    background_opts.surface = screen;
	SDL_Colour colour = { 0, 0, 0 };
    background_opts.colour = colour;

    SDLDraw_Rect(background_opts, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void
DrawShape(cpShape* shape, SDL_Surface* screen)
{
	cpBody* body = shape->body;
    DrawOptions opts;
    opts.surface = screen;
    opts.colour = (SDL_Colour) { 200, 200, 200 };

	switch (shape->CP_PRIVATE(klass)->type)
	{
		case CP_CIRCLE_SHAPE:
		{
			cpVect centre = cpvadd(cpBodyGetPos(body),
                                   cpCircleShapeGetOffset(shape));
			SDLDraw_Circle(opts,
                           centre.x,
                           centre.y,
                           cpCircleShapeGetRadius(shape));
            break;
        }
        default: break;
    }
}

void
DrawSpace(cpSpace* space, SDL_Surface* screen)
{
	cpSpaceEachShape(space, (cpSpaceShapeIteratorFunc)DrawShape, screen);
}
