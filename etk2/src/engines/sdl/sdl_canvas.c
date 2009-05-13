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
		//return SDL_SetVideoMode(w, h, 32, SDL_SRCALPHA);
#else
		//return SDL_SetVideoMode(w, h, 32, SDL_RESIZABLE | SDL_SRCALPHA |
		//		SDL_DOUBLEBUF);
		return SDL_SetVideoMode(w, h, 32, SDL_SRCALPHA | SDL_DOUBLEBUF);
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
	Enesim_Surface *es;

	es = enesim_surface_new(ENESIM_FORMAT_ARGB8888, w, h);
	if (root)
	{
		SDL_Surface *s;

		/* the destination surface */
		s = _create(root, w, h);
		enesim_surface_private_set(es, s);
	}
	return es;
}


static void _enesim_blit(void *s, void *context, void *src, Eina_Rectangle *srect)
{
	etk2_enesim_image(s, context, src, srect);
}

static Eina_Bool _enesim_flush(void *src, Eina_Rectangle *srect)
{
	Enesim_Surface *es = src;
	Enesim_Operator op;
	Enesim_Cpu **cpus;
	uint32_t *sdata;
	uint8_t *cdata;

	SDL_Surface *s;

	int h = srect->h;
	int stride;
	int inc;
	int numcpus;
	int soffset;
	int coffset;

#if 0
	printf("Flushing the canvas %d %d %d %d\n", srect->x, srect->y, srect->w, srect->h);
	{
		int w, h;
		enesim_surface_size_get(es, &w, &h);
		printf("%d %d\n", w, h);
	}
#endif
	/* setup the pointers */
	s = enesim_surface_private_get(es);
	stride = enesim_surface_stride_get(es);

	sdata = enesim_surface_data_get(es);
	cdata = s->pixels;

	soffset = (stride * srect->y) + srect->x;
	coffset = (s->pitch * srect->y) + (srect->x * 4);

	cdata += coffset;
	sdata += soffset;
	/* convert */
	cpus = enesim_cpu_get(&numcpus);
	enesim_converter_1d_from_get(&op, cpus[0], ENESIM_FORMAT_ARGB8888, ENESIM_CONVERTER_ARGB8888);

	_lock(s);
	while (h--)
	{
		enesim_operator_converter_1d(&op, sdata, srect->w, cdata);
		sdata += stride;
		cdata += s->pitch;
	}
	_unlock(s);
	return _flush(s, srect);
}

static void _enesim_lock(void *src)
{
	Enesim_Surface *es = src;
	SDL_Surface *s;

	/* FIXME here we can have a SDL_Surface or an Enesim_Surface */
	s = enesim_surface_private_get(es);
	if (s)
		_lock(s);
}

static void _enesim_unlock(void *src)
{
	Enesim_Surface *es = src;
	SDL_Surface *s;

	/* FIXME here we can have a SDL_Surface or an Enesim_Surface */
	s = enesim_surface_private_get(es);
	if (s)
		_unlock(s);
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

