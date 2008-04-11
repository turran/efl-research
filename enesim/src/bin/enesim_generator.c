#include <stdio.h>

#define MAX_PLANES 4

typedef enum
{
	TYPE_UINT8,
	TYPE_UINT16,
	TYPE_UINT32,
	TYPES
} Type;

const char *type_names[TYPES] = {
	"unsigned char",
	"unsigned short int",
	"unsigned int",
};

typedef enum
{
	COLOR_RED,
	COLOR_GREEN,
	COLOR_BLUE,
	COLOR_ALPHA,
	COLORS
} Color_Name;

const char *color_names[COLORS] = {
	"red",
	"green",
	"blue",
	"alpha",
};

typedef struct _Color
{
	Color_Name name;
	unsigned int offset;
	unsigned int length;
	unsigned int type;
} Color;

typedef struct _Plane
{
	Color colors[COLORS];
	unsigned int length;
	unsigned int num_colors;
	unsigned int type;
} Plane;

typedef struct _Format
{
	Plane planes[MAX_PLANES];
	unsigned int num_planes;
	const char *name;
} Format;

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
/* function to pack a pixel from its components */
void plane_pack(const char *name, Plane *p, unsigned int num)
{
	int i;
	
	printf("static inline %s %s_plane%d_pack(", type_names[p->type], name, num);
	for (i = 0; i < p->num_colors; i++)
	{
		Color *c;
		
		c = &p->colors[i];
		if (i == p->num_colors - 1)
		{
			printf("%s %s)\n", type_names[c->type], color_names[c->name]);
		}
		else
		{
			printf("%s %s, ", type_names[c->type], color_names[c->name]);
		}
	}
	printf("{\n");
	printf("\treturn ");
	for (i = 0; i < p->num_colors; i++)
	{
			Color *c;
			
			c = &p->colors[i];
			if (i == p->num_colors - 1)
			{
				printf("(%s << %d);", color_names[c->name], c->offset);
			}
			else
			{
				printf("(%s << %d) | ", color_names[c->name], c->offset);
			}
	}
	printf("\n");
	printf("}\n");
}
/* function to get a color component */
void color_get(const char *name, Color *c)
{
	printf("static inline uint8 %s_%s_get(uint32 c)\n", name, color_names[c->name]);
	printf("{\n");
	printf("\treturn ((c >> %d) & 0x%x);\n", c->offset, (1 << c->length) - 1);
	printf("}\n");	
}
/* get the pointer for each plane */
void pointer_get(Plane *p)
{
	
}
/* core functions for a pixel format */
void core_functions(Format *f)
{
	int i;
	int j;
	
	for (i = 0; i < f->num_planes; i++)
	{
		Plane *p;
					
		p = &f->planes[i];
		for (j = 0; j < p->num_colors; j++)
		{
			Color *c;
			
			c = &p->colors[j];
			color_get(f->name, c);
		}
		plane_pack(f->name, p, i);
	}
}

/* iterators */
void iterator(Plane *p)
{
	
}
/* functions for any raster operation, blend, mul, etc but only for points
 * the format is:
 * sf_df_pt_rop_stype_dtype
 * sf, df: source and destination formats
 * rop: blend, etc
 * stype, dtype: opaque, transparent?
 * pixel, color, mask, etc
 */
void rop_functions(Format *sf, Format *df)
{
	/* point functions */
	/* blend, d = d*(1 - sa) + s */
	printf("static inline void %s_%s_pt_blend\n", sf->name, df->name);
	printf("{\n");
	printf("}\n");
	/* fill, d = s */
	printf("static inline void %s_%s_pt_fill\n", sf->name, df->name);
	printf("{\n");
	printf("}\n");
	/* span functions */
	
}

void converter_functions(Format *sf, Format *df)
{
	
}

void drawer_functions(Format *f)
{
	
}

int main(void)
{
	Format *sf;
	int i = 0;
	/* TODO what about indentation? before writing the file try to call
	 * indent and write the final file like that?
	 */
	sf = formats[0];
	while (sf)
	{
		Format *df;
		int j = 0;
		
		printf("Parsing %s Pixel Format\n", sf->name);
		core_functions(sf);
		
		df = formats[0];
		while (df)
		{
			rop_functions(sf, df);
			df = formats[++j];
		}
		sf = formats[++i];
	}
	return 0;
}
