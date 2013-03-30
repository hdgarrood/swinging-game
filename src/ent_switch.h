#include <chipmunk/chipmunk.h>

#ifndef ENT_SWITCH_H
#define ENT_SWITCH_H

struct ent_switch {
	cpSpace *space;
	cpBody *body;
	cpShape *left_shape;
	cpShape *right_shape;
};

struct ent_switch *make_switch(cpSpace *, cpVect pos);
void free_switch(struct ent_switch *);

#endif
