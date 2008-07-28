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
enesim_surface_new_data_from(Enesim_Surface_Format f, int w, int h, Enesim_Surface_Data *sdata)
{
	Enesim_Surface *s;
	
	s = calloc(1, sizeof(Enesim_Surface));
	s->w = w;
	s->h = h;
	s->format = f;
	s->data = *sdata;
	ENESIM_MAGIC_SET(s, ENESIM_SURFACE_MAGIC);
	
	return s;
}
/**
 * 
 */
EAPI Enesim_Surface *
enesim_surface_new(Enesim_Surface_Format f, int w, int h)
{
	Enesim_Surface *s;
		
	s = calloc(1, sizeof(Enesim_Surface));
	s->w = w;
	s->h = h;
	s->format = f;
	
	ENESIM_MAGIC_SET(s, ENESIM_SURFACE_MAGIC);
	switch (s->format)
	{
		case ENESIM_SURFACE_ARGB8888:
		s->data.argb8888.plane0 = calloc(w * h, sizeof(unsigned int));
		break;
		
#ifdef BUILD_SURFACE_ARGB888_UNPRE
		case ENESIM_SURFACE_ARGB8888_UNPRE:
		s->data.argb8888_unpre.plane0 = calloc(w * h, sizeof(unsigned int));
		break;
#endif

#ifdef BUILD_SURFACE_RGB565_XA5
		case ENESIM_SURFACE_RGB565_XA5:
		s->data.rgb565.plane0 = calloc(w * h, sizeof(unsigned short int));
		s->data.rgb565.plane1 = calloc(w * h, sizeof(unsigned char));
		break;
#endif
		default:
		break;
	}
	return s;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void
enesim_surface_size_get(const Enesim_Surface *s, int *w, int *h)
{
	ENESIM_ASSERT(s, ENESIM_ERROR_HANDLE_INVALID);
	ENESIM_MAGIC_CHECK(s, ENESIM_SURFACE_MAGIC);
	if (w) *w = s->w;
	if (h) *h = s->h;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void enesim_surface_size_set(Enesim_Surface *s, int w, int h)
{
	ENESIM_ASSERT(s, ENESIM_ERROR_HANDLE_INVALID);
	ENESIM_ASSERT(w >= 0, ENESIM_ERROR_GEOMETRY_INVALID);
	ENESIM_ASSERT(h >= 0, ENESIM_ERROR_GEOMETRY_INVALID);
	ENESIM_MAGIC_CHECK(s, ENESIM_SURFACE_MAGIC);
	s->w = w;
	s->h = h;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Surface_Format
enesim_surface_format_get(const Enesim_Surface *s)
{
	ENESIM_ASSERT(s, ENESIM_ERROR_HANDLE_INVALID);
	ENESIM_MAGIC_CHECK(s, ENESIM_SURFACE_MAGIC);
	return s->format;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void 
enesim_surface_convert(Enesim_Surface *s, Enesim_Surface *d)
{
	ENESIM_ASSERT(s, ENESIM_ERROR_HANDLE_INVALID);
	ENESIM_ASSERT(d, ENESIM_ERROR_HANDLE_INVALID);
	ENESIM_MAGIC_CHECK(s, ENESIM_SURFACE_MAGIC);
	ENESIM_MAGIC_CHECK(d, ENESIM_SURFACE_MAGIC);
	
	if (s->format == d->format) return;
	/* TODO call the correct convert function based on the src
	 * and dst format, the src and dst flags, etc
	 */
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void 
enesim_surface_delete(Enesim_Surface *s)
{
	assert(s);
	free(s);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void
enesim_surface_data_get(const Enesim_Surface *s, Enesim_Surface_Data *sdata)
{
	ENESIM_ASSERT(s, ENESIM_ERROR_HANDLE_INVALID);
	ENESIM_ASSERT(sdata, ENESIM_ERROR_HANDLE_INVALID);
	ENESIM_MAGIC_CHECK(s, ENESIM_SURFACE_MAGIC);
	*sdata = s->data;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void 
enesim_surface_data_set(Enesim_Surface *s, const Enesim_Surface_Data *sdata)
{
	ENESIM_ASSERT(s, ENESIM_ERROR_HANDLE_INVALID);
	ENESIM_ASSERT(sdata, ENESIM_ERROR_HANDLE_INVALID);
	ENESIM_MAGIC_CHECK(s, ENESIM_SURFACE_MAGIC);
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
	ENESIM_ASSERT(sdata, ENESIM_ERROR_HANDLE_INVALID);
	switch (sfmt)
	{
	case ENESIM_SURFACE_ARGB8888:
		argb8888_data_increment(sdata, len);
		break;
#ifdef BUILD_SURFACE_ARGB888_UNPRE
	case ENESIM_SURFACE_ARGB8888_unpre:
		argb8888_unpre_data_increment(sdata, len);
		break;
#endif
#ifdef BUILD_SURFACE_RGB565_XA5
	case ENESIM_SURFACE_RGB565_XA5:
		rgb565_data_increment(sdata, len);
		break;
#endif
	default:
		break;
	}
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI unsigned int
enesim_surface_data_to_argb(Enesim_Surface_Data *sdata, Enesim_Surface_Format sfmt)
{
	unsigned int argb;
	
	ENESIM_ASSERT(sdata, ENESIM_ERROR_HANDLE_INVALID);
	
	switch (sfmt)
	{
	case ENESIM_SURFACE_ARGB8888:
		argb8888_to_argb(&argb, *(sdata->argb8888.plane0));
		break;
#ifdef BUILD_SURFACE_ARGB888_UNPRE
	case ENESIM_SURFACE_ARGB8888_UNPRE:
		argb8888_unpre_to_argb(&argb, *(sdata->argb8888_unpre.plane0));
		break;
#endif
#ifdef BUILD_SURFACE_RGB565_XA5
	case ENESIM_SURFACE_RGB565_XA5:
		rgb565_to_argb(&argb, *(sdata->rgb565.plane0), *(sdata->rgb565.plane1));
		break;
#endif
	default:
		break;
	}
	return argb;
}
