#include <stdlib.h>

#include "Emage.h"
#include "emage_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static inline void
emage_scanline_draw_color(Emage_Scanline *sl, Emage_Surface *dst, Emage_Draw_Context *dc)
{
	Emage_Sl_Func func;
	Emage_Span *span;
	int i;
	int offset;
	int roffset;
	
	func = emage_compositor_sl_color_get(dc, dst);
	span = sl->spans;
	offset = ((sl->y * dst->w)) + sl->x;

	printf("from %d,%d width %d\n", sl->y, sl->x, sl->w);
	for (i = 0; i < sl->num_spans; i++)
	{

		roffset = offset + span->x;
		func(NULL, 0, NULL, 0, dc->col.col, dst, offset, span->w);
		span++;
	}
}

static inline void
emage_scanline_draw_surface(Emage_Scanline *sl, Emage_Surface *dst, Emage_Draw_Context *dc)
{
#if 0
	Emage_Sl_Func func;
	Emage_Span *span;

	Emage_Rectangle *drect = &dc->fill.surface.drect;
	Emage_Rectangle *srect = &dc->fill.surface.srect;

	int i;
	int offset;
	int doffset;
	int sx_offset;
	int sy_offset;

	/* TODO
	 * Split this function in three: _color (mul), _mask, and this 
	 * (only _pixel)
	 */
	
	/* TODO drect includes dst->w,h => always pixel */
	/* TODO srect = drect => always pixel */
	/* TODO if scanline (x+w, y) isnt inside drect just use the color */
	/* TODO if we dont have a bounding box use the color */
	if (!(dc->fill.surface.type & EMAGE_FILL_SURFACE_REPEAT_Y))
	{	
	}

	func = emage_compositor_sl_pixel_get(dc, dc->fill.surface.s, dst);
	span = sl->spans;
	offset = ((sl->y * dst->w)) + sl->x;

	sx_offset = dc->fill.surface.srect.x % dc->fill.surface.drect. 
	printf("from %d,%d width %d\n", sl->y, sl->x, sl->w);
	for (i = 0; i < sl->num_spans; i++)
	{
		roffset = offset + span->x;
		func(dc->fill.surface.s, 10, NULL, 0, 0, dst, offset, span->w);
		span++;
	}
#endif
}

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
#if 0
void emage_scanline_sls_draw(Emage_Scanline *sls, Emage_Surface *dst, Emage_Draw_Context *dc)
{
	while (sls->w != 0)
	{
		sls++;
	}
}

EAPI Emage_Scanline *
emage_scanline_add(Emage_Scanline *sls, int x, int y, int w)
{
	Emage_Scanline *sl;

	sl = malloc(sizeof(Emage_Scanline));
	if (!sl) return sls;
	sl->x = x;
	sl->y = y;
	sl->w = w;
	sls = evas_object_list_append(sls, sl);
	return sls;

}

EAPI Emage_Scanline *
emage_scanline_delete(Emage_Scanline *sls)
{
	if (!sls) return NULL;

	while (sls)
	{
		Emage_Scanline *old;
		old = sls;
		sls = evas_object_list_remove(sls, sls);
		free(old);
	}
	return NULL;
}
#endif

/**
 * TODO
 */
EAPI void
emage_scanline_draw(Emage_Scanline *sl, Emage_Surface *dst, Emage_Draw_Context *dc)
{
	
	/* Check the fill type */
	switch (dc->fill.type)
	{
		case EMAGE_FILL_COLOR:
		emage_scanline_draw_color(sl, dst, dc);
		break;
		
		case EMAGE_FILL_SURFACE:
		emage_scanline_draw_surface(sl, dst, dc);
		break;

		default:
		PRINTF("other fill types are not supported yet\n");
		return;
	}
	
}
