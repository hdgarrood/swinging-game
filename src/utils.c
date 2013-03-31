#include <stddef.h>
#include <chipmunk/chipmunk.h>

double
array_average(int array[], size_t size)
{
    int sum = 0;

    for (int i = 0; i < size; i++)
        sum += array[i];

    return ((double)sum / size);
}

cpFloat
angle_between(cpVect a, cpVect b)
{
    return cpvtoangle(cpvsub(b, a));
}
