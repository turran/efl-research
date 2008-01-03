#include <stdio.h>
#include <stdlib.h>
#include <Enesim.h>

#define DATA "../data"
#define WIDTH 500
#define HEIGHT 800


struct Renderer
{
	Enesim_Renderer *r;
	Enesim_Surface *s;
};

void kiia_scanline_callback(void *sl_data, int sl_type, void *data)
{
	Enesim_Scanline_Mask *sl = sl_data;
	struct Renderer *rend = data;
	enesim_renderer_draw(rend->r, sl_type, sl, rend->s);
}

static void kiia_rasterizer(Enesim_Component_Reader *r)
{
	float x, y;
	int cmd;
	struct Renderer rend;
	struct timeval ts, te;

	Enesim_Rasterizer *rs;
	Enesim_Renderer *rd;
	DATA32 *d32 = calloc(1, sizeof(DATA32) * WIDTH * HEIGHT);
	Enesim_Surface *dst;
	Enesim_Rectangle rect;

	enesim_rectangle_from_coords(&rect, 0, 0, WIDTH, HEIGHT);
	rs = enesim_rasterizer_kiia_new(ENESIM_RASTERIZER_KIIA_COUNT_16, rect);
	while ((cmd = enesim_reader_vertex_get(r, &x, &y)) != ENESIM_CMD_END)
	{
		enesim_rasterizer_vertex_add(rs, x, y);
	}
	rd = enesim_fill_color_new();
	enesim_fill_color_color_set(rd, 0xff0000ff);
	dst = enesim_surface_new(ENESIM_SURFACE_ARGB8888, WIDTH, HEIGHT, ENESIM_SURFACE_ALPHA, d32);
	rend.r = rd;
	rend.s = dst;
	gettimeofday(&ts, NULL);
	enesim_rasterizer_generate(rs, kiia_scanline_callback, &rend);
	gettimeofday(&te, NULL);
	printf("KIIA: \n");
	time_display(ts, te);

	png_save(dst, "/tmp/enesim_kiia.png", 0);
	enesim_rasterizer_delete(rs);
}


void cpsc_scanline_callback(void *sl_data, int sl_type, void *data)
{
	Enesim_Scanline_Alias *sl = sl_data;
	struct Renderer *rend = data;
	enesim_renderer_draw(rend->r, sl_type, sl, rend->s);
}

static void cpsc_rasterizer(Enesim_Component_Reader *r)
{
	float x, y;
	int cmd;
	struct Renderer rend;
	struct timeval ts, te;

	Enesim_Rasterizer *rs;
	Enesim_Scanline *sl;
	Enesim_Renderer *rd;
	DATA32 *d32 = calloc(1, sizeof(DATA32) * WIDTH * HEIGHT);
	Enesim_Surface *dst;
	Enesim_Rectangle rect;

	enesim_rectangle_from_coords(&rect, 0, 0, WIDTH, HEIGHT);
	rs = enesim_rasterizer_cpsc_new(rect);
	rd = enesim_fill_color_new();
	enesim_fill_color_color_set(rd, 0xff0000ff);
	dst = enesim_surface_new(ENESIM_SURFACE_ARGB8888, WIDTH, HEIGHT, ENESIM_SURFACE_ALPHA, d32);
	while ((cmd = enesim_reader_vertex_get(r, &x, &y)) != ENESIM_CMD_END)
	{
		enesim_rasterizer_vertex_add(rs, x, y);
	}
	rend.r = rd;
	rend.s = dst;

	gettimeofday(&ts, NULL);
	enesim_rasterizer_generate(rs, cpsc_scanline_callback, &rend);
	gettimeofday(&te, NULL);
	printf("CPSC: \n");
	time_display(ts, te);
	png_save(dst, "/tmp/enesim_cpsc.png", 0);
	enesim_rasterizer_delete(rs);
}

int main(int argc, char **argv)
{

	Enesim_Component *src;
	Enesim_Component *t;
	Enesim_Component_Reader *r;
	
	//float m[] = {0.44, -0.89, 0.89, 0.44};
	float m[] = {0.44, 0.89, 0.89, 0.44};

	/* vector pipeline */
	src = enesim_source_csv_new();
	t = enesim_transform_new();
	r = enesim_reader_new(src);
	
	enesim_source_csv_file_set(src, DATA"/vector_source1.csv");
	enesim_transform_matrix_set(t, m);
	//enesim_component_source_set(t, src);
	
	cpsc_rasterizer(r);
	enesim_reader_rewind(r);
	kiia_rasterizer(r);
	
	enesim_component_delete(src);
	enesim_component_delete(t);
	//enesim_reader_delete(r);
	return 0;
}
