#include "random.h"

#include <stdlib.h>

int32_t Random_getInt32Range(int32_t min, int32_t max)
{
	return (int32_t)(((double)rand() /
		((double)RAND_MAX + 1.0)) * (max - min + 1) + min);
}

// Returns a double value in the range 0 - 1.
double Random_getDouble()
{
	return ((double)rand() / (double)RAND_MAX);
}