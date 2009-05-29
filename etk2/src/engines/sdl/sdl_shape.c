/*
 * sdl_shape.c
 *
 *  Created on: 05-feb-2009
 *      Author: jl
 */
#include "Eon.h"
#include "eon_private.h"
#include "SDL.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static void _rect(void *surface, void *context, int x, int y, int w, int h)
{
	SDL_Rect rect;
	Enesim_Context *ctx = context;
	uint32_t color;

#ifdef EON_DEBUG
	printf("RENDERING A RECTANGLE at %d %d %d %d\n", x, y, w, h);
#endif
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	//color = enesim_context_color_get(ctx)
	SDL_FillRect(surface, &rect, color);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eon_Shape_Engine eon_shape_engine_sdl = {
	.rect = _rect,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

