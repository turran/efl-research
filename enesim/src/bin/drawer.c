#include "enesim_generator.h"
/* pixel
 * color in ARGB format
 * pixel_color
 * mask_color
 * pixel_mask
 */

static void src_data(Format *f)
{
	if (!strcmp(f->name, "argb8888"))
	{
		printf("unsigned int dplane0 = d.argb8888.data;\n");	
	}
	else if (!strcmp(f->name, "rgb565"))
	{
		printf("\tunsigned short int *dplane0 = d.rgb565.data;\n");
		printf("\tunsigned char *dplane1 = d.rgb565.alpha;\n");
	}
}


static void point_prototype(Format *f)
{
	printf("(Enesim_Surface_Data *d, Enesim_Surface_Data *s, unsigned int color, unsigned char *mask)\n");
	printf("{\n");
}

static void span_prototype(Format *f)
{
	printf("(Enesim_Surface_Data *d, unsigned int len, Enesim_Surface_Data *s, unsigned int color, unsigned char *mask)");
}


static void point_functions(Format *f, const char *rop)
{
	/* color */
	printf("static void %s_%s_pt_color", f->name, rop);
	point_prototype(f);
	printf("}\n");
}

static void span_functions(Format *f, const char *rop)
{
	/* color */
	printf("static void %s_%s_sp_color", f->name, rop);
	span_prototype(f);
	printf("}\n");
	/* pixel */
}

void drawer_functions(void)
{
	Format *f;
	int i = 0;
		
	f = formats[0];
	while (f)
	{
		int rop;
		
		for (rop = 0; rop < ROPS; rop++)
		{
			point_functions(f, rop_names[rop]);		
			span_functions(f, rop_names[rop]);		
		}
		f = formats[++i];
	}
}
