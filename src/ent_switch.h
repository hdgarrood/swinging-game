#ifndef ENT_SWITCH_H_EDINSWID
#define ENT_SWITCH_H_EDINSWID

#include <chipmunk/chipmunk.h>

typedef struct {
    cpSpace *space;
    cpBody *body;
    cpShape *left_shape;
    cpShape *right_shape;
    cpConstraint *pivot_constraint;
    cpConstraint *rotary_constraint;

    /* the angle of the surface to which the switch is attached.
     * this should be halfway through its allowed range.
     */
    cpFloat ground_angle;

    /* the maximum distance the switch is allowed to rotate either side
     * of its ground_angle.
     */
    cpFloat half_switch_angle;
} ent_switch;

ent_switch *ent_switch_new(cpSpace *space, cpVect pos, cpFloat ground_angle);
void ent_switch_free(ent_switch *);
bool ent_switch_get_state(ent_switch *);

#endif /* end of include guard: ENT_SWITCH_H_EDINSWID */
