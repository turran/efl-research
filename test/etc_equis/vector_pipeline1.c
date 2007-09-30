#include <stdio.h>
#include <Equis.h>
#include <Etc.h>

#define DATA "../data"

int main(void)
{
	float x, y;
	int cmd;
	Equis_Component *src;
	Equis_Component *t;
	Equis_Component_Reader *r;
	Etc_Rasterizer *rs;
	Etc_Scanline *sl;
	//Etc_Renderer *rd;
	//Etc_Surface *dst;
	float m[] = {1.5, 3.4, 0.0, 10};

	/* vector pipeline */
	r = equis_reader_new(src);
	src = equis_source_csv_new();
	t = equis_transform_new();
	
	equis_source_csv_file_set(src, DATA"/vector_source1.csv");
	//equis_transform_matrix_set(t, m);
	equis_component_source_set(t, src);
	while ((cmd = equis_reader_vertex_get(r, &x, &y)) != EQUIS_CMD_END)
	{
		//printf("%f %f\n", x, y);
		etc_rasterizer_vertex_add(rs, x, y);
	}
	equis_reader_rewind(r);
	equis_component_delete(src);
	equis_component_delete(t);
	//equis_reader_delete(r);
	
	/* raster pipeline */
	//etc_init();
	rs = etc_rasterizer_new();
	sl = etc_scanline_new();
	etc_rasterizer_generate(rs, sl);
	//rd = etc_renderer_new();
	//dst = etc_surface_new(ETC_SURFACE_ARGB888);
	//etc_renderer_draw(rd, dst, sl);
	etc_scanline_delete(sl);
	//etc_rasterizer_delete(rs);
	//etc_shutdown();
}
