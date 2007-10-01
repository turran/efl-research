#include <stdlib.h>

#include "Enesim.h"
#include "enesim_private.h"
#include "scanline.h"

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Scanline * enesim_scanline_new(void)
{
	Enesim_Scanline *sl;

	sl = calloc(1, sizeof(Enesim_Scanline));
	sl->funcs = &naa;
	sl->data = sl->funcs->create();
	return sl;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void enesim_scanline_delete(Enesim_Scanline *sl)
{
	sl->funcs->free(sl->data);
	free(sl);
}
