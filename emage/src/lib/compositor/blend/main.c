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
	COMPOSITOR_OVERRIDE_ALL(c, blend, argb8888, c)
	c = &Emage_Compositors[EMAGE_DATA_RGB565_A5][EMAGE_RENDER_BLEND];
	COMPOSITOR_OVERRIDE_ALL(c, blend, rgb565_a5, c)
}
