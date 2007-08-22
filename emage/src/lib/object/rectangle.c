#include <stdlib.h>

#include "Emage.h"
#include "emage_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void
rectangle_draw_internal(Emage_Surface *dst, Emage_Draw_Context *dc, int x, int y, int w, int h)
{
	Emage_Scanline 	sl;
	int 		hh;

	if ((w <= 0) || (h <= 0)) return;
	RECTS_CLIP_TO_RECT(x, y, w, h, 0, 0, dst->w, dst->h);
	if ((w <= 0) || (h <= 0)) return;

	if (dc->clip.use)
	{
		RECTS_CLIP_TO_RECT(x, y, w, h, dc->clip.r.x, dc->clip.r.y, dc->clip.r.w, dc->clip.r.h);
	}
	if ((w <= 0) || (h <= 0)) return;

	sl.x = x;
	sl.w = w;
	sl.coverage = 32; // FIXME

	hh = y + h;
	for (sl.y = y; sl.y < hh; sl.y++)
	{
		emage_scanline_draw(&sl, dst, dc);
	}
}

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * TODO
 */
EAPI void
emage_rectangle_draw(Emage_Rectangle *r, Emage_Surface *dst, Emage_Draw_Context *dc)
{
	Cutout_Rects 			*crects;
	Cutout_Rect  			*cr;
	Emage_Draw_Context_Clip 	bc;
	int          			i;
	
	/* handle cutouts here! */
	if ((r->w <= 0) || (r->h <= 0)) return;
	if (!(RECTS_INTERSECT(r->x, r->y, r->w, r->h, 0, 0, dst->w, dst->h)))
		return;
	/* no cutouts - cut right to the chase */
	if (!dc->cutout.rects)
	{
		rectangle_draw_internal(dst, dc, r->x, r->y, r->w, r->h);
		return;
	}
	/* save out clip info */
	bc = dc->clip;
	emage_draw_context_clip_clip(dc, 0, 0, dst->w, dst->h);
	emage_draw_context_clip_clip(dc, r->x, r->y, r->w, r->h);
	/* our clip is 0 size.. abort */
	if ((dc->clip.r.w <= 0) || (dc->clip.r.h <= 0))
	{
		dc->clip = bc;
		return;
	}
	crects = emage_draw_context_apply_cutouts(dc);
	for (i = 0; i < crects->active; ++i)
	{
		cr = crects->rects + i;
		emage_draw_context_set_clip(dc, cr->x, cr->y, cr->w, cr->h);
		rectangle_draw_internal(dst, dc, r->x, r->y, r->w, r->h);
	}
	emage_draw_context_apply_clear_cutouts(crects);
	/* restore clip info */
	dc->clip = bc;
}
