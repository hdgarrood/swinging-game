#include <stddef.h>

double
array_average(int array[], size_t size)
{
    int sum = 0;

    for (int i = 0; i < size; i++)
        sum += array[i];

    return ((double)sum / size);
}
