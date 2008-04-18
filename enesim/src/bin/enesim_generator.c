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

const char *pixel_types[PIXELS] = {
	"opaque",
	"transparent"
};

FILE *fout = NULL;

/*============================================================================*
 *                                  argb8888                                  * 
 *============================================================================*/
Format argb8888_pre = {
	.name = "argb8888_pre",
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
	.premul = 1,
};

Format argb8888 = {
	.name = "argb8888",
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
	.premul = 0,
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
	.planes[1] = {
		.colors = {
			{
				.offset = 0,
				.length = 5,
				.name = COLOR_ALPHA,
				.type = TYPE_UINT8,
			},
		},
		.num_colors = 1,
		.type = TYPE_UINT8,
	},
	.num_planes = 2,
	.premul = 1,
};

Format *formats[] = {
	&argb8888,
	&argb8888_pre,
	&rgb565,
	NULL,
};


static void help(void)
{
	printf("enesim_generator OPTION FORMAT FILE\n");
	printf("OPTION: core, drawer\n");
	printf("FORMAT: argb8888, rgb565\n");
}

/* helper function to make uppercase a string */
void strupr(char *dst, const char *src)
{
	while (*src)
	{
		*dst = toupper(*src);
		dst++;
		src++;
	}
	*dst = '\0';
}

int main(int argc, char **argv)
{	
	int i = 0;
	Format *f;
	
	if (argc < 4)
	{
		help();
		return 1;
	}
	
	fout = fopen(argv[3], "w+");
	if (!fout)
	{
		help();
		return 2;
	}
	
	f = formats[0];
	while (f)
	{
		if (!(strcmp(f->name, argv[2])))
			break;
		f = formats[++i];
	}
	if (!f)
	{
		help();
		return 3;
	}
	
	if (!(strcmp(argv[1], "core")))
	{
		core_functions();
	}
	else if (!strcmp(argv[1], "drawer"))
	{
		drawer_functions(f);
	}

	return 0;
}
