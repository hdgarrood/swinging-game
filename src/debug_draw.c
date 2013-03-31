#include <SDL/SDL.h>
#include <math.h>
#include <chipmunk/chipmunk.h>

#include "drawing.h"
#include "utils.h"
#include "macros.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

static void
draw_circle(struct draw_options opts, cpVect centre, cpFloat radius)
{
    sdldraw_circle(opts, centre.x, centre.y, radius);
}

static void
draw_line(struct draw_options opts, cpVect v1, cpVect v2)
{
    sdldraw_line(opts, v1.x, v1.y, v2.x, v2.y);
}

void
draw_background(SDL_Surface* screen)
{
    struct draw_options opts = {
        .surface = screen,
        .colour = colour(0, 0, 0)
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

    draw_circle(opts, centre, radius);
    draw_line(opts, centre, edgePoint);
}

static void
draw_segment_shape(struct draw_options opts, cpShape* shape, cpBody* body)
{
    cpVect vect_a = cpSegmentShapeGetA(shape);
    cpVect vect_b = cpSegmentShapeGetB(shape);

    draw_line(opts, vect_a, vect_b);
}

static void
draw_poly_shape(struct draw_options opts, cpShape *shape, cpBody *body)
{
	int num_verts = cpPolyShapeGetNumVerts(shape);
	cpVect first_vert = cpBodyLocal2World(body, cpPolyShapeGetVert(shape, 0));
	cpVect prev_vert = first_vert;
	cpVect pos = cpBodyGetPos(body);
	
	for (int i = 1; i < num_verts; i++)
	{
		cpVect vert = cpBodyLocal2World(body, cpPolyShapeGetVert(shape, i));
		draw_line(opts, prev_vert, vert);
		prev_vert = vert;
	}

	/* close up the polygon */
	draw_line(opts, prev_vert, first_vert);
}

static void
draw_shape(cpShape* shape, SDL_Surface* screen)
{
	cpBody* body = shape->body;
    struct draw_options opts = {
        .surface = screen,
        .colour = colour(200, 200, 200)
    };

	switch (shape->CP_PRIVATE(klass)->type)
	{
		case CP_CIRCLE_SHAPE:
            draw_circle_shape(opts, shape, body);
            break;
        case CP_SEGMENT_SHAPE:
            draw_segment_shape(opts, shape, body);
            break;
		case CP_POLY_SHAPE:
			draw_poly_shape(opts, shape, body);
			break;
        default:
			debug_putsf("ignoring unrecognised shape type %d",
					shape->CP_PRIVATE(klass)->type);
            break;
    }
}

static void
draw_constraint(cpConstraint* constraint, SDL_Surface *screen)
{
    struct draw_options opts = {
        .surface = screen,
        .colour = colour(100, 100, 200)
    };

	cpVect vect_a = cpBodyGetPos(cpConstraintGetA(constraint));
	cpVect vect_b = cpBodyGetPos(cpConstraintGetB(constraint));
	draw_line(opts, vect_a, vect_b);
}

static void
draw_rotation_vector(cpBody *body, SDL_Surface *screen)
{
    struct draw_options opts = {
        .surface = screen,
        .colour = colour(200, 100, 100)
    };

    cpFloat rotation_vector_length = 30;
    cpVect pos = cpBodyGetPos(body);
    cpVect rotation = cpvmult(cpvforangle(cpBodyGetAngle(body)),
                              rotation_vector_length);
    draw_line(opts, pos, cpvadd(pos, rotation));
}

static void
draw_forces(cpBody *body, SDL_Surface *screen)
{
    struct draw_options opts = {
        .surface = screen,
        .colour = colour(100, 200, 100)
    };

    cpVect pos = cpBodyGetPos(body);
    cpVect force = cpBodyGetForce(body);
    draw_line(opts, pos, cpvadd(pos, force));
}

static void
draw_body(cpBody *body, SDL_Surface *screen)
{
    draw_rotation_vector(body, screen);
    draw_forces(body, screen);
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
    cpSpaceEachBody(space,
        (cpSpaceBodyIteratorFunc)draw_body,
        screen);
}
