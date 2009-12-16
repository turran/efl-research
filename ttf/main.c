#include "ttf.h"

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

typedef enum ttf_outline_flags
{
	CURVE    = 1 << 0,
	X_SHORT  = 1 << 1,
	Y_SHORT  = 1 << 2,
	REPEAT   = 1 << 3,
	PX_SHORT = 1 << 4,
	PY_SHORT = 1 << 5,
} ttf_outline_flags;

typedef struct ttf_glyph
{
	int16 ncontours;
	fword xmin;
	fword ymin;
	fword xmax;
	fword ymax;
} ttf_glyph;

int main(int argc, char **argv)
{
	Font *f;
	Glyph g;
	int index;
	int ch = 0x6e;

	ch = 0x6a;
	f = ttf_fopen(argv[1]);
	index = ttf_glyph_index_get(f, ch);
	printf("index for char %d is %d\n", ch, index);
	ttf_glyph_info_get(f, index, &g);
	/* TODO write a simple text */
	/* foreach char get the glyph index, load it and render it */
#if 0
	{
		ttf_font_glyph_index_get(Font *f, char ch);
		/* FIXME renderer get()? */
		ttf_font_glyph_render(Font *f, int gid, Enesim_Surface *s);
	}
#endif

	return 0;
}
