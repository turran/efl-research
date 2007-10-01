#include "Enesim.h"
#include "enesim_private.h"
#include "surface.h"

#include <stdlib.h>
#include <stdarg.h>

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Surface *
enesim_surface_new(Enesim_Surface_Data_Format f, int w, int h, ...)
{
	Enesim_Surface *s;
	va_list va;

	s = calloc(1, sizeof(Enesim_Surface));
	s->w = w;
	s->h = h;
	s->format = f;
	s->flags = RGBA_SURFACE_HAS_ALPHA;

	va_start(va, h);
	switch (s->format)
	{
		case ENESIM_DATA_ARGB8888:
		s->data.argb8888.data = va_arg(va, DATA32 *);
		break;

		case ENESIM_DATA_RGB565_A5:
		s->data.rgb565_a5.data = va_arg(va, DATA16 *);
		s->data.rgb565_a5.alpha = va_arg(va, DATA8 *);
		break;

		default:
		break;
	}
	va_end(va);
	return s;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void
enesim_surface_size_get(Enesim_Surface *s, int *w, int *h)
{
	assert(s);
	if (w) *w = s->w;
	if (h) *h = s->h;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void
enesim_surface_data_get(Enesim_Surface *s, ...)
{
	DATA32 	**d32;
	DATA16 	**d16;
	DATA8 	**d8;
	va_list va;
	
	assert(s);
	va_start(va, s);
	switch (s->format)
	{
		case ENESIM_DATA_ARGB8888:
		d32 = va_arg(va, DATA32 **);
		*d32 = s->data.argb8888.data;
		break;

		case ENESIM_DATA_RGB565_A5:
		d16 = va_arg(va, DATA16 **);
		*d16 = s->data.rgb565_a5.data;
		d8 = va_arg(va, DATA8 **);
		*d8 = s->data.rgb565_a5.alpha;
		break;

		default:
		break;
	}
	va_end(va);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Surface_Data_Format
enesim_surface_format_get(Enesim_Surface *s)
{
	assert(s);
	return s->format;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void 
enesim_surface_data_set(Enesim_Surface *s, Enesim_Surface_Data_Format f, ...)
{
	va_list va;
	
	assert(s);
	va_start(va, f);
	switch (s->format)
	{
		case ENESIM_DATA_ARGB8888:
		s->data.argb8888.data = va_arg(va, DATA32 *);
		break;

		case ENESIM_DATA_RGB565_A5:
		s->data.rgb565_a5.data = va_arg(va, DATA16 *);
		s->data.rgb565_a5.alpha = va_arg(va, DATA8 *);
		break;

		default:
		break;
	}
	va_end(va);
}
