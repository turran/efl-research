#include "esvg_common.h"
#include "ESVG.h"
#include "esvg_private.h"
/* TODO
 * change this interface to:
 * struct _ESVG_Engine
 * {
 * 	void *data;
 * 	ESVG_Engine_Func func;
 * };
 */
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
void * esvg_engine_context_new(ESVG *svg)
{
	return svg->engine->context_new(svg->engine_data);
}

void esvg_engine_context_free(ESVG *svg, void *context)
{
	svg->engine->context_free(svg->engine_data, context);
}

void esvg_engine_rect_draw(ESVG *svg, void *context, ESVG_Rect *r, Eina_Rectangle *rclip)
{
	svg->engine->rect_draw(svg->engine_data, context, r, rclip);
}
