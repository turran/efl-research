#include "emage_test.h"

double time_get(void)
{
	struct timeval      timev;

	gettimeofday(&timev, NULL);
	return (double)timev.tv_sec + (((double)timev.tv_usec) / 1000000);
}

