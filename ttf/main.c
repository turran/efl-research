#include "ttf.h"
#include "Enesim.h"

#if 0
typedef struct ttf_header
{
	fixed version;
	fixed revision;
	uint32 chksumadj;
	uint32 magic;
	uint16 flags;
	uint16 units_per_em;
	longdate created;
	longdate modified;
	fword xmin;
	fword ymin;
	fword xmax;
	fword ymax;
	uint16 macstyle;
	uint16 ppem;
	int16 direction_hint;
	int16 index_to_loc;
	int16 glyph_data_format;
} ttf_header;

typedef struct ttf_glyph
{
	int16 ncontours;
	fword xmin;
	fword ymin;
	fword xmax;
	fword ymax;
} ttf_glyph;
#endif

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

		case TTF_OP_CUBIC_TO:
		enesim_renderer_path_cubic_to(r, p->x[1], p->y[1],
				p->x[2], p->y[2], p->x[3], p->y[3]);
		break;
	}
}

int main(int argc, char **argv)
{
	Font *f;
	Glyph g;
	int index;
	int ch = 0x6e;
	Enesim_Surface *s;
	Enesim_Renderer *r;


	enesim_init();
	ch = 0x6a;
	f = ttf_fopen(argv[1]);
	index = ttf_glyph_index_get(f, ch);
	printf("index for char %d is %d\n", ch, index);

	s = enesim_surface_new(ENESIM_FORMAT_ARGB8888, 256, 256);
	r = enesim_renderer_path_new();
	ttf_glyph_info_get(f, index, &g, cb, r);
	enesim_renderer_state_setup(r);
	enesim_renderer_surface_draw(r, s, ENESIM_FILL, ENESIM_COLOR_FULL, NULL);
	enesim_renderer_state_cleanup(r);

	enesim_shutdown();

	return 0;
}
