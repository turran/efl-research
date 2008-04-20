#include "enesim_common.h"
#include "Enesim.h"
#include "enesim_private.h"

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Surface *
enesim_surface_new(Enesim_Surface_Format f, int w, int h, Enesim_Surface_Flag flags, Enesim_Surface_Data *sdata)
{
	Enesim_Surface *s;
	
	s = calloc(1, sizeof(Enesim_Surface));
	s->w = w;
	s->h = h;
	s->format = f;
	s->flags = flags;

	s->data = *sdata;
	
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
EAPI void enesim_surface_size_set(Enesim_Surface *s, int w, int h)
{
	assert(s);
	s->w = w;
	s->h = h;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Surface_Format
enesim_surface_format_get(Enesim_Surface *s)
{
	assert(s);
	return s->format;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI int
enesim_surface_flag_get(Enesim_Surface *s)
{
	assert(s);
	return s->flags;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void 
enesim_surface_flag_set(Enesim_Surface *s, Enesim_Surface_Flag flags)
{
	assert(s);
	s->flags = flags;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void
enesim_surface_data_get(Enesim_Surface *s, Enesim_Surface_Data *sdata)
{
	assert(s);
	*sdata = s->data;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void 
enesim_surface_data_set(Enesim_Surface *s, Enesim_Surface_Data *sdata)
{
	assert(s);
	/* TODO check if we already had data */
	s->data = *sdata;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void 
enesim_surface_data_increment(Enesim_Surface_Data *sdata, Enesim_Surface_Format sfmt, unsigned int len)
{
	assert(sdata);
	switch (sfmt)
	{
	case ENESIM_SURFACE_ARGB8888:
		argb8888_data_increment(sdata, len);
		break;
	case ENESIM_SURFACE_ARGB8888_PRE:
		argb8888_pre_data_increment(sdata, len);
		break;
	case ENESIM_SURFACE_RGB565:
		rgb565_data_increment(sdata, len);
		break;
	default:
		break;
	}
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void 
enesim_surface_unpremul(Enesim_Surface *s, ...)
{	
	Enesim_Surface_Data data;
	va_list va;
	
	assert(s);
	/* TODO check if we already had data */
	va_start(va, s);
	switch (s->format) {
	case ENESIM_SURFACE_ARGB8888:
		data.argb8888.plane0 = va_arg(va, DATA32 *);
		break;

	case ENESIM_SURFACE_RGB565:
		data.rgb565.plane0 = va_arg(va, DATA16 *);
		data.rgb565.plane1 = va_arg(va, DATA8 *);
		break;

	default:
		break;
	}
	// TODO _backends[s->format]->unpremul(&s->data, &data, s->w * s->h);
	va_end(va);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void 
enesim_surface_convert(Enesim_Surface *s, Enesim_Surface *d)
{
	assert(s);
	assert(d);
	if (s->format == d->format) return;
	/* TODO call the correct convert function based on the src
	 * and dst format, the src and dst flags, etc
	 */
}

EAPI void 
enesim_surface_delete(Enesim_Surface *s)
{
	assert(s);
	free(s);
}

