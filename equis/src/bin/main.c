#include "Equis.h"
#include "equis_private.h"
#include <sys/time.h>

void time_display(struct timeval ts, struct timeval te)
{
	struct timeval tr;

	timersub(&te, &ts, &tr);
	printf("TIME %ld us\n", (tr.tv_sec * 1000000) + tr.tv_usec);
}

int main(void)
{
	Equis_Path *p1, *p2;
	struct timeval ts, te;
	int i;
	float t[4];
	float x, y;

	p1 = equis_path_new(10000);
	p2 = equis_path_new(10000);

	for (i = 0; i < 10000; i++)
	{
		equis_path_vertex_add(p1, i, i);
	}
	
	gettimeofday(&ts, NULL);
	t[0] = 2.5, t[1] = 0, t[2] = 0, t[3] =  3.0;
	equis_path_transform(p1, p2, t);
	t[0] = 1, t[1] = 3.0, t[2] = 0, t[3] =  1;
	equis_path_transform(p1, p2, t);
	gettimeofday(&te, NULL);
	time_display(ts, te);
	
	equis_path_rewind(p2);
	for (i = 0; i < 10; i++)
	{
		equis_path_vertex_get(p2, &x, &y);
		printf("x = %f y = %f\n", x, y);
	}
	equis_path_delete(p1);
	equis_path_delete(p2);
	return 0;
}
