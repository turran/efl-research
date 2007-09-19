#include "Equis.h"
#include "equis_private.h"
#include <sys/time.h>

void time_display(struct timeval ts, struct timeval te)
{
	struct timeval tr;

	timersub(&te, &ts, &tr);
	printf("TIME %ld us\n", (tr.tv_sec * 1000000) + tr.tv_usec);
}

#if 0
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
	//t[0] = 2.5, t[1] = 0, t[2] = 0, t[3] =  3.0;
	//equis_path_transform(p1, p2, t);
	t[0] = 1.1, t[1] = 3.0, t[2] = 0.4, t[3] =  1;
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
#endif

/* Test1. Source -> Reader */
void test1(void)
{
	float x, y;
	int cmd;
	Equis_Component *src;
	Equis_Component_Reader *r;

	src = equis_source_csv_new();
	//equis_source_csv_file_set(src, "/tmp/test.csv");
	r = equis_reader_new(src);
	equis_source_csv_file_set(src, "/tmp/test.csv");
	while ((cmd = equis_reader_vertex_get(r, &x, &y)) != EQUIS_CMD_END)
	{
		printf("%d %f %f\n", cmd, x, y);
	}
	equis_component_delete(src);
}

/* Test2. Source -> Transform -> Reader */
void test2(void)
{
	float x, y;
	int cmd;
	Equis_Component *src;
	Equis_Component *t;
	Equis_Component_Reader *r;
	float m[] = {1.5, 3.4, 0.0, 10};
	int end = 0;
	struct timeval ts, te;
	int i = 0;
	
	src = equis_source_csv_new();
	equis_source_csv_file_set(src, "/tmp/test.csv");
	t = equis_transform_new();
	equis_transform_matrix_set(t, m);
	equis_component_source_set(t, src);
	r = equis_reader_new(t);
read:
	gettimeofday(&ts, NULL);
	while ((cmd = equis_reader_vertex_get(r, &x, &y)) != EQUIS_CMD_END)
	{
		printf("[%d] READING %d %f %f\n", i, cmd, x, y);
		i++;
	}
	gettimeofday(&te, NULL);
	printf("LOOP %d\n", end);
	time_display(ts, te);
	equis_reader_rewind(r);
	if (!end)
	{
		end = 1;
		i = 0;
		goto read;
	}
	equis_component_delete(src);
	equis_component_delete(t);
}

int main(void)
{
	test1();
	printf("==========\n");
	test2();
	return 1;
}
