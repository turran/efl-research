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
#define SINGLE_BUFFER

static void * _create(Eina_Bool root, int w, int h)
{
	if (root)
	{
		printf("[SDL] Setting video mode to %d %d\n", w, h);
#ifdef SINGLE_BUFFER
		return SDL_SetVideoMode(w, h, 32, SDL_RESIZABLE | SDL_SRCALPHA);
#else
		return SDL_SetVideoMode(w, h, 32, SDL_RESIZABLE | SDL_SRCALPHA |
				SDL_DOUBLEBUF);
#endif
	}
	else
	{
		printf("[SDL] Creating surface %d %d\n", w, h);
		return SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA,
				w, h, 32,
				0xff << 24, 0xff << 16, 0xff << 8, 0xff);
	}
}

static void _lock(void *s)
{
	SDL_Surface *src = s;

	SDL_LockSurface(src);
}

static void _unlock(void *s)
{
	SDL_Surface *src = s;

	SDL_UnlockSurface(src);
}

static void _blit(void *src, Eina_Rectangle *srect, void *context, void *dst, Eina_Rectangle *drect)
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

#ifdef ETK2_DEBUG
	printf("[SDL] rendering into %p from %p (%d %d %d %d to %d %d %d %d)\n",
			dst, src, srect->x, srect->y, srect->w, srect->h,
			drect->x, drect->y, drect->w, drect->h);
#endif
	SDL_BlitSurface(src, &ssrect, dst, &sdrect);
}

static Eina_Bool _flush(void *src, Eina_Rectangle *srect)
{
	SDL_Surface *s = src;

#ifdef ETK2_DEBUG
	printf("[SDL] Flushing surface %p\n", s);
#endif
#ifdef SINGLE_BUFFER
	SDL_UpdateRect(s, srect->x, srect->y, srect->w, srect->h);
	return EINA_FALSE;
#else
	SDL_Flip(s);
	return EINA_TRUE;
#endif

}

static void * _enesim_create(Eina_Bool root, int w, int h)
{
	SDL_Surface *s;
	Enesim_Surface *es;
	Enesim_Surface_Data sdata;

	s = _create(root, w, h);
	if (!root)
	{
		/* FIXME SDL is UNPRE */
		sdata.format= ENESIM_SURFACE_ARGB8888;
		sdata.data.argb8888.plane0 = s->pixels;
		es = enesim_surface_new_data_from(w, h, &sdata);
		enesim_surface_private_set(es, s);
	}
	else
	{
		Enesim_Surface *root;

		/* the backbuffer */
		es = enesim_surface_new(ENESIM_SURFACE_ARGB8888, w, h);
		/* FIXME SDL is UNPRE */
		sdata.format= ENESIM_SURFACE_ARGB8888_UNPRE;
		sdata.data.argb8888.plane0 = s->pixels;
		root = enesim_surface_new_data_from(w, h, &sdata);
		enesim_surface_private_set(root, s);
		enesim_surface_private_set(es, root);
	}


	return es;
}


static void _enesim_blit(void *src, Eina_Rectangle *srect, void *context, void *dst, Eina_Rectangle *drect)
{
	Enesim_Surface *s = src;
	Enesim_Surface *d = dst;
	Enesim_Matrix m;

	/* FIXME fix this */
	enesim_context_clip_set(context, drect);
	enesim_image_draw(d, context, s, srect);
}

static Eina_Bool _enesim_flush(void *src, Eina_Rectangle *srect)
{
	Enesim_Surface *es = src;
	Enesim_Surface *root;
	SDL_Surface *s;

	root = enesim_surface_private_get(es);
	s = enesim_surface_private_get(root);
	enesim_surface_convert(es, root, srect);
	return _flush(s, srect);
}

static void _enesim_lock(void *src)
{
	Enesim_Surface *es = src;

	/* FIXME here we can have a SDL_Surface or an Enesim_Surface */
	_lock(enesim_surface_private_get(es));
}

static void _enesim_unlock(void *src)
{
	Enesim_Surface *es = src;

	/* FIXME here we can have a SDL_Surface or an Enesim_Surface */
	_unlock(enesim_surface_private_get(es));
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Etk_Canvas_Engine etk_canvas_engine_sdl = {
	.create = _create,
	.blit = _blit,
	.flush = _flush,
	.lock = _lock,
	.unlock = _unlock,
};

Etk_Canvas_Engine etk_canvas_engine_sdl_enesim = {
	.create = _enesim_create,
	.blit = _enesim_blit,
	.flush = _enesim_flush,
	.lock = _enesim_lock,
	.unlock = _enesim_unlock,
};
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/

