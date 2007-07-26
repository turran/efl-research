#include <Emage.h>
#include <emage_private.h>
/*
#include "argb8888_c.c"
#include "argb8888_mmx.c"
#include "rgb565_a5_c.c"
*/

static Emage_Compositor argb888_blend = {


};

static Emage_Compositor rgb565_a5_blend = {


};

void emage_compositor_blend_init(void)
{
	Emage_Compositors[EMAGE_DATA_ARGB8888][EMAGE_RENDER_BLEND] = argb888_blend;
	Emage_Compositors[EMAGE_DATA_RGB565_A5][EMAGE_RENDER_BLEND] = rgb565_a5_blend;
}
