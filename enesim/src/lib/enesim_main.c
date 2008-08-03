#include "enesim_common.h"
#include "Enesim.h"
#include "enesim_private.h"

static int _init = 0;

/**
 * 
 */
EAPI int enesim_init(void)
{
	if (!_init++)
	{
		/* initialize the drawer */
		enesim_drawer_init();
	}
	return _init;
}
/**
 * 
 */
EAPI void enesim_shutdown(void)
{
	
}

/**
 * 
 */
EAPI void enesim_color_get(Enesim_Color *color, uint8_t a, uint8_t r, uint8_t g, uint8_t b)
{
	unsigned int alpha = a + 1;
	
	*color = (a << 24) | (((r * alpha) >> 8) << 16) | (((g * alpha) >> 8) << 8)
		| ((b * alpha) >> 8);
}
