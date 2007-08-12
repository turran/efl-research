#include <stdlib.h>

#include "Emage.h"
#include "emage_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void
rectangle_draw_internal(Emage_Surface *dst, Emage_Draw_Context *dc, int x, int y, int w, int h)
{
   //RGBA_Gfx_Func func;
   Emage_Sl_Func func;
   int yy;
   int offset;
   DATA32 *ptr;

   if ((w <= 0) || (h <= 0)) return;
   RECTS_CLIP_TO_RECT(x, y, w, h, 0, 0, dst->w, dst->h);
   if ((w <= 0) || (h <= 0)) return;

   if (dc->clip.use)
     {
	RECTS_CLIP_TO_RECT(x, y, w, h, dc->clip.r.x, dc->clip.r.y, dc->clip.r.w, dc->clip.r.h);
     }
   if ((w <= 0) || (h <= 0)) return;

   //func = evas_common_gfx_func_composite_color_span_get(dc->col.col, dst, w, dc->render_op);
   func = emage_compositor_sl_color_get(dc, dst);
   //ptr = dst->data + (y * dst->w) + x;
   offset = (y * dst->w) + x;
   for (yy = 0; yy < h; yy++)
     {
#ifdef EVAS_SLI
	if (((yy + y) % dc->sli.h) == dc->sli.y)
#endif
	  {
	     func(NULL, 0, NULL, 0, dc->col.col, dst, offset, w);
	  }
	//ptr += dst->w;
	offset += dst->w;
     }
}

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
EAPI void
emage_rectangle_draw(Emage_Surface *dst, Emage_Draw_Context *dc, int x, int y, int w, int h)
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
   c = dc->clip.use; cx = dc->clip.r.x; cy = dc->clip.r.y; cw = dc->clip.r.w; ch = dc->clip.r.h;
   emage_draw_context_clip_clip(dc, 0, 0, dst->w, dst->h);
   emage_draw_context_clip_clip(dc, x, y, w, h);
   /* our clip is 0 size.. abort */
   if ((dc->clip.r.w <= 0) || (dc->clip.r.h <= 0))
     {
	dc->clip.use = c; dc->clip.r.x = cx; dc->clip.r.y = cy; dc->clip.r.w = cw; dc->clip.r.h = ch;
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
   dc->clip.use = c; dc->clip.r.x = cx; dc->clip.r.y = cy; dc->clip.r.w = cw; dc->clip.r.h = ch;
}
