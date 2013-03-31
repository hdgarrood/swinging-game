#include <SDL/SDL.h>
#include <chipmunk/chipmunk.h>

#ifndef UTILS_H
#define UTILS_H

double array_average(int array[], size_t size);
cpFloat angle_between(cpVect a, cpVect b);
SDL_Colour colour(int r, int g, int b);

#endif
