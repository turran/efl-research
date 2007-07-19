#include <stdlib.h>

#include "Emage.h"
#include "emage_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void
rectangle_draw_internal(RGBA_Surface *dst, RGBA_Draw_Context *dc, int x, int y, int w, int h)
{
   RGBA_Gfx_Func func;
   int yy;
   DATA32 *ptr;

   if ((w <= 0) || (h <= 0)) return;
   RECTS_CLIP_TO_RECT(x, y, w, h, 0, 0, dst->w, dst->h);
   if ((w <= 0) || (h <= 0)) return;

   if (dc->clip.use)
     {
	RECTS_CLIP_TO_RECT(x, y, w, h, dc->clip.x, dc->clip.y, dc->clip.w, dc->clip.h);
     }
   if ((w <= 0) || (h <= 0)) return;

   func = evas_common_gfx_func_composite_color_span_get(dc->col.col, dst, w, dc->render_op);
   ptr = dst->data + (y * dst->w) + x;
   for (yy = 0; yy < h; yy++)
     {
#ifdef EVAS_SLI
	if (((yy + y) % dc->sli.h) == dc->sli.y)
#endif
	  {
	     func(NULL, NULL, dc->col.col, ptr, w);
	  }
	ptr += dst->w;
     }
}

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
EAPI void
emage_rectangle_draw(RGBA_Surface *dst, RGBA_Draw_Context *dc, int x, int y, int w, int h)
{
   Cutout_Rects *rects;
   Cutout_Rect  *r;
   int          c, cx, cy, cw, ch;
   int          i;
   /* handle cutouts here! */

   if ((w <= 0) || (h <= 0)) return;
   if (!(RECTS_INTERSECT(x, y, w, h, 0, 0, dst->w, dst->h)))
     return;
   /* no cutouts - cut right to the chase */
   if (!dc->cutout.rects)
     {
	rectangle_draw_internal(dst, dc, x, y, w, h);
	return;
     }
   /* save out clip info */
   c = dc->clip.use; cx = dc->clip.x; cy = dc->clip.y; cw = dc->clip.w; ch = dc->clip.h;
   emage_draw_context_clip_clip(dc, 0, 0, dst->w, dst->h);
   emage_draw_context_clip_clip(dc, x, y, w, h);
   /* our clip is 0 size.. abort */
   if ((dc->clip.w <= 0) || (dc->clip.h <= 0))
     {
	dc->clip.use = c; dc->clip.x = cx; dc->clip.y = cy; dc->clip.w = cw; dc->clip.h = ch;
	return;
     }
   rects = emage_draw_context_apply_cutouts(dc);
   for (i = 0; i < rects->active; ++i)
     {
	r = rects->rects + i;
	emage_draw_context_set_clip(dc, r->x, r->y, r->w, r->h);
	rectangle_draw_internal(dst, dc, x, y, w, h);
     }
   emage_draw_context_apply_clear_cutouts(rects);
   /* restore clip info */
   dc->clip.use = c; dc->clip.x = cx; dc->clip.y = cy; dc->clip.w = cw; dc->clip.h = ch;
}
