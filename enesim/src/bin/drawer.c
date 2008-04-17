#include "enesim_generator.h"

static void point_prototype_start(Format *f)
{
	int i;
	
	fprintf(fout, "(Enesim_Surface_Data *d, Enesim_Surface_Data *s, unsigned int color, unsigned char *mask)\n");
	fprintf(fout, "{\n");
	
	for (i = 0; i < f->num_planes; i++)
	{
		Plane *p = &f->planes[i];
		
		fprintf(fout, "\t%s data%d;\n", type_names[p->type], i);
	}
}

static void point_prototype_end(f)
{
	fprintf(fout, "}\n");
}

static void point_functions(Format *f, const char *rop)
{
	Format *sf;
	int i = 0;

	/* color */
	fprintf(fout, "static void %s_pt_color_%s", f->name, rop);
	point_prototype_start(f);
	fprintf(fout, "\t%s_from_argb(");
	for (i = 0; i < f->num_planes; i++)
	{
		Plane *p = &f->planes[i];
		
		fprintf(fout, "&data%d, ", i);
	}
	fprintf(fout, "color);\n");
	fprintf(fout, "\t%s_%s(", f->name, rop);
	for (i = 0; i < f->num_planes; i++)
	{
		Plane *p = &f->planes[i];
			
		fprintf(fout, "d->%s.plane%d, ", f->name, i);
	}
	for (i = 0; i < f->num_planes; i++)
	{
		Plane *p = &f->planes[i];
		
		if (i == f->num_planes - 1)
			fprintf(fout, "&data%d);\n", i);
		else
			fprintf(fout, "&data%d, ", i);
	}
	point_prototype_end(f);
	/* pixel */
	sf = formats[0];
	while (sf)
	{
		fprintf(fout, "static void %s_pt_pixel_%s_%s", f->name, rop, sf->name);
		point_prototype_start(f);
		/* TODO convert the src pixel to the dst pixel format */
		/* TODO rop there */
		point_prototype_end(f);
		sf = formats[++i];
	}
	/* pixel_color */
	/* mask_color */
	/* pixel_mask */
}

static void span_prototype_start(Format *f)
{
	fprintf(fout, "(Enesim_Surface_Data *d, unsigned int len, Enesim_Surface_Data *s, unsigned int color, unsigned char *mask)\n");
	fprintf(fout, "{\n");
}

static void span_prototype_end(Format *f)
{
	fprintf(fout, "}\n");
}

static void span_functions(Format *f, const char *rop)
{
	int i = 0;
	Format *sf;
	
	/* color */
	fprintf(fout, "static void %s_sp_color_%s", f->name, rop);
	span_prototype_start(f);
	fprintf(fout, "\tEnesim_Surface_Data dtmp, end;\n\n");
	fprintf(fout, "\t%s_data_copy(d, &dtmp);\n", f->name);
	fprintf(fout, "\t%s_data_offset(d, &end, len);\n", f->name);
	fprintf(fout, "\twhile (dtmp.plane0 < end.plane0)\n");
	fprintf(fout, "\t{\n");
	/* TODO get the alpha */
	/* TODO rop */
	fprintf(fout, "\t\t%s_pt_color_%s(&dtmp, NULL, color, NULL);\n", f->name, rop);
	fprintf(fout, "\t\t%s_increment(&dtmp, 1);\n", f->name);
	fprintf(fout, "\t}\n");
	span_prototype_end(f);
	/* pixel */			
	sf = formats[0];
	while (sf)
	{
		fprintf(fout, "static void %s_sp_pixel_%s_%s", f->name, rop, sf->name);
		span_prototype_start(f);
		fprintf(fout, "\tEnesim_Surface_Data stmp, dtmp, end;\n\n");
		fprintf(fout, "\t%s_data_copy(d, &dtmp);\n", f->name);
		fprintf(fout, "\t%s_data_copy(s, &stmp);\n", sf->name);
		fprintf(fout, "\t%s_data_offset(d, &end, len);\n", f->name);
		fprintf(fout, "\twhile (dtmp.plane0 < end.plane0)\n");
		fprintf(fout, "\t{\n");
		fprintf(fout, "\t\t%s_pt_pixel_%s_%s(&dtmp, &stmp, 0, NULL);\n", f->name, rop, sf->name);
		fprintf(fout, "\t\t%s_increment(&stmp, 1);\n", sf->name);
		fprintf(fout, "\t\t%s_increment(&dtmp, 1);\n", f->name);
		fprintf(fout, "\t}\n");
		span_prototype_end(f);
		sf = formats[++i];
	}
	/* pixel_color */
	sf = formats[0];
	while (sf)
	{
		fprintf(fout, "static void %s_sp_pixel_color_%s_%s", f->name, rop, sf->name);
		span_prototype_start(f);
		fprintf(fout, "\tEnesim_Surface_Data stmp, dtmp, end;\n\n");
		fprintf(fout, "\t%s_data_copy(d, &dtmp);\n", f->name);
		fprintf(fout, "\t%s_data_copy(s, &stmp);\n", sf->name);
		fprintf(fout, "\t%s_data_offset(d, &end, len);\n", f->name);
		fprintf(fout, "\twhile (dtmp.plane0 < end.plane0)\n");
		fprintf(fout, "\t{\n");
		fprintf(fout, "\t\t%s_pt_pixel_color_%s_%s(&dtmp, &stmp, color, NULL);\n", f->name, rop, sf->name);
		fprintf(fout, "\t\t%s_increment(&stmp, 1);\n", sf->name);
		fprintf(fout, "\t\t%s_increment(&dtmp, 1);\n", f->name);
		fprintf(fout, "\t}\n");
		span_prototype_end(f);
		sf = formats[++i];
	}
	/* mask_color */
	/* pixel_mask */

}

static void drawer_definition(Format *f)
{
	int rop;
	
	fprintf(fout, "Enesim_Drawer %s_drawer = {\n", f->name);
	for (rop = 0; rop < ROPS; rop++)
	{
		fprintf(fout, "\t.sp_color[%s][COLOR_TRANSPARENT] = %s_sp_%s_color_transparent,\n", rop_names[rop], f->name);
		fprintf(fout, "\t.sp_color[%s][COLOR_OPAQUE] = %s_sp_%s_color_opaque,\n", rop_names[rop], f->name);			
	}
	/*
	 * Enesim_Drawer_Span sp_color[COLOR_TYPES];
	 * Enesim_Drawer_Span sp_mask_color[COLOR_TYPES];
	 * Enesim_Drawer_Span sp_pixel[ENESIM_SURFACE_FORMATS];
	 * Enesim_Drawer_Span sp_pixel_color[ENESIM_SURFACE_FORMATS][COLOR_TYPES];	
	 * Enesim_Drawer_Span sp_pixel_mask[ENESIM_SURFACE_FORMATS];
	 * Enesim_Drawer_Point pt_color[COLOR_TYPES];
	 * Enesim_Drawer_Point pt_mask_color[COLOR_TYPES];
	 * Enesim_Drawer_Point pt_pixel[ENESIM_SURFACE_FORMATS];
	 * Enesim_Drawer_Point pt_pixel_color[ENESIM_SURFACE_FORMATS][COLOR_TYPES];
	 * Enesim_Drawer_Point pt_pixel_mask[ENESIM_SURFACE_FORMATS];
	 */
	fprintf(fout, "};\n");
}

void drawer_functions(Format *df)
{
	int rop;
		
	/* print the header */
	fprintf(fout, "#include \"enesim_common.h\"\n");
	fprintf(fout, "#include \"Enesim.h\"\n");
	fprintf(fout, "#include \"enesim_private.h\"\n\n");
		
	for (rop = 0; rop < ROPS; rop++)
	{
		point_functions(df, rop_names[rop]);		
		span_functions(df, rop_names[rop]);
	}
	drawer_definition(df);
}
