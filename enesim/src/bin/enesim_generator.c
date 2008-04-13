#include "enesim_generator.h"

const char *type_names[TYPES] = {
	"unsigned char",
	"unsigned short int",
	"unsigned int",
};


const char *color_names[COLORS] = {
	"red",
	"green",
	"blue",
	"alpha",
};

/* offset + length */
const int argb_offsets[COLORS] = {
	32,
	24,
	16,
	8
};

const char *rop_names[ROPS] = {
	"blend",
	"fill"
};

/*============================================================================*
 *                                  argb8888                                  * 
 *============================================================================*/
Format argb8888 = {
	.name = "argb888",
	.planes[0] = {
		.colors = {
			{
				.offset = 0,
				.length = 8,
				.name = COLOR_BLUE,
				.type = TYPE_UINT8,
			},
			{
				.offset = 8,
				.length = 8,
				.name = COLOR_GREEN,
				.type = TYPE_UINT8,
			},
			{
				.offset = 16,
				.length = 8,
				.name = COLOR_RED,
				.type = TYPE_UINT8,
			},
			{
				.offset = 24,
				.length = 8,
				.type = TYPE_UINT8,
				.name = COLOR_ALPHA,
			},
		},
		.num_colors = 4,
		.type = TYPE_UINT32,
	},
	.num_planes = 1,
};

/*============================================================================*
 *                                  argb8888                                  * 
 *============================================================================*/
Format rgb565 = {
	.name = "rgb565",
	.planes[0] = {
		.colors = {
			{
				.offset = 0,
				.length = 5,
				.name = COLOR_BLUE,
				.type = TYPE_UINT8,
			},
			{
				.offset = 5,
				.length = 6,
				.name = COLOR_GREEN,
				.type = TYPE_UINT8,
			},
			{
				.offset = 11,
				.length = 5,
				.name = COLOR_RED,
				.type = TYPE_UINT8,
			},
		},
		.num_colors = 3,
		.type = TYPE_UINT16,
	},
	.num_planes = 1,
};

Format *formats[] = {
	&argb8888,
	&rgb565,
	NULL,
};

/*============================================================================*
 *                           generator functions                              * 
 *============================================================================*/


/* iterators */
void iterator(Plane *p)
{
	
	/* iterate two surfaces */
	/* we need source and destination's width, height, and len */
#if 0
	h = height;
	while (h--)
	{
		/* rop a span */
		DATA32 *d = dp;
		DATA32 *s = sp;
		DATA32 *e = d + length;

		
		while (d < e)
		{
			RENDER_OP_CALL
			d++;
			s++;
		}
		dp += dwidth;
		sp += swidth;
	}
#endif
}

/* given a format generate the parameters for a function */
void data_parameters(Format *f)
{
	int i;
	
	for (i = 0; i < f->num_planes; i++)
	{
		Plane *p = &f->planes[i];
		
		if (i == f->num_planes - 1)
			printf("%s data%d", type_names[p->type], i);
		else
			printf("%s data%d, ", type_names[p->type], i);
	}
}

void converter_functions(Format *sf, Format *df)
{
	
}

int main(void)
{
	core_functions();
	drawer_functions();
	return 0;
}
