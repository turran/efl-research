#include "emage_test.h"

int w, h;

static void line_test(Emage_Surface *s, Emage_Draw_Context *dc)
{
	int i;

	for (i = 0; i < w -1; i += 4)
	{
		emage_line_draw(s, dc, i, 0, (w - 1) - i, h - 1);
	}
}

static void rectangle_test(Emage_Surface *s, Emage_Draw_Context *dc)
{
	int i;
	
	for (i = 0; i < w - 1; i ++)
	{
		emage_rectangle_draw(s, dc, i, i, 25, 25);
	}
}

static void polygon_test(Emage_Surface *s, Emage_Draw_Context *dc)
{
	//emage_polygon_draw(Emage_Surface *dst, Emage_Draw_Context *dc, RGBA_Polygon_Point *points);
}

void object_test(Emage_Surface *s, Emage_Draw_Context *dc)
{
	double t1, t2;

	emage_surface_size_get(s, &w, &h);
	t1 = time_get();
	line_test(s, dc);
	t2 = time_get();
	printf("%g\n", t2 - t1);

	t1 = time_get();
	rectangle_test(s, dc);
	t2 = time_get();
	printf("%g\n", t2 - t1);
	
	
	t1 = time_get();
	polygon_test(s, dc);
	t2 = time_get();
	printf("%g\n", t2 - t1);
}
