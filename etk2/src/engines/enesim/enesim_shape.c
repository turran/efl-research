/*
 * enesim_shape.c
 *
 *  Created on: 04-feb-2009
 *      Author: jl
 */
#include "Etk2.h"
#include "etk2_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static void _rect(void *surface, void *context, int x, int y, int w, int h)
{
	Enesim_Surface *s = surface;
	Enesim_Context *c = context;

#ifdef ETK2_DEBUG
	printf("[Etk_Engine_Enesim] RENDERING A RECTANGLE at %d %d %d %d to %p\n", x, y, w, h, s);
#endif
	enesim_rect_draw(s, c, x, y, w, h);
}

static void _image(void *surface, void *context, Enesim_Surface *s, Eina_Rectangle *srect)
{
	Enesim_Surface *dst = surface;
	Enesim_Context *c = context;

#ifdef ETK2_DEBUG
	printf("[Etk_Engine_Enesim] RENDERING AN IMAGE\n");
#endif
	enesim_image_draw(dst, c, s, srect);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Etk_Shape_Engine etk_shape_engine_enesim = {
	.rect = _rect,
	.image = _image,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

