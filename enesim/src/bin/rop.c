#include "enesim_generator.h"
/* rop functions 
 * pixel
 * color
 * pixel_color
 * mask_color
 * pixel_mask
 */ 
static void fill(Format *f)
{
	/* point */
	
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
	/* color */
	printf("static inline void %s_%s_span_blend_color(", sf->name, df->name);
	data_parameters(df);
	printf(", unsigned int c, int len)\n");
	printf("{\n");
	printf("\t%s end = data0 + len\n", type_names[df->planes[0].type]);
	printf("\twhile (data0 < end)\n");
	printf("\t{\n");
	printf("\t\tdata0++\n");
	printf("\t}\n");
	printf("}\n");
#if 0
		e = data0 + len;
		a = 256 - (c >> 24);

		while (d < e)
		{
			*d = c + mul_256(a, *d);
			d++;
		}
#endif	
	/* pixel */
}
