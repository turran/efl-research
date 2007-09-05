#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define EIX_24P8
#define EIX_OVERFLOW
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
	float fa, fb, fr;
	int i;
	struct timeval ts, te, tr;


	a = eix24p8_min;
	b = eix24p8_max;
	r = eix_add(a, b); 
	printf("%d.%d\n", r >> 8, r & 0xff);

	a = eix_from_int(10245123);
	printf("%d.%d\n", a >> 8, a & 0xff);
	b = eix24p8_max;
	printf("%f\n", eix_to_float(b));

	
	fa = -4.13;
	fb = 1.13;
	a = eix_from_float(fa);
	b = eix_from_float(fb);
	r = eix_mul(a, b);
	printf("%d.%d\n", r >> 8, r & 0xff);

	gettimeofday(&ts, NULL);
	for (i = 0; i < 15; i++)
	{
		a = eix_mul(a, b);
	}
	gettimeofday(&te, NULL);
	printf("RESULT = %f\n", eix_to_float(a));
	time_display(ts, te);
	
	gettimeofday(&ts, NULL);
	for (i = 0; i < 15; i++)
	{
		fa *= fb;
	}
	gettimeofday(&te, NULL);
	printf("RESULT = %f\n", fa);
	time_display(ts, te);
	
	return 0;
}


