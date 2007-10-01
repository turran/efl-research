#include <stdio.h>
#include <Enginy.h>
#include <Enesim.h>

#define DATA "../data"

int main(void)
{
	float x, y;
	int cmd;
	Enginy_Component *src;
	Enginy_Component *t;
	Enginy_Component_Reader *r;
	
	Enesim_Rasterizer *rs;
	Enesim_Scanline *sl;
	//Enesim_Renderer *rd;
	//Enesim_Surface *dst;
	float m[] = {1.5, 3.4, 0.0, 10};

	/* vector pipeline */
	src = enginy_source_csv_new();
	t = enginy_transform_new();
	r = enginy_reader_new(src);
	
	enginy_source_csv_file_set(src, DATA"/vector_source1.csv");
	//enginy_transform_matrix_set(t, m);
	enginy_component_source_set(t, src);
	rs = enesim_rasterizer_new();
	while ((cmd = enginy_reader_vertex_get(r, &x, &y)) != EQUIS_CMD_END)
	{
		//printf("%f %f\n", x, y);
		enesim_rasterizer_vertex_add(rs, x, y);
	}
	//enginy_reader_rewind(r);
	enginy_component_delete(src);
	enginy_component_delete(t);
	//enginy_reader_delete(r);
	
	/* raster pipeline */
	sl = enesim_scanline_new();
	enesim_rasterizer_generate(rs, sl);
	rd = enesim_fill_color_new();
	//dst = enesim_surface_new(ETC_SURFACE_ARGB888);
	//enesim_renderer_draw(rd, dst, sl);
	enesim_scanline_delete(sl);
	//enesim_rasterizer_delete(rs);
}
