#include "emage_test.h"

#define TEST_START(name, desc) 
#define TEST_END(name, desc) 

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
	Emage_Data_Format fmt;
	fmt = emage_surface_format_get(s);
	switch(fmt)
	{
		case EMAGE_DATA_ARGB8888:
		{
			DATA32 *d;

			emage_surface_data_get(s, &d);
			free(d);
		}
		default:
		break;
	}
	free(s);
}

/* creates a 64x64 surface used for scalers and fills */
Emage_Surface * surface_blocks(void)
{
	Emage_Draw_Context *dc;
	Emage_Surface *s;
	Emage_Rectangle r;
	int i, j;

	dc = emage_draw_context_new();
	emage_draw_context_set_anti_alias(dc, 1);
	emage_draw_context_set_render_op(dc, EMAGE_RENDER_BLEND);
	
	s = surface_new(64, 64, EMAGE_DATA_ARGB8888);
	r.w = 8;
	r.h = 8;
	for (i = 0; i < 63; i += 8)
	{
		for (j = 0; j < 63; j += 8)
		{
			int a;

			r.x = i;
			r.y = j;

			a = i > j ? i : j;
			emage_draw_context_set_color(dc, i * 4, j * 4, 0, a * 4);
			emage_rectangle_draw(&r, s, dc);
		}
	}
	free(dc);
	png_save(s, "/tmp/emage_pattern.png", 0);
	return s;
}

/* tests */

/* Test 1
 * ======
 * Render two transparent boxes with all compositors 
 */
void test1(void)
{
	Emage_Draw_Context *dc;
	Emage_Surface *s;
	Emage_Rectangle r;
	int rop;

	dc = emage_draw_context_new();
	emage_draw_context_set_anti_alias(dc, 1);

	s = surface_new(128, 128, EMAGE_DATA_ARGB8888);

	emage_draw_context_fill_type_set(dc, EMAGE_FILL_COLOR);
	for (rop = EMAGE_RENDER_BLEND; rop < EMAGE_RENDER_OPS; rop++)
	{
		char file[PATH_MAX];
		/* clear the surface */
		emage_draw_context_set_render_op(dc, EMAGE_RENDER_BLEND);
		emage_draw_context_set_color(dc, 255, 255, 255, 255);
	
		emage_draw_context_set_render_op(dc, rop);
		r.x = 0, r.y = 0, r.w = 128, r.h = 128;
		emage_rectangle_draw(&r, s, dc);
		emage_draw_context_set_color(dc, 255, 0, 0, 255);
		r.x = 0, r.y = 0, r.w = 80, r.h = 80;
		emage_rectangle_draw(&r, s, dc);
		emage_draw_context_set_color(dc, 0, 240, 0, 240);
		r.x = 47, r.y = 47, r.w = 80, r.h = 80;
		emage_rectangle_draw(&r, s, dc);

		snprintf(file, PATH_MAX, "/tmp/emage_rop%d.png", rop);
		png_save(s, file, 0);
	}
	surface_free(s);
	free(dc);
}

/* Test 2
 * ======
 * Fill a polygon with an image
 */
void test2(void)
{
	Emage_Draw_Context *dc = NULL;
	Emage_Surface *s = NULL;
	Emage_Surface *pattern = NULL;
	Emage_Rectangle r, r2;
	Emage_Polygon_Point *pts = NULL;

	pattern = surface_blocks();
	s = surface_new(128, 128, EMAGE_DATA_ARGB8888);
	
	dc = emage_draw_context_new();
	emage_draw_context_set_anti_alias(dc, 1);
	emage_draw_context_set_render_op(dc, EMAGE_RENDER_BLEND);

	emage_draw_context_set_color(dc, 255, 255, 255, 255);
	r.x = 0, r.y = 0, r.w = 128, r.h = 128;
	emage_rectangle_draw(&r, s, dc);

#if 0 
	/* drect */
	emage_draw_context_set_color(dc, 0, 0, 80, 80);
	emage_draw_context_fill_type_set(dc, EMAGE_FILL_COLOR);
	EMAGE_RECT_FROM_COORDS(r, 110, 0, 128, 128);
#if 0
	emage_rectangle_draw(&r, s, dc);
#endif

	/* srect */
	emage_draw_context_set_color(dc, 0, 80, 0, 80);
	emage_draw_context_fill_type_set(dc, EMAGE_FILL_COLOR);
	EMAGE_RECT_FROM_COORDS(r2, 0, 0, 32, 32);
	emage_rectangle_draw(&r2, s, dc);

	emage_draw_context_set_color(dc, 255, 0, 255, 255);
	emage_draw_context_fill_type_set(dc, EMAGE_FILL_SURFACE);
	EMAGE_RECT_FROM_COORDS(r2, 32, 32, 32, 32);
	emage_draw_context_fill_surface_set(dc, pattern, &r2, &r);
	pts = emage_polygon_point_add(pts, 92, 1);
	pts = emage_polygon_point_add(pts, 1, 92);
	pts = emage_polygon_point_add(pts, 80, 80);
	pts = emage_polygon_point_add(pts, 100, 100);
	pts = emage_polygon_point_add(pts, 120, 5);
	pts = emage_polygon_point_add(pts, 108, 30);
	emage_polygon_draw(s, dc, pts);
	emage_polygon_points_clear(pts);
#else
	/* drect */
	emage_draw_context_set_color(dc, 0, 0, 80, 80);
	emage_draw_context_fill_type_set(dc, EMAGE_FILL_COLOR);
	EMAGE_RECT_FROM_COORDS(r, 34, 10, 80, 118);
#if 0
	emage_rectangle_draw(&r, s, dc);
#endif
	/* srect */
	emage_draw_context_set_color(dc, 0, 80, 0, 80);
	emage_draw_context_fill_type_set(dc, EMAGE_FILL_COLOR);
	EMAGE_RECT_FROM_COORDS(r2, 54, 10, 32, 32);
	emage_rectangle_draw(&r2, s, dc);

	emage_draw_context_fill_type_set(dc, EMAGE_FILL_SURFACE);
	emage_draw_context_fill_surface_type_set(dc, EMAGE_FILL_SURFACE_REPEAT_Y); //| EMAGE_FILL_SURFACE_REPEAT_X);
	EMAGE_RECT_FROM_COORDS(r2, 32, 32, 32, 32);
	emage_draw_context_fill_surface_set(dc, pattern, &r2, &r);
	EMAGE_RECT_FROM_COORDS(r, 10, 10, 118, 118);
	emage_rectangle_draw(&r, s, dc);
	
#endif
	png_save(s, "/tmp/emage_test2.png", 0);
	
	surface_free(s);
	surface_free(pattern);
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
	test2();

	emage_shutdown();
	return 0;
}
