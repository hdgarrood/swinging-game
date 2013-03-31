#include <chipmunk/chipmunk.h>

#ifndef ENT_SWITCH_H
#define ENT_SWITCH_H

typedef struct {
	cpSpace *space;
	cpBody *body;
	cpShape *left_shape;
	cpShape *right_shape;
    cpConstraint *pivot_constraint;
    cpConstraint *rotary_constraint;
} ent_switch;

ent_switch *ent_switch_new(cpSpace *space, cpVect pos, cpFloat ground_angle);
void ent_switch_free(ent_switch *);
cpFloat ent_switch_get_angle(ent_switch *);

#endif
