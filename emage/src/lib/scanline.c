#include <stdlib.h>

#include "Emage.h"
#include "emage_private.h"

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

EAPI void
emage_scanline_draw(Emage_Scanline *sl, Emage_Surface *dst, Emage_Draw_Context *dc)
{
	Emage_Sl_Func func;
	Emage_Span *span;
	int i;
	int offset;
	int roffset;
	
	/* TODO abstract this call as it might not be a _color_ */
	func = emage_compositor_sl_color_get(dc, dst);
	
	span = sl->spans;
	offset = ((sl->y * dst->w)) + sl->x;

	for (i = 0; i < sl->num_spans; i++)
	{

		roffset = offset + span->x;
		func(NULL, 0, NULL, 0, dc->col.col, dst, offset, span->w);
		span++;
	}
}
