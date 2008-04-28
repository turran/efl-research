#include "enesim_common.h"
#include "Enesim.h"
#include "enesim_private.h"

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Rasterizer * enesim_rasterizer_kiia_new(Enesim_Rasterizer_Kiia_Count
		count, Eina_Rectangle boundaries)
{
	Enesim_Rasterizer *r = NULL;
	switch (count)
	{
	case ENESIM_RASTERIZER_KIIA_COUNT_8:
		r = enesim_rasterizer_kiia8_new(boundaries);
		break;
	case ENESIM_RASTERIZER_KIIA_COUNT_16:
		r = enesim_rasterizer_kiia16_new(boundaries);
		break;
	case ENESIM_RASTERIZER_KIIA_COUNT_32:
		break;
	}
	return r;
}
