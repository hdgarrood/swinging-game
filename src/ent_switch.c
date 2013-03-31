#include <stdbool.h>
#include <chipmunk/chipmunk.h>
#include "ent_switch.h"
#include "constants.h"
#include "macros.h"
#include "utils.h"

ent_switch
*ent_switch_new(cpSpace *space, cpVect pos, cpFloat ground_angle)
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

    cpFloat half_switch_angle = angle_between(all_verts[1], all_verts[2]);

	/* create the switch body */
	cpFloat mass = 1;
	cpFloat moment = cpMomentForPoly(mass, 6, all_verts, cpvzero);

	cpBody *body = cpSpaceAddBody(space, cpBodyNew(mass, moment));
	cpBodySetPos(body, pos);
    cpBodySetAngle(body, ground_angle);

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
	cpConstraint *pivot_constraint = cpSpaceAddConstraint(space,
			cpPivotJointNew(space->staticBody, body, pos));

    /* add an angle constraint */
    cpConstraint *rotary_constraint = cpSpaceAddConstraint(space,
            cpRotaryLimitJointNew(
                space->staticBody,
                body,
                ground_angle - half_switch_angle,
                ground_angle + half_switch_angle));

    debug_putsf("half_switch_angle is %f", half_switch_angle);
    debug_putsf("setting angles: min: %f, max: %f",
            ground_angle - half_switch_angle, ground_angle + half_switch_angle);

	/* create a struct and return it */
	ent_switch *sw = malloc(sizeof(ent_switch));

	debug_putsf("created an ent_switch at %p", sw);

	sw->body = body;
	sw->left_shape = left_shape;
	sw->right_shape = right_shape;
	sw->space = space;
    sw->pivot_constraint = pivot_constraint;
    sw->rotary_constraint = rotary_constraint;
    sw->ground_angle = ground_angle;

	return sw;
}

void
free_ent_switch(ent_switch *sw)
{
    cpSpaceRemoveConstraint(sw->space, sw->rotary_constraint);
    cpSpaceRemoveConstraint(sw->space, sw->pivot_constraint);
    cpSpaceRemoveShape(sw->space, sw->left_shape);
    cpSpaceRemoveShape(sw->space, sw->right_shape);
    cpSpaceRemoveBody(sw->space, sw->body);

    cpConstraintFree(sw->rotary_constraint);
    cpConstraintFree(sw->pivot_constraint);
    cpShapeFree(sw->left_shape);
    cpShapeFree(sw->right_shape);
    cpBodyFree(sw->body);
	free(sw);
}

bool
ent_switch_get_state(ent_switch *sw)
{
    return (cpBodyGetAngle(sw->body) > sw->ground_angle);
}
