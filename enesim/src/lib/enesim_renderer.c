#include "enesim_common.h"
#include "Enesim.h"
#include "enesim_private.h"
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
Enesim_Renderer * enesim_renderer_new(void)
{
	Enesim_Renderer *r;

	r = calloc(1, sizeof(Enesim_Renderer));
	ENESIM_MAGIC_SET(r, ENESIM_RENDERER_MAGIC);
	return r;
}
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void enesim_renderer_rop_set(Enesim_Renderer *r, int rop)
{
	ENESIM_ASSERT(r, ENESIM_ERROR_HANDLE_INVALID);
	ENESIM_MAGIC_CHECK(r, ENESIM_RENDERER_MAGIC);
	r->rop = rop;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void enesim_renderer_delete(Enesim_Renderer *r)
{
	ENESIM_ASSERT(r, ENESIM_ERROR_HANDLE_INVALID);
	ENESIM_MAGIC_CHECK(r, ENESIM_RENDERER_MAGIC);
	r->funcs->free(r);
	free(r);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool enesim_renderer_draw(Enesim_Renderer *r, int type, void *sl, Enesim_Surface *dst)
{
	ENESIM_ASSERT(r, ENESIM_ERROR_HANDLE_INVALID);
	ENESIM_ASSERT(sl, ENESIM_ERROR_HANDLE_INVALID);
	ENESIM_ASSERT(dst, ENESIM_ERROR_HANDLE_INVALID);
	ENESIM_MAGIC_CHECK(r, ENESIM_RENDERER_MAGIC);
	return r->funcs->draw(r, type, sl, dst);
}
