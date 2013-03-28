#include <SDL/SDL.h>
#include <math.h>
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

static void
DrawCircleShape(DrawOptions opts, cpShape* shape, cpBody* body)
{
    cpVect centre = cpvadd(cpBodyGetPos(body),
                           cpCircleShapeGetOffset(shape));
    cpFloat radius = cpCircleShapeGetRadius(shape);
    cpFloat angle = cpBodyGetAngle(body);
    cpVect edgePoint = cpv(radius * cos(angle), radius * sin(angle));

    SDLDraw_Circle(opts, centre.x, centre.y, radius);
    SDLDraw_Line(opts, centre.x, centre.y, edgePoint.x, edgePoint.y);
}

static void
DrawSegmentShape(DrawOptions opts, cpShape* shape, cpBody* body)
{
    cpVect vect_a = cpSegmentShapeGetA(shape);
    cpVect vect_b = cpSegmentShapeGetB(shape);

    SDLDraw_Line(opts,
                 vect_a.x,
                 vect_a.y,
                 vect_b.x,
                 vect_b.y);
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
            DrawCircleShape(opts, shape, body);
            break;
        case CP_SEGMENT_SHAPE:
            DrawSegmentShape(opts, shape, body);
            break;
        default:
            break;
    }
}

void
DrawSpace(cpSpace* space, SDL_Surface* screen)
{
	cpSpaceEachShape(space, (cpSpaceShapeIteratorFunc)DrawShape, screen);
}
