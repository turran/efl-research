#include "enesim_test.h"

Enesim_Surface * surface_new(int w, int h, Enesim_Surface_Format fmt)
{
	Enesim_Surface *s = NULL;
	Enesim_Surface_Data sdata;
	
	switch(fmt)
	{
		case ENESIM_SURFACE_ARGB8888_PRE:
		sdata.argb8888_pre.plane0 = calloc(1, sizeof(unsigned int) * w * h);
		s = enesim_surface_new(fmt, w, h, ENESIM_SURFACE_ALPHA, &sdata);
		break;

		case ENESIM_SURFACE_ARGB8888:
		sdata.argb8888_pre.plane0 = calloc(1, sizeof(unsigned int) * w * h);
		s = enesim_surface_new(fmt, w, h, ENESIM_SURFACE_ALPHA, &sdata);
		break;
		
		default:
		break;
	}
	return s;
}

void surface_free(Enesim_Surface *s)
{
	Enesim_Surface_Format fmt;
	Enesim_Surface_Data sdata;
	
	fmt = enesim_surface_format_get(s);
	enesim_surface_data_get(s, &sdata);
	
	switch(fmt)
	{
		case ENESIM_SURFACE_ARGB8888_PRE:
		free(sdata.argb8888_pre.plane0);
		break;
		
		default:
		break;
	}
	free(s);
}

void surface_blt(Enesim_Surface *s, SDL_Surface *sdl)
{
	SDL_Surface *tmp;
	Enesim_Surface_Data sdata;
	Enesim_Surface_Format fmt;
	void *data;
	unsigned int amask, rmask, gmask, bmask, pitch, bpp;  
	int w, h;
	
	enesim_surface_size_get(s, &w, &h);
	enesim_surface_data_get(s, &sdata);
	fmt = enesim_surface_format_get(s);
	switch (fmt)
	{
		case ENESIM_SURFACE_ARGB8888_PRE:
		amask = 0xff000000;
		rmask = 0x00ff0000;
		gmask = 0x0000ff00;
		bmask = 0x000000ff;
		pitch = 4 * w;
		bpp = 32;
		data = sdata.argb8888_pre.plane0;
		break;
		
		case ENESIM_SURFACE_ARGB8888:
		amask = 0xff000000;
		rmask = 0x00ff0000;
		gmask = 0x0000ff00;
		bmask = 0x000000ff;
		pitch = 4 * w;
		bpp = 32;
		data = sdata.argb8888.plane0;
		break;
		
		default:
		return;
	}
	/* TODO for now only argb8888_pre */
	tmp = SDL_CreateRGBSurfaceFrom(data, w, h, bpp, pitch, rmask, gmask, bmask, amask);
	SDL_BlitSurface(tmp, NULL, sdl, NULL);
	SDL_FreeSurface(sdl);
}
