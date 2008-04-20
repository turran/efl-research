#ifndef _SURFACE_H
#define _SURFACE_H

/**
 * @todo resize code (enesim_surface_resize())
 * @todo clean this file
 */

typedef enum
{
	ENESIM_SURFACE_NO_SCALE,
	ENESIM_SURFACE_SCALE_X,
	ENESIM_SURFACE_SCALE_Y,
	ENESIM_SURFACE_SCALE_ALL,
	ENESIM_SURFACE_SCALES,
} Enesim_Surface_Scale_Direction;

struct _Enesim_Surface
{
	int w;
	int h;
	Enesim_Surface_Format format;
	int flags;
	Enesim_Surface_Data 		data;
};

void enesim_surface_premul(Enesim_Surface *s);

#endif
