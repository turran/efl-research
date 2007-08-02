#include "Emage.h"
#include "emage_private.h"

#include "main.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/

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
#if 0
	c->sl_pixel = argb8888_c_sl_pixel;
	c->sl_color = argb8888_c_sl_color;
	c->sl_pixel_color = argb8888_c_sl_pixel_color;
	c->sl_mask_color = argb8888_c_sl_mask_color;
	c->sl_pixel_mask = argb8888_c_sl_pixel_mask;
	c->pt_pixel = argb8888_c_pt_pixel;
	c->pt_color = argb8888_c_pt_color;
	c->pt_pixel_color = argb8888_c_pt_pixel_color;
	c->pt_mask_color = argb8888_c_pt_mask_color;
	c->sl_pixel_mask = argb8888_c_sl_pixel_mask;
#endif	
	//Emage_Compositors[EMAGE_DATA_RGB565_A5][EMAGE_RENDER_BLEND] = rgb565_a5_blend;
}
