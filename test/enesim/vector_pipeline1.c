#include <stdio.h>
#include <Enesim.h>

#define DATA "./data"


static void kiia_rasterizer(Enesim_Reader *r)
{


}

static void cpsc_rasterizer(Enesim_Reader *r)
{
	Enesim_Rasterizer *rs;
	Enesim_Scanline *sl;
	Enesim_Renderer *rd;
	DATA32 *d32 = calloc(1, sizeof(DATA32) * 400 * 400);
	Enesim_Surface *dst;

	rs = enesim_rasterizer_new();
	while ((cmd = enesim_reader_vertex_get(r, &x, &y)) != ENESIM_CMD_END)
	{
		//printf("%f %f\n", x, y);
		enesim_rasterizer_vertex_add(rs, x, y);
	}
	sl = enesim_scanline_alias_new();
	enesim_rasterizer_generate(rs, sl);
	rd = enesim_fill_color_new();
	enesim_fill_color_color_set(rd, 0xff0000ff);
	dst = enesim_surface_new(ENESIM_SURFACE_ARGB8888, 400, 400, ENESIM_SURFACE_ALPHA, d32);
	enesim_renderer_draw(rd, sl, dst);
	//enesim_rasterizer_delete(rs);
	png_save(dst, "/tmp/enesim0.png", 0);
	enesim_scanline_delete(sl);
}

int main(void)
{
	float x, y;
	int cmd;
	Enesim_Component *src;
	Enesim_Component *t;
	Enesim_Component_Reader *r;
	
	float m[] = {0.44, -0.89, 0.89, 0.44};

	/* vector pipeline */
	src = enesim_source_csv_new();
	t = enesim_transform_new();
	r = enesim_reader_new(t);
	
	enesim_source_csv_file_set(src, DATA"/vector_source1.csv");
	enesim_transform_matrix_set(t, m);
	enesim_component_source_set(t, src);
	//enesim_reader_rewind(r);
	
	enesim_component_delete(src);
	enesim_component_delete(t);
	//enesim_reader_delete(r);
	
	cpsc_rasterizer(r);
	kiia_rasterizer(r);
}
