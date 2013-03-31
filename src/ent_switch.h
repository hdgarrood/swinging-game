#include <chipmunk/chipmunk.h>

#ifndef ENT_SWITCH_H
#define ENT_SWITCH_H

struct ent_switch {
	cpSpace *space;
	cpBody *body;
	cpShape *left_shape;
	cpShape *right_shape;
    cpConstraint *pivot_constraint;
    cpConstraint *rotary_constraint;
};

struct ent_switch *make_switch(cpSpace *, cpVect pos, cpFloat ground_angle);
void free_switch(struct ent_switch *);
cpFloat ent_switch_get_angle(struct ent_switch *);

#endif
