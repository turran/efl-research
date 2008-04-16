#include "enesim_generator.h"

/* functions to inrement each of the data pointers the format data has */
static void data_increment(Format *f)
{
	int i;

	printf("static inline void %s_data_increment(Enesim_Surface_Data *d, unsigned int len)\n", f->name);
	printf("{\n");
	for (i = 0; i < f->num_planes; i++)
	{
		Plane *p = &f->planes[i];
		printf("\td->%s.plane%d += len;\n", f->name, i);
	}
	printf("}\n");
}

/* function to copy the data pointers */
static void data_copy(Format *f)
{
	int i;
	printf("static inline void %s_data_copy(Enesim_Surface_Data *s, Enesim_Surface_Data *d)\n", f->name);
	printf("{\n");
	for (i = 0; i < f->num_planes; i++)
	{
		Plane *p = &f->planes[i];
		printf("\td->%s.plane%d = s->%s.plane%d;\n", f->name, i, f->name, i);	
	}
	printf("}\n");
}

/* copy and increment at once */
static void data_offset(Format *f)
{
	int i;
	printf("static inline void %s_data_offset(Enesim_Surface_Data *s, Enesim_Surface_Data *d, unsigned int offset)\n", f->name);
	printf("{\n");
	for (i = 0; i < f->num_planes; i++)
	{
		Plane *p = &f->planes[i];
		printf("\td->%s.plane%d = s->%s.plane%d + offset;\n", f->name, i, f->name, i);	
	}
	printf("}\n");
}

/* return the alpha value as an unsigned char */
static void data_alpha_get(Format *f)
{
	int i;
	printf("static inline unsigned char %s_data_alpha_get(Enesim_Surface_Data *d)\n", f->name);
	printf("{\n");
	
	/* find the alpha */
	for (i = 0; i < f->num_planes; i++)
	{
		int j;
		Plane *p = &f->planes[i];
		
		for (j = 0; j < p->num_colors; j++)
		{
			Color *c = &p->colors[j];

			if (c->name == COLOR_ALPHA)
			{
				printf("\treturn (d->%s.plane%d >> %d) & 0x%x;\n", f->name, i, c->offset, (1 << c->length) - 1);
				goto end;
			}
		}
	}
end:
	printf("}\n");
}

/* print the format plane's mask when converting the plane data into a 32bit
 * value, like: rgb565 should be r5r5g6g6b5b5
 */
static void mask(Format *f)
{
	if (!(strcmp(f->name, "rgb565")))
	{	
		printf("#define MASK 0x\n");
	}
}

/* function to blend a source pixel given an alpha value */
static void blend(Format *f)
{
	int i;
	/* for each component check the length and calculate how it can
	 * be split on simd operations
	 */
	printf("static inline void %s_blend(", f->name);
	for (i = 0; i < f->num_planes; i++)
	{
		Plane *p = &f->planes[i];
		printf("%s plane%d, ", type_names[p->type], i);
	}
	printf("unsigned char alpha)\n");
	printf("{\n");
	/* TODO check if its premul data */
	/* for non premul data, we should mul every component */
	printf("}\n");
}

/* function to pack a pixel from its components */
static void plane_pack(const char *name, Plane *p, unsigned int num)
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

/* convert from/to argb to/from destination format */
static void argb_conv(Format *f)
{
	int i;
	
	/* source format to argb data */
	printf("static inline void %s_to_argb(unsigned int *argb, ", f->name);
	for (i = 0; i < f->num_planes; i++)
	{
		Plane *p = &f->planes[i];
		if (i == f->num_planes - 1)
			printf("%s plane%d)\n", type_names[p->type], i);
		else
			printf("%s plane%d, \n", type_names[p->type], i);
	}
	printf("{\n");
	printf("\t*argb = ");
	for (i = 0; i < f->num_planes; i++)
	{
		int j;
		Plane *p = &f->planes[i];
	
		/* TODO if not alpha or with 0xff000000 */
		for (j = 0; j < p->num_colors; j++)
		{
			Color *c = &p->colors[j];
			
			if ((i == f->num_planes - 1) && (j == p->num_colors - 1))
				printf("((plane%d & 0x%x) << %d)\n", i, (((1 << c->length) - 1) << c->offset), argb_offsets[c->name] - (c->offset + c->length));
			else
				printf("((plane%d & 0x%x) << %d) | ", i, (((1 << c->length) - 1) << c->offset), argb_offsets[c->name] - (c->offset + c->length));
		}
	}
	printf("}\n");
	/* source format from argb data */
	printf("static inline void %s_from_argb(unsigned int argb, ", f->name);
	for (i = 0; i < f->num_planes; i++)
	{
		Plane *p = &f->planes[i];
		if (i == f->num_planes - 1)
			printf("%s *plane%d)\n", type_names[p->type], i);
		else
			printf("%s *plane%d, \n", type_names[p->type], i);
	}
	printf("{\n");
	/* TODO check if its premul */
	printf("}\n");
}

/* function to get a color component */
static void color_get(const char *name, Color *c)
{
	printf("static inline uint8 %s_%s_get(uint32 c)\n", name, color_names[c->name]);
	printf("{\n");
	printf("\treturn ((c >> %d) & 0x%x);\n", c->offset, (1 << c->length) - 1);
	printf("}\n");	
}

/* get the pointer for each plane */
static void pointer_get(Plane *p)
{
	
}
/* core functions for a pixel format */
void core_functions(void)
{
	Format *sf;
	int i = 0;
	
	/* single operations */
	sf = formats[0];
	while (sf)
	{
		Format *df;
		int j = 0;

		/* composed operations */
		df = formats[0];
		while (df)
		{
			/* color from to */
			df = formats[++j];
		}
		//plane_pack(sf->name, p, i);
		argb_conv(sf);
		blend(sf);
		/* data functions */
		data_copy(sf);
		data_increment(sf);
		data_offset(sf);
		data_alpha_get(sf);
		sf = formats[++i];
	}
}
