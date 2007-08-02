#include "Emage.h"
#include "emage_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/

#include "argb8888_c.c"
/*
#include "argb8888_mmx.h"
#include "rgb565_a5_c.h"
*/

/*
CPU_FEATURE_C
CPU_FEATURE_MMX
CPU_FEATURE_MMX2
CPU_FEATURE_SSE
CPU_FEATURE_ALTIVEC
CPU_FEATURE_VIS
CPU_FEATURE_VIS2
*/

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/

void emage_compositor_blend_init(void)
{
	Emage_Compositor *c;

#if 0
	/* set the correct compositor based on the cpu properties */
	if (Emage_CPU_Features & (CPU_FEATURE_MMX | CPU_FEATURE_MMX2 | CPU_FEATURE_SSE))
	{

	}
	else
	{

	}
#endif
	c = &Emage_Compositors[EMAGE_DATA_ARGB8888][EMAGE_RENDER_BLEND];
	c->sl_pixel = blend_c_sl_pixel;
	c->sl_color = blend_c_sl_color;
	c->sl_pixel_color = blend_c_sl_pixel_color;
	c->sl_mask_color = blend_c_sl_mask_color;
	c->sl_pixel_mask = blend_c_sl_pixel_mask;
	c->pt_pixel = blend_c_pt_pixel;
	c->pt_color = blend_c_pt_color;
	c->pt_pixel_color = blend_c_pt_pixel_color;
	c->pt_mask_color = blend_c_pt_mask_color;
	c->sl_pixel_mask = blend_c_sl_pixel_mask;
	
	//Emage_Compositors[EMAGE_DATA_RGB565_A5][EMAGE_RENDER_BLEND] = rgb565_a5_blend;
}
