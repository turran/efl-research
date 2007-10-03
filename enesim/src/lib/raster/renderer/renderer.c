#include "Enesim.h"
#include "enesim_private.h"
#include "renderer.h"

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
EAPI void enesim_renderer_rop_set(Enesim_Renderer *r, int rop)
{
	assert(r);
	r->rop = rop;
}

EAPI void enesim_renderer_delete(Enesim_Renderer *r)
{
#if 0
	assert(r);
	r->funcs->free(r);
	free(r);
#endif
}

EAPI void enesim_renderer_draw(Enesim_Renderer *r, Enesim_Scanline *sl, Enesim_Surface *dst)
{
#if 0
	assert(r);
	assert(sl);
	assert(dst);
	r->funcs->draw(r, sl, dst);
#endif
}
