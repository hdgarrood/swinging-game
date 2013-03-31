#include <chipmunk/chipmunk.h>

#include "ent_switch.h"
#include "constants.h"
#include "macros.h"

struct ent_switch *make_switch(cpSpace *space, cpVect pos, cpFloat ground_angle)
{
	cpVect left_verts[4];
	left_verts[0] = cpv(0, 10);
	left_verts[1] = cpv(0, -2);
	left_verts[2] = cpv(-15, -10);
	left_verts[3] = cpv(-20, 0);

	cpVect right_verts[4];
	right_verts[0] = cpv(20, 0);
	right_verts[1] = cpv(15, -10);
	right_verts[2] = cpv(0, -2);
	right_verts[3] = cpv(0, 10);

	cpVect all_verts[6];
	all_verts[0] = cpv(-20, 0);
	all_verts[1] = cpv(-15, -10);
	all_verts[2] = cpv(0, -2);
	all_verts[3] = cpv(15, -10);
	all_verts[4] = cpv(20, 0);
	all_verts[5] = cpv(0, 10);

    cpFloat switch_angle = 0.3;

	/* create the switch body */
	cpFloat mass = 10;
	cpFloat moment = cpMomentForPoly(mass, 6, all_verts, cpvzero);

	cpBody *body = cpSpaceAddBody(space, cpBodyNew(mass, moment));
	cpBodySetPos(body, pos);

	debug_puts("about to create left shape");
	cpShape *left_shape = cpSpaceAddShape(space,
			cpPolyShapeNew(body, 4, left_verts, cpvzero));

	cpShapeSetFriction(left_shape, 0.5);
	cpShapeSetLayers(left_shape, L_SWITCH);

	debug_puts("about to create right shape");
	cpShape *right_shape = cpSpaceAddShape(space,
		cpPolyShapeNew(body, 4, right_verts, cpvzero));

	cpShapeSetFriction(right_shape, 0.5);
	cpShapeSetLayers(right_shape, L_SWITCH);

	/* attach it to the space's static body */
	cpConstraint *constraint = cpSpaceAddConstraint(space,
			cpPivotJointNew(space->staticBody, body, pos));

    /* add an angle constraint */
    cpSpaceAddConstraint(space, cpRotaryLimitJointNew(
                space->staticBody,
                body,
                ground_angle,
                ground_angle + switch_angle));

	/* create a struct and return it */
	struct ent_switch *sw = malloc(sizeof(struct ent_switch));

	sw->body = body;
	sw->left_shape = left_shape;
	sw->right_shape = right_shape;
	sw->space = space;

	return sw;
}

void
free_ent_switch(struct ent_switch *ent_switch)
{
	/* TODO */
	free(ent_switch);
}
