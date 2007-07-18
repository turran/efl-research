#include "Emage.h"

int w, h;

static void line_test(RGBA_Surface *s, RGBA_Draw_Context *dc)
{
	int i;

	for (i = 0; i < w -1; i += 4)
	{
		emage_line_draw(s, dc, i, 0, (w - 1) - i, h - 1);
	}
}

static void rectangle_test(RGBA_Surface *s, RGBA_Draw_Context *dc)
{
	int i;
	
	for (i = 0; i < w - 1; i += 25)
	{
		emage_rectangle_draw(s, dc, i, i, 25, 25);
	}
}

void object_test(RGBA_Surface *s, RGBA_Draw_Context *dc)
{
	emage_surface_size_get(s, &w, &h);
	line_test(s, dc);
	rectangle_test(s, dc);
}
