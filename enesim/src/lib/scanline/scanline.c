#include <stdlib.h>

#include "Etc.h"
#include "etc_private.h"
#include "scanline.h"

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Etc_Scanline * etc_scanline_new(void)
{
	Etc_Scanline *sl;

	sl = calloc(1, sizeof(Etc_Scanline));
	sl->funcs = &naa;
	sl->data = sl->funcs->create();
	return sl;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void etc_scanline_delete(Etc_Scanline *sl)
{
	sl->funcs->free(sl->data);
	free(sl);
}
