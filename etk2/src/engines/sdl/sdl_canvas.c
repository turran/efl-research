/*
 * etk2_canvas_sdl.c
 *
 *  Created on: 03-feb-2009
 *      Author: jl
 */
#include "Etk2.h"
#include "etk2_private.h"
#include "SDL.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#if 0
/

/* render */
Eina_Rectangle sgeom, rscaled;
SDL_Rect srect, drect;
Etk_Canvas *dc;
Etk_Canvas_Private *sprv, *cprv;

sprv = PRIVATE(r);
dc = (Etk_Canvas *)ekeko_renderable_canvas_get(r);
cprv = PRIVATE(dc);

/* blt there */
ekeko_renderable_geometry_get(r, &sgeom);
eina_rectangle_rescale_in(&sgeom, rect, &rscaled);

srect.x = rscaled.x;
srect.y = rscaled.y;
srect.w = rscaled.w;
srect.h = rscaled.h;

drect.x = rect->x;
drect.y = rect->y;
drect.w = rect->w;
drect.h = rect->h;


/* canvas flip */


#endif

void * _create(Eina_Bool root, int w, int h)
{
	if (root)
	{
		printf("[SDL] Setting video mode to %d %d\n", w, h);
		return SDL_SetVideoMode(w, h, 32, SDL_RESIZABLE | SDL_SRCALPHA |
				SDL_DOUBLEBUF);
	}
	else
	{
		printf("[SDL] Creating surface %d %d\n", w, h);
		return SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA,
				w, h, 32,
				0xff << 24, 0xff << 16, 0xff << 8, 0xff);
	}
}

void _blit(void *src, Eina_Rectangle *srect, void *context, void *dst, Eina_Rectangle *drect)
{
	SDL_Rect ssrect, sdrect;

	ssrect.x = srect->x;
	ssrect.y = srect->y;
	ssrect.w = srect->w;
	ssrect.h = srect->h;

	sdrect.x = drect->x;
	sdrect.y = drect->y;
	sdrect.w = drect->w;
	sdrect.h = drect->h;

	printf("[SDL] rendering into %p from %d %d %d %d to %d %d %d %d\n",
			dst, srect->x, srect->y, srect->w, srect->h,
			drect->x, drect->y, drect->w, drect->h);
	SDL_BlitSurface(src, &ssrect, dst, &sdrect);
}

Eina_Bool _flush(void *src, Eina_Rectangle *srect)
{
	printf("[SDL] Flushing surface\n");
	SDL_Flip(src);
	return EINA_TRUE;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Etk_Canvas_Engine etk_canvas_engine_sdl = {
	.create = _create,
	.blit = _blit,
	.flush = _flush,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

