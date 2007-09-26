#include "emage_test.h"

#define TEST_START(name, desc) 
#define TEST_END(name, desc) 

/* parameters */

//Emage_Surface * surface_new_from_png()
/* local */
Emage_Surface * surface_new(int w, int h, Emage_Surface_Data_Format fmt)
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
	Emage_Surface_Data_Format fmt;
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

#define B_W 192
#define B_H 192
#define B_STEP 8
#define B_MUL 256 / B_W

	dc = emage_draw_context_new();
	emage_draw_context_set_anti_alias(dc, 1);
	emage_draw_context_set_render_op(dc, EMAGE_RENDER_BLEND);
	
	s = surface_new(B_W, B_H, EMAGE_DATA_ARGB8888);
	r.w = 8;
	r.h = 8;
	for (i = 0; i < B_W; i += B_STEP)
	{
		for (j = 0; j < B_H; j += B_STEP)
		{
			int a;

			r.x = i;
			r.y = j;

			a = i > j ? i : j;
			emage_draw_context_set_color(dc, i * B_MUL, 
				j * B_MUL, 0, a * B_MUL);
			emage_rectangle_draw(&r, s, dc);
		}
	}
	free(dc);
	png_save(s, "/tmp/emage_pattern.png", 0);
	return s;
}

void _background_draw(Emage_Surface *s, Emage_Draw_Context *dc)
{
	Emage_Rectangle r;
	int w, h;

	emage_surface_size_get(s, &w, &h);
	/* TODO fill type get */
	emage_draw_context_fill_type_set(dc, EMAGE_FILL_COLOR);
	emage_draw_context_set_color(dc, 255, 255, 255, 255);
	EMAGE_RECT_FROM_COORDS(r, 0, 0, w, h);
	emage_rectangle_draw(&r, s, dc);
	/* TODO restore fill type and color */
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
		_background_draw(s, dc);
		
		emage_draw_context_set_render_op(dc, rop);
		emage_rectangle_draw(&r, s, dc);
		emage_draw_context_set_color(dc, 255, 0, 0, 255);
		EMAGE_RECT_FROM_COORDS(r, 0, 0, 80, 80);
		emage_rectangle_draw(&r, s, dc);
		emage_draw_context_set_color(dc, 0, 240, 0, 240);
		EMAGE_RECT_FROM_COORDS(r, 47, 47, 80, 80);
		emage_rectangle_draw(&r, s, dc);

		snprintf(file, PATH_MAX, "/tmp/emage_rop%d.png", rop);
		png_save(s, file, 0);
	}
	surface_free(s);
	free(dc);
}

/* Test 2
 * ======
 * Fill a polygon with another surface
 */
void test2(void)
{
	int i;
	double start, end;

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

	_background_draw(s, dc);

	start = time_get();
	for (i = 0; i < 1000; i ++)
	{
		/* drect */
		emage_draw_context_set_color(dc, 0, 0, 80, 80);
		emage_draw_context_fill_type_set(dc, EMAGE_FILL_COLOR);
		EMAGE_RECT_FROM_COORDS(r, 50, 0, 50, 128);
		emage_rectangle_draw(&r, s, dc);

		/* srect */
		emage_draw_context_set_color(dc, 0, 80, 0, 80);
		emage_draw_context_fill_type_set(dc, EMAGE_FILL_COLOR);
		EMAGE_RECT_FROM_COORDS(r2, 0, 0, 32, 32);
		emage_rectangle_draw(&r2, s, dc);

		emage_draw_context_set_color(dc, 255, 0, 255, 255);
		//emage_draw_context_fill_type_set(dc, EMAGE_FILL_SURFACE);
		emage_draw_context_fill_surface_type_set(dc, EMAGE_FILL_SURFACE_REPEAT_X | EMAGE_FILL_SURFACE_REPEAT_Y);
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
		pts = NULL;
	}
	end = time_get();
	printf("%g\n", end - start);
	png_save(s, "/tmp/emage_test2.png", 0);
	
	surface_free(s);
	surface_free(pattern);
	free(dc);
}

/* Test 3
 * ======
 * Scale a surface into another surface
 */
void test3(void)
{
	Emage_Surface *dst = NULL;
	Emage_Surface *src = NULL;
	Emage_Draw_Context *dc = NULL;
	Emage_Rectangle sr, dr;

	dc = emage_draw_context_new();
	emage_draw_context_set_anti_alias(dc, 1);
	emage_draw_context_set_render_op(dc, EMAGE_RENDER_BLEND);
	
	dst = surface_new(128, 128, EMAGE_DATA_ARGB8888);
	_background_draw(dst, dc);
	src = surface_blocks();

	EMAGE_RECT_FROM_COORDS(sr, 0, 0, 100, 100);
	EMAGE_RECT_FROM_COORDS(dr, 0, 0, 128, 128);
	emage_draw_context_set_color(dc, 0, 80, 0, 80);
	//emage_rectangle_draw(&dr, dst, dc);

	emage_draw_context_scaler_type_set(dc, EMAGE_SCALER_SAMPLED);
	emage_surface_scale(src, sr, dst, dr, dc);
	
	png_save(src, "/tmp/emage_test3_src.png", 0);
	png_save(dst, "/tmp/emage_test3_dst.png", 0);
	surface_free(src);
	surface_free(dst);
	free(dc);
}

void test4(void)
{
	Emage_Surface *dst = NULL;
	Emage_Draw_Context *dc = NULL;

	dc = emage_draw_context_new();
	emage_draw_context_set_anti_alias(dc, 1);
	emage_draw_context_set_render_op(dc, EMAGE_RENDER_BLEND);
	
	dst = surface_new(320, 320, EMAGE_DATA_ARGB8888);
	png_load(dst, "evas.png");
	png_save(dst, "/tmp/emage_test4_src.png", 0);
	surface_free(dst);
	free(dc);
}

void test5(void)
{
	Emage_Surface *dst = NULL;
	Emage_Draw_Context *dc = NULL;
	Emage_Polygon_Point *pts = NULL;
	int i;
	struct
	{
		int x;
		int y;
	} points[] = {
		{1168, 2500},
		{1181, 2510},
		{1219, 2580},
		{1272, 2700},
		{1328, 2870},
		{1371, 3040},
		{1436, 3320},
		{1431, 3430},
		{1453, 3520},
		{1450, 3570},
		{1445, 3520},
		{1428, 3460},
		{1404, 3390},
		{1379, 3330},
		{1344, 3270},
		{1330, 3230},
		{1329, 3200},
		{1321, 3160},
		{1327, 3160},
		{1311, 3120},
		{1320, 3130},
		{1299, 3080},
		{1310, 3090},
		{1295, 3060},
		{1305, 3060},
		{1309, 3080},
		{1322, 3110},
		{1318, 3110},
		{1338, 3160},
		{1327, 3150},
		{1345, 3190},
		{1340, 3200},
		{1350, 3230},
		{1374, 3270},
		{1397, 3320},
		{1382, 3260},
		{1404, 3320},
		{1381, 3230},
		{1400, 3280},
		{1379, 3200},
		{1396, 3250},
		{1372, 3160},
		{1389, 3200},
		{1370, 3130},
		{1383, 3160},
		{1366, 3100},
		{1353, 3050},
		{1341, 2980},
		{1318, 2880},
		{1292, 2790},
		{1246, 2660},
		{1216, 2590},
		{1185, 2530}
	};
	dc = emage_draw_context_new();
	emage_draw_context_set_anti_alias(dc, 1);
	emage_draw_context_set_render_op(dc, EMAGE_RENDER_BLEND);
	dst = surface_new(4000, 4000, EMAGE_DATA_ARGB8888);
	_background_draw(dst, dc);
	emage_draw_context_set_color(dc, 0, 0, 30, 30);
	for (i = 0; i < 53; i++)
	{
		pts = emage_polygon_point_add(pts, points[i].x, points[i].y);
	}
	emage_polygon_draw(dst, dc, pts);
	emage_polygon_points_clear(pts);
	png_save(dst, "/tmp/emage_test5.png", 0);
	surface_free(dst);
	free(dc);
}

/* performance test */
void ptest1(void)
{
	Emage_Surface *dst = NULL;
	Emage_Surface *src = NULL;
	Emage_Draw_Context *dc = NULL;
	Emage_Rectangle sr, dr;
	struct timeval ts, te;
	int i;

	dc = emage_draw_context_new();
	emage_draw_context_set_anti_alias(dc, 1);
	emage_draw_context_set_render_op(dc, EMAGE_RENDER_BLEND);
	emage_draw_context_scaler_type_set(dc, EMAGE_SCALER_SAMPLED);
	src = surface_blocks();
	
	/* upscaling */
	dst = surface_new(1024, 1024, EMAGE_DATA_ARGB8888);
	_background_draw(dst, dc);
	EMAGE_RECT_FROM_COORDS(sr, 0, 0, 192, 192);
	EMAGE_RECT_FROM_COORDS(dr, 0, 0, 1024, 1024);
	gettimeofday(&ts, NULL);
	for (i = 0; i < 2000; i++)
	{
		emage_surface_scale(src, sr, dst, dr, dc);
		//emage_surface_scale(dst, dr, src, sr, dc);
	}
	gettimeofday(&te, NULL);
	time_display(ts, te);
	png_save(dst, "/tmp/emage_ptest1_dst.png", 0);
	png_save(src, "/tmp/emage_ptest1_src.png", 0);
	surface_free(dst);
	
	surface_free(src);
	free(dc);

}

void ptest2(void)
{
	Emage_Surface *dst = NULL;
	Emage_Draw_Context *dc = NULL;
	struct timeval ts, te;
	int i;

	dc = emage_draw_context_new();
	emage_draw_context_set_anti_alias(dc, 0);
	emage_draw_context_set_render_op(dc, EMAGE_RENDER_BLEND);
	
	dst = surface_new(1024, 1024, EMAGE_DATA_ARGB8888);
	_background_draw(dst, dc);
	emage_draw_context_set_color(dc, 255, 0, 255, 255);
	gettimeofday(&ts, NULL);
	for (i = 0; i < 2000; i++)
	{
		emage_line_draw(dst, dc, 0, 0, 192, 128);
	}
	gettimeofday(&te, NULL);
	time_display(ts, te);
	png_save(dst, "/tmp/emage_ptest2_dst.png", 0);
	surface_free(dst);
	free(dc);
}

void ptest3(void)
{
	Emage_Surface *s = NULL;
	Emage_Draw_Context *dc = NULL;
	Emage_Polygon_Point *pts = NULL;
	struct timeval ts, te;
	int i;

	dc = emage_draw_context_new();
	emage_draw_context_set_anti_alias(dc, 0);
	emage_draw_context_set_render_op(dc, EMAGE_RENDER_BLEND);
	
	s = surface_new(120, 120, EMAGE_DATA_ARGB8888);
	_background_draw(s, dc);
	emage_draw_context_set_color(dc, 0, 0, 30, 30);
	gettimeofday(&ts, NULL);
	for (i = 0; i < 2000; i++)
	{
		pts = emage_polygon_point_add(pts, 92, 1);
		pts = emage_polygon_point_add(pts, 45, 5);
		pts = emage_polygon_point_add(pts, 1, 92);
		pts = emage_polygon_point_add(pts, 80, 80);
		pts = emage_polygon_point_add(pts, 100, 100);
		pts = emage_polygon_point_add(pts, 120, 5);
		pts = emage_polygon_point_add(pts, 110, 20);
		pts = emage_polygon_point_add(pts, 108, 30);
		emage_polygon_draw(s, dc, pts);
		emage_polygon_points_clear(pts);
		pts = NULL;
	}
	gettimeofday(&te, NULL);
	time_display(ts, te);
	png_save(s, "/tmp/emage_ptest3_dst.png", 0);
	surface_free(s);
	free(dc);
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

	//test1();
	//test2();
	//test3();
	//test4();
	test5();
	//ptest1();
	//ptest2();
	//ptest3();

	emage_shutdown();
	return 0;
}
