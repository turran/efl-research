#include "Emage.h"
#include "emage_private.h"

#include <stdlib.h>
#include <stdarg.h>

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * TODO
 */
EAPI Emage_Surface *
emage_surface_new(Emage_Data_Format f, int w, int h, ...)
{
	Emage_Surface *s;
	va_list va;

	s = calloc(1, sizeof(Emage_Surface));
	s->w = w;
	s->h = h;
	s->format = f;
	s->flags = RGBA_SURFACE_HAS_ALPHA;

	va_start(va, h);
	switch (s->format)
	{
		case EMAGE_DATA_ARGB8888:
		s->data.argb8888.data = va_arg(va, DATA32 *);
		break;

		case EMAGE_DATA_RGB565_A5:
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
 * TODO
 *
 */
EAPI void
emage_surface_size_get(Emage_Surface *s, int *w, int *h)
{
	assert(s);
	if (w) *w = s->w;
	if (h) *h = s->h;
}

/**
 * TODO
 *
 */
EAPI void
emage_surface_data_get(Emage_Surface *s, ...)
{
	DATA32 	**d32;
	DATA16 	**d16;
	DATA8 	**d8;
	va_list va;
	
	assert(s);
	va_start(va, s);
	switch (s->format)
	{
		case EMAGE_DATA_ARGB8888:
		d32 = va_arg(va, DATA32 **);
		*d32 = s->data.argb8888.data;
		break;

		case EMAGE_DATA_RGB565_A5:
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
 * TODO
 *
 */
EAPI Emage_Data_Format
emage_surface_format_get(Emage_Surface *s)
{
	assert(s);
	return s->format;
}

/**
 * TODO
 *
 */
EAPI void 
emage_surface_data_set(Emage_Surface *s, Emage_Data_Format f, ...)
{
	va_list va;
	
	assert(s);
	va_start(va, f);
	switch (s->format)
	{
		case EMAGE_DATA_ARGB8888:
		s->data.argb8888.data = va_arg(va, DATA32 *);
		break;

		case EMAGE_DATA_RGB565_A5:
		s->data.rgb565_a5.data = va_arg(va, DATA16 *);
		s->data.rgb565_a5.alpha = va_arg(va, DATA8 *);
		break;

		default:
		break;
	}
	va_end(va);
}

/**
 * TODO
 *
 */
EAPI void
emage_surface_scale(Emage_Surface *src, Emage_Rectangle sr, Emage_Surface *dst, Emage_Rectangle dr, Emage_Draw_Context *dc)
{
	Emage_Draw_Context_Clip bc;
   	Cutout_Rects *rects;
	int i;

	/* dest rect outside dest surface */
	if (!(RECTS_INTERSECT(dr.x, dr.y, dr.w, dr.h, 0, 0, dst->w, dst->h)))
		return;
	/* source rect outside source surface */
	if (!(RECTS_INTERSECT(sr.x, sr.y, sr.w, sr.h, 0, 0, src->w, src->h)))
		return;

	/* apply the clipping to the destination surface */
	//HANDLE_CUTOUTS(dr, dst)
	/* no cutouts - cut right to the chase */
	if (!dc->cutout.rects)
	{
		/* CALL THE SCALER FUNCTION BASED ON THE CPU */
		emage_scaler_scale(src, &sr, dst, &dr, dc);
		return;
	}
	//FOREACH_RECT_START(dc, drect, dst)
	//scaler function
	//FOREACH_RECT_END(dc)


	/* OLD */
	/* save out clip info */
	bc = dc->clip;
	emage_draw_context_clip_clip(dc, 0, 0, dst->w, dst->h);
	emage_draw_context_clip_clip(dc, dr.x, dr.y, dr.w, dr.h);
	/* our clip is 0 size.. abort */
	if ((dc->clip.r.w <= 0) || (dc->clip.r.h <= 0))
		goto restore_clip;
	rects = emage_draw_context_apply_cutouts(dc);
	for (i = 0; i < rects->active; ++i)
	{
		Cutout_Rect  *r = rects->rects + i;

		emage_draw_context_set_clip(dc, r->x, r->y, r->w, r->h);
		emage_scaler_scale(src, &sr, dst, &dr, dc);
		/* CALL THE SCALER FUNCTION BASED ON THE CPU */
	}
	emage_draw_context_apply_clear_cutouts(rects);
	/* END OLD */

	/* restore clip info */
restore_clip:
	dc->clip = bc;
}
