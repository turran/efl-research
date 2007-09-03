#include <stdlib.h>
#include <sys/time.h>
#include "Eix.h"

void time_display(struct timeval ts, struct timeval te)
{
	struct timeval tr;

	timersub(&te, &ts, &tr);
	printf("TIME %ld us\n", (tr.tv_sec * 1000000) + tr.tv_usec);
}

int main(void)
{
	eix a, b, r;
	float fa, fb;
	int i;
	struct timeval ts, te, tr;

	fa = 25.13;
	fb = 32.10;
	a = eix_from_float(fa);
	b = eix_from_float(fb);

	r = eix_mul(a, b);
	printf("%d.%d\n", r >> 8, r & 0xff);

	gettimeofday(&ts, NULL);
	for (i = 0; i < 10000; i++)
	{
		a = eix_add(a, b);
	}
	gettimeofday(&te, NULL);
	printf("RESULT = %f\n", eix_to_float(a));
	time_display(ts, te);
	
	gettimeofday(&ts, NULL);
	for (i = 0; i < 10000; i++)
	{
		fa += fb;
	}
	gettimeofday(&te, NULL);
	printf("RESULT = %f\n", fa);
	time_display(ts, te);
	
	return 0;
}


