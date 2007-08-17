#include "emage_test.h"

#define WIDTH 256
#define HEIGHT 512 


/* parameters */

/* local */
Emage_Surface * surface_new(int w, int h, Emage_Data_Format fmt)
{
	Emage_Surface *s = NULL;
	switch(fmt)
	{
		case EMAGE_DATA_ARGB8888:
		{
			DATA32 *d;

			d = calloc(1, sizeof(DATA32) * w * h);
			s = emage_surface_new(fmt, w, h, d);
		}
		break;

		default:
		break;
	}
	return s;
}

void surface_free(Emage_Surface *s)
{
	// TODO surface_format_get
	#if 0
	switch(fmt)
	{
		case EMAGE_DATA_ARGB8888:
		{
		#endif
			DATA32 *d;

			emage_surface_data_get(s, &d);
			free(d);
		#if 0
		}
	}
	#endif
	free(s);
}

/* tests */
void test1(void)
{
	Emage_Draw_Context *dc;
	Emage_Surface *s;

	dc = emage_draw_context_new();
	emage_draw_context_set_anti_alias(dc, 1);
	emage_draw_context_set_render_op(dc, EMAGE_RENDER_BLEND);

	s = surface_new(256, 256, EMAGE_DATA_ARGB8888);
		
	emage_draw_context_set_color(dc, 255, 255, 255, 255);
	emage_rectangle_draw(s, dc, 0, 0, 255, 255);
	emage_draw_context_set_color(dc, 255, 0, 0, 255);
	emage_rectangle_draw(s, dc, 64, 64, 80, 80);
	printf("blending\n");
	emage_draw_context_set_color(dc, 0, 240, 0, 240);
	emage_rectangle_draw(s, dc, 100, 100, 80, 80);
	png_save(s, "/tmp/emage_test1.png", 0);
	
	surface_free(s);
	free(dc);
}

/* performance test */
void ptest1(void)
{

}

#if 0
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
	
	for (i = 0; i < w - 1; i += 25)
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
#if 0 
	t1 = time_get();
	line_test(s, dc);
	t2 = time_get();
	printf("%g\n", t2 - t1);
#endif

	t1 = time_get();
	rectangle_test(s, dc);
	t2 = time_get();
	printf("%g\n", t2 - t1);
	
	t1 = time_get();
	polygon_test(s, dc);
	t2 = time_get();
	printf("%g\n", t2 - t1);
}
#endif

int main(void)
{

	if (!emage_init()) return -1;

	test1();

	emage_shutdown();
	return 0;
}
