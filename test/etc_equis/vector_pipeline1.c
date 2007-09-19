#include <Equis.h>
#include <Etc.h>

int main(void)
{
	float x, y;
	int cmd;
	Equis_Component *src;
	Equis_Component *t;
	Equis_Component_Reader *r;
	Etc_Scanline *sl;
	float m[] = {1.5, 3.4, 0.0, 10};
	
	sl = etc_scanline_new();
	src = equis_source_csv_new();
	equis_source_csv_file_set(src, "/tmp/test.csv");
	t = equis_transform_new();
	equis_transform_matrix_set(t, m);
	equis_component_source_set(t, src);
	r = equis_reader_new(t);
read:
	while ((cmd = equis_reader_vertex_get(r, &x, &y)) != EQUIS_CMD_END)
	{
		printf("%f %f\n", x, y);
		etc_scanline_vertex_add(sl, x, y);
	}
	etc_scanline_generate(sl);
	equis_reader_rewind(r);
	equis_component_delete(src);
	equis_component_delete(t);

}
