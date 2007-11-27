#include <stdio.h>
#include <stdlib.h>
#include <Enesim.h>

#define DATA "../data"

struct Renderer
{
	Enesim_Renderer *r;
	Enesim_Surface *s;
};

void kiia_scanline_callback(void *sl_data, void *data)
{
	Enesim_Scanline_Mask *sl = sl_data;
	struct Renderer *rend = data;
	int i;
	//printf("called %p y = %d x = %d w = %d!!!\n", rend, sl->y, sl->x, sl->w);
	#if 0 
	for (i = 0; i < sl->w; i++)
	{
		printf("%x\n", sl->coverages[i]);
	}
	#endif
	enesim_renderer_draw(rend->r, ENESIM_SCANLINE_MASK, sl, rend->s);
}

static void kiia_rasterizer(Enesim_Component_Reader *r)
{
	float x, y;
	int cmd;
	struct Renderer rend;

	Enesim_Rasterizer *rs;
	Enesim_Scanline *sl;
	Enesim_Renderer *rd;
	DATA32 *d32 = calloc(1, sizeof(DATA32) * 400 * 400);
	Enesim_Surface *dst;
	Enesim_Rectangle rect;

	enesim_rectangle_from_coords(&rect, 0, 0, 400, 400);
	rs = enesim_rasterizer_kiia_new(ENESIM_RASTERIZER_KIIA_COUNT_8, rect);
	while ((cmd = enesim_reader_vertex_get(r, &x, &y)) != ENESIM_CMD_END)
	{
		enesim_rasterizer_vertex_add(rs, x, y);
	}
	sl = enesim_scanline_alias_new();
	rd = enesim_fill_color_new();
	enesim_fill_color_color_set(rd, 0xff0000ff);
	dst = enesim_surface_new(ENESIM_SURFACE_ARGB8888, 400, 400, ENESIM_SURFACE_ALPHA, d32);
	rend.r = rd;
	rend.s = dst;
	enesim_rasterizer_generate(rs, ENESIM_SCANLINE_MASK, kiia_scanline_callback, &rend);
	png_save(dst, "/tmp/enesim_kiia.png", 0);
	enesim_rasterizer_delete(rs);
}


void cpsc_scanline_callback(void *sl_data, void *data)
{
	Enesim_Scanline_Alias *sl = sl_data;
	struct Renderer *rend = data;
	//printf("called %p y = %d x = %d w = %d!!!\n", rend, sl->y, sl->x, sl->w);
	enesim_renderer_draw(rend->r, ENESIM_SCANLINE_ALIAS, sl, rend->s);
}

static void cpsc_rasterizer(Enesim_Component_Reader *r)
{
	float x, y;
	int cmd;
	struct Renderer rend;

	Enesim_Rasterizer *rs;
	Enesim_Scanline *sl;
	Enesim_Renderer *rd;
	DATA32 *d32 = calloc(1, sizeof(DATA32) * 400 * 400);
	Enesim_Surface *dst;
	Enesim_Rectangle rect;

	enesim_rectangle_from_coords(&rect, 0, 0, 400, 400);
	rs = enesim_rasterizer_cpsc_new(rect);
	rd = enesim_fill_color_new();
	enesim_fill_color_color_set(rd, 0xff0000ff);
	dst = enesim_surface_new(ENESIM_SURFACE_ARGB8888, 400, 400, ENESIM_SURFACE_ALPHA, d32);
	while ((cmd = enesim_reader_vertex_get(r, &x, &y)) != ENESIM_CMD_END)
	{
		enesim_rasterizer_vertex_add(rs, x, y);
	}
	rend.r = rd;
	rend.s = dst;
	enesim_rasterizer_generate(rs, ENESIM_SCANLINE_ALIAS, cpsc_scanline_callback, &rend);
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
