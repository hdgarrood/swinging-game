#include <SDL/SDL.h>
#include <math.h>
#include <chipmunk/chipmunk.h>

#include "drawing.h"
#include "macros.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

void
draw_background(SDL_Surface* screen)
{
    struct draw_options opts = {
        .surface = screen,
        .colour = (SDL_Colour){ 0, 0, 0 }
    };

    sdldraw_rect(opts, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

static void
draw_circle_shape(struct draw_options opts, cpShape* shape, cpBody* body)
{
    cpVect centre = cpvadd(cpBodyGetPos(body),
                           cpCircleShapeGetOffset(shape));
    cpFloat radius = cpCircleShapeGetRadius(shape);
    cpFloat angle = cpBodyGetAngle(body);
    cpVect edgePoint = cpvadd(centre,
                              cpv(radius * cos(angle), radius * sin(angle)));

    sdldraw_circle(opts, centre.x, centre.y, radius);
    sdldraw_line(opts, centre.x, centre.y, edgePoint.x, edgePoint.y);
}

static void
draw_segment_shape(struct draw_options opts, cpShape* shape, cpBody* body)
{
    cpVect vect_a = cpSegmentShapeGetA(shape);
    cpVect vect_b = cpSegmentShapeGetB(shape);

    sdldraw_line(opts,
                 vect_a.x,
                 vect_a.y,
                 vect_b.x,
                 vect_b.y);
}

void
draw_shape(cpShape* shape, SDL_Surface* screen)
{
	cpBody* body = shape->body;
    struct draw_options opts = {
        .surface = screen,
        .colour = (SDL_Colour){ 200, 200, 200 }
    };

	switch (shape->CP_PRIVATE(klass)->type)
	{
		case CP_CIRCLE_SHAPE:
            draw_circle_shape(opts, shape, body);
            break;
        case CP_SEGMENT_SHAPE:
            draw_segment_shape(opts, shape, body);
            break;
        default:
			debug_putsf("ignoring unrecognised shape type %d",
					shape->CP_PRIVATE(klass)->type);
            break;
    }
}

void
draw_constraint(cpConstraint* constraint, SDL_Surface *screen)
{
    struct draw_options opts = {
        .surface = screen,
        .colour = (SDL_Colour){ 100, 100, 200 }
    };

	cpVect vect_a = cpBodyGetPos(cpConstraintGetA(constraint));
	cpVect vect_b = cpBodyGetPos(cpConstraintGetB(constraint));
	sdldraw_line(opts,
                 vect_a.x,
                 vect_a.y,
                 vect_b.x,
                 vect_b.y);
}

void
debug_draw_space(cpSpace* space, SDL_Surface* screen)
{
	cpSpaceEachShape(space,
		(cpSpaceShapeIteratorFunc)draw_shape,
		screen);
	cpSpaceEachConstraint(space,
		(cpSpaceConstraintIteratorFunc)draw_constraint,
		screen);
}
