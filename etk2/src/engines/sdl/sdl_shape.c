/*
 * sdl_shape.c
 *
 *  Created on: 05-feb-2009
 *      Author: jl
 */
#include "Etk2.h"
#include "etk2_private.h"
#include "SDL.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static void _rect(void *surface, void *context, int x, int y, int w, int h)
{
	SDL_Rect rect;

#ifdef ETK2_DEBUG
	printf("RENDERING A RECTANGLE at %d %d %d %d\n", x, y, w, h);
#endif
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
	SDL_FillRect(surface, &rect, 0xff * rect.x + 0xffff);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Etk_Shape_Engine etk_shape_engine_sdl = {
	.rect = _rect,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

