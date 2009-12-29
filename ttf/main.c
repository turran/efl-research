#include "ttf.h"
#include "Enesim.h"

void cb(Glyph *g, ttf_point *p, void *data)
{
	Enesim_Renderer *r = data;
	int i;

	printf("Point received %d\n", p->op);
	switch (p->op)
	{
		case TTF_OP_MOVE_TO:
		enesim_renderer_path_move_to(r, p->x[0], p->y[0]);
		break;

		case TTF_OP_LINE_TO:
		enesim_renderer_path_line_to(r, p->x[1], p->y[1]);
		break;

		case TTF_OP_QUADRATIC_TO:
		enesim_renderer_path_quadratic_to(r, p->x[1], p->y[1],
				p->x[2], p->y[2]);
		break;
	}
}

const char *text = "Hello 6941";

int main(int argc, char **argv)
{
	Font *f;
	Glyph g;
	Enesim_Surface *s;
	Enesim_Renderer *r;
	int i;

	enesim_init();
	emage_init();

	f = ttf_fopen(argv[1]);

	s = enesim_surface_new(ENESIM_FORMAT_ARGB8888, 256, 256);
	r = enesim_renderer_path_new();
	enesim_renderer_shape_draw_mode_set(r, ENESIM_SHAPE_DRAW_MODE_FILL);
	enesim_renderer_shape_fill_color_set(r, 0xff0000ff);
	{
		Enesim_Matrix m;

		enesim_matrix_scale(&m, 8, 8);
		enesim_renderer_transform_set(r, &m);
	}
	for (i = 0; i < strlen(text); i++)
	{
		int index;

		index = ttf_glyph_index_get(f, text[i]);
		printf("index for char %d is %d\n", text[i], index);
		ttf_glyph_info_get(f, index, &g, cb, r);
	}
	enesim_renderer_state_setup(r);
	enesim_renderer_surface_draw(r, s, ENESIM_FILL, ENESIM_COLOR_FULL, NULL);
	enesim_renderer_state_cleanup(r);
 	emage_save("out.png", s, NULL);

	enesim_shutdown();
	emage_shutdown();

	return 0;
}
