#include "Emage.h"
#include "emage_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/

/*
#include "argb8888_c.h"
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

static Emage_Compositor argb888_blend;
static Emage_Compositor rgb565_a5_blend;

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/

void emage_compositor_blend_init(void)
{
	/* set the correct compositor based on the cpu properties */
	if (Emage_CPU_Features & (CPU_FEATURE_MMX | CPU_FEATURE_MMX2 | CPU_FEATURE_SSE))
	{

	}
	else
	{

	}
	//Emage_Compositors[EMAGE_DATA_ARGB8888][EMAGE_RENDER_BLEND] = argb888_blend;
	//Emage_Compositors[EMAGE_DATA_RGB565_A5][EMAGE_RENDER_BLEND] = rgb565_a5_blend;
}
