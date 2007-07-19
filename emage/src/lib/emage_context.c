#include <stdlib.h>
#include <string.h>

#include "Emage.h"
#include "emage_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
int
emage_draw_context_cutout_split(Cutout_Rects* res, int index, Cutout_Rect *split)
{
   /* 1 input rect, multiple out */
   Cutout_Rect  in = res->rects[index];

   /* this is to save me a LOT of typing */
#define INX1 (in.x)
#define INX2 (in.x + in.w)
#define SPX1 (split->x)
#define SPX2 (split->x + split->w)
#define INY1 (in.y)
#define INY2 (in.y + in.h)
#define SPY1 (split->y)
#define SPY2 (split->y + split->h)
#define X1_IN (in.x < split->x)
#define X2_IN ((in.x + in.w) > (split->x + split->w))
#define Y1_IN (in.y < split->y)
#define Y2_IN ((in.y + in.h) > (split->y + split->h))
#define R_NEW(_r, _x, _y, _w, _h) { emage_draw_context_cutouts_add(_r, _x, _y, _w, _h); }
   if (!RECTS_INTERSECT(in.x, in.y, in.w, in.h,
			split->x, split->y, split->w, split->h))
     {
        /* No colision => no clipping, don't touch it. */
	return 1;
     }

   /* S    = split (ie cut out rect) */
   /* +--+ = in (rect to be cut) */

   /*
    *  +---+
    *  |   |
    *  | S |
    *  |   |
    *  +---+
    *
    */
   if (X1_IN && X2_IN && Y1_IN && Y2_IN)
     {
        R_NEW(res, in.x, in.y, in.w, SPY1 - in.y);
	R_NEW(res, in.x, SPY1, SPX1 - in.x, SPY2 - SPY1);
	R_NEW(res, SPX2, SPY1, INX2 - SPX2, SPY2 - SPY1);
        /* out => (in.x, SPY2, in.w, INY2 - SPY2) */
        res->rects[index].h = INY2 - SPY2;
        res->rects[index].y = SPY2;
	return 1;
     }
   /* SSSSSSS
    * S+---+S
    * S|SSS|S
    * S|SSS|S
    * S|SSS|S
    * S+---+S
    * SSSSSSS
    */
   if (!X1_IN && !X2_IN && !Y1_IN && !Y2_IN)
     {
        emage_draw_context_cutouts_del(res, index);
	return 0;
     }
   /* SSS
    * S+---+
    * S|S  |
    * S|S  |
    * S|S  |
    * S+---+
    * SSS
    */
   if (!X1_IN && X2_IN && !Y1_IN && !Y2_IN)
     {
        /* in => (SPX2, in.y, INX2 - SPX2, in.h) */
        res->rects[index].w = INX2 - SPX2;
        res->rects[index].x = SPX2;
	return 1;
     }
   /*    S
    *  +---+
    *  | S |
    *  | S |
    *  | S |
    *  +---+
    *    S
    */
   if (X1_IN && X2_IN && !Y1_IN && !Y2_IN)
     {
        R_NEW(res, in.x, in.y, SPX1 - in.x, in.h);
        /* in => (SPX2, in.y, INX2 - SPX2, in.h) */
        res->rects[index].w = INX2 - SPX2;
        res->rects[index].x = SPX2;
	return 1;
     }
   /*     SSS
    *  +---+S
    *  |  S|S
    *  |  S|S
    *  |  S|S
    *  +---+S
    *     SSS
    */
   if (X1_IN && !X2_IN && !Y1_IN && !Y2_IN)
     {
        /* in => (in.x, in.y, SPX1 - in.x, in.h) */
        res->rects[index].w = SPX1 - in.x;
	return 1;
     }
   /* SSSSSSS
    * S+---+S
    * S|SSS|S
    *  |   |
    *  |   |
    *  +---+
    *
    */
   if (!X1_IN && !X2_IN && !Y1_IN && Y2_IN)
     {
        /* in => (in.x, SPY2, in.w, INY2 - SPY2) */
        res->rects[index].h = INY2 - SPY2;
        res->rects[index].y = SPY2;
	return 1;
     }
   /*
    *  +---+
    *  |   |
    * S|SSS|S
    *  |   |
    *  +---+
    *
    */
   if (!X1_IN && !X2_IN && Y1_IN && Y2_IN)
     {
        R_NEW(res, in.x, SPY2, in.w, INY2 - SPY2);
        /* in => (in.x, in.y, in.w, SPY1 - in.y) */
        res->rects[index].h = SPY1 - in.y;
	return 1;
     }
   /*
    *  +---+
    *  |   |
    *  |   |
    * S|SSS|S
    * S+---+S
    * SSSSSSS
    */
   if (!X1_IN && !X2_IN && Y1_IN && !Y2_IN)
     {
        /* in => (in.x, in.y, in.w, SPY1 - in.y) */
        res->rects[index].h = SPY1 - in.y;
	return 1;
     }
   /* SSS
    * S+---+
    * S|S  |
    *  |   |
    *  |   |
    *  +---+
    *
    */
   if (!X1_IN && X2_IN && !Y1_IN && Y2_IN)
     {
	R_NEW(res, SPX2, in.y, INX2 - SPX2, SPY2 - in.y);
        /* in => (in.x, SPY2, in.w, INY2 - SPY2) */
        res->rects[index].h = INY2 - SPY2;
        res->rects[index].y = SPY2;
	return 1;
     }
   /*    S
    *  +---+
    *  | S |
    *  |   |
    *  |   |
    *  +---+
    *
    */
   if (X1_IN && X2_IN && !Y1_IN && Y2_IN)
     {
	R_NEW(res, in.x, in.y, SPX1 - in.x, SPY2 - in.y);
	R_NEW(res, SPX2, in.y, INX2 - SPX2, SPY2 - in.y);
        /* in => (in.x, SPY2, in.w, INY2 - SPY2) */
        res->rects[index].h = INY2 - SPY2;
        res->rects[index].y = SPY2;
	return 1;
     }
   /*     SSS
    *  +---+S
    *  |  S|S
    *  |   |
    *  |   |
    *  +---+
    *
    */
   if (X1_IN && !X2_IN && !Y1_IN && Y2_IN)
     {
	R_NEW(res, in.x, in.y, SPX1 - in.x, SPY2 - in.y);
        /* in => (in.x, SPY2, in.w, INY2 - SPY2) */
        res->rects[index].h = INY2 - SPY2;
        res->rects[index].y = SPY2;
	return 1;
     }
   /*
    *  +---+
    *  |   |
    * S|S  |
    *  |   |
    *  +---+
    *
    */
   if (!X1_IN && X2_IN && Y1_IN && Y2_IN)
     {
	R_NEW(res, in.x, SPY2, in.w, INY2 - SPY2);
	R_NEW(res, SPX2, SPY1, INX2 - SPX2, SPY2 - SPY1);
        /* in => (in.x, SPY2, in.w, INY2 - SPY2) */
        res->rects[index].h = SPY1 - in.y;
	return 1;
     }
   /*
    *  +---+
    *  |   |
    *  |  S|S
    *  |   |
    *  +---+
    *
    */
   if (X1_IN && !X2_IN && Y1_IN && Y2_IN)
     {
	R_NEW(res, in.x, SPY2, in.w, INY2 - SPY2);
	R_NEW(res, in.x, SPY1, SPX1 - in.x, SPY2 - SPY1);
        /* in => (in.x, in.y, in.w, SPY1 - in.y) */
        res->rects[index].h = SPY1 - in.y;
	return 1;
     }
   /*
    *  +---+
    *  |   |
    *  |   |
    * S|S  |
    * S+---+
    * SSS
    */
   if (!X1_IN && X2_IN && Y1_IN && !Y2_IN)
     {
        R_NEW(res, SPX2, SPY1, INX2 - SPX2, INY2 - SPY1);
        /* in => (in.x, in.y, in.w, SPY1 - in.y) */
        res->rects[index].h = SPY1 - in.y;
	return 1;
     }
   /*
    *  +---+
    *  |   |
    *  |   |
    *  | S |
    *  +---+
    *    S
    */
   if (X1_IN && X2_IN && Y1_IN && !Y2_IN)
     {
	R_NEW(res, in.x, SPY1, SPX1 - in.x, INY2 - SPY1);
        R_NEW(res, SPX2, SPY1, INX2 - SPX2, INY2 - SPY1);
        /* in => (in.x, in.y, in.w, SPY1 - in.y) */
        res->rects[index].h = SPY1 - in.y;
	return 1;
     }
   /*
    *  +---+
    *  |   |
    *  |   |
    *  |  S|S
    *  +---+S
    *     SSS
    */
   if (X1_IN && !X2_IN && Y1_IN && !Y2_IN)
     {
        R_NEW(res, in.x, SPY1, SPX1 - in.x, INY2 - SPY1);
        /* in => (in.x, in.y, in.w, SPY1 - in.y) */
        res->rects[index].h = SPY1 - in.y;
	return 1;
     }
   emage_draw_context_cutouts_del(res, index);
   return 0;
#undef INX1
#undef INX2
#undef SPX1
#undef SPX2
#undef INY1
#undef INY2
#undef SPY1
#undef SPY2
#undef X1_IN
#undef X2_IN
#undef Y1_IN
#undef Y2_IN
#undef R_NEW
}



/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/


/**
 *
 */
EAPI RGBA_Draw_Context *
emage_draw_context_new(void)
{
   RGBA_Draw_Context *dc;

   dc = calloc(1, sizeof(RGBA_Draw_Context));
   dc->sli.h = 1;
   return dc;
}

/**
 *
 */
EAPI Cutout_Rects*
emage_draw_context_cutouts_new()
{
   Cutout_Rects *rects;

   rects = malloc(sizeof(Cutout_Rects));
   rects->rects = NULL;
   rects->active = 0;
   rects->max = 0;

   return rects;
}

/**
 *
 */
EAPI void
emage_draw_context_cutouts_free(Cutout_Rects* rects)
{
   rects->active = 0;
}

/**
 *
 */
EAPI Cutout_Rect*
emage_draw_context_cutouts_add(Cutout_Rects* rects,
                                     int x, int y, int w, int h)
{
   Cutout_Rect* rect;

   if (rects->max < rects->active + 1) {
      rects->max += 8;
      rects->rects = realloc(rects->rects, sizeof(Cutout_Rect) * rects->max);
   }

   rect = rects->rects + rects->active++;
   rect->x = x;
   rect->y = y;
   rect->w = w;
   rect->h = h;

   return rect;
}

/**
 *
 */
EAPI void
emage_draw_context_cutouts_del(Cutout_Rects* rects,
                                     int index)
{
   if (index >= 0 && index < rects->active)
     {
        Cutout_Rect*    rect = rects->rects + index;

        memmove(rect, rect + 1, sizeof (Cutout_Rect) * (rects->active - index - 1));
        rects->active--;
     }
}

/**
 *
 */
EAPI void
emage_draw_context_apply_clean_cutouts(Cutout_Rects* rects)
{
   free(rects->rects);
   rects->rects = NULL;
   rects->active = 0;
   rects->max = 0;
}


EAPI void
emage_draw_context_clear_cutouts(RGBA_Draw_Context *dc)
{
   emage_draw_context_apply_clean_cutouts(&dc->cutout);
}

/**
 *
 */
EAPI void
emage_draw_context_free(RGBA_Draw_Context *dc)
{
   if (!dc) return;

   emage_draw_context_apply_clean_cutouts(&dc->cutout);
   free(dc);
}


/**
 *
 */
EAPI void
emage_draw_context_set_clip(RGBA_Draw_Context *dc, int x, int y, int w, int h)
{
   dc->clip.use = 1;
   dc->clip.x = x;
   dc->clip.y = y;
   dc->clip.w = w;
   dc->clip.h = h;
}

/**
 *
 */
EAPI void
emage_draw_context_clip_clip(RGBA_Draw_Context *dc, int x, int y, int w, int h)
{
   if (dc->clip.use)
     {
	RECTS_CLIP_TO_RECT(dc->clip.x, dc->clip.y, dc->clip.w, dc->clip.h,
			   x, y, w, h);
     }
   else
     emage_draw_context_set_clip(dc, x, y, w, h);
}

/**
 *
 */
EAPI void
emage_draw_context_unset_clip(RGBA_Draw_Context *dc)
{
   dc->clip.use = 0;
}

/**
 *
 */
EAPI void
emage_draw_context_set_color(RGBA_Draw_Context *dc, int r, int g, int b, int a)
{
   R_VAL(&(dc->col.col)) = (DATA8)r;
   G_VAL(&(dc->col.col)) = (DATA8)g;
   B_VAL(&(dc->col.col)) = (DATA8)b;
   A_VAL(&(dc->col.col)) = (DATA8)a;
}

/**
 *
 */
EAPI void
emage_draw_context_set_multiplier(RGBA_Draw_Context *dc, int r, int g, int b, int a)
{
   dc->mul.use = 1;
   R_VAL(&(dc->mul.col)) = (DATA8)r;
   G_VAL(&(dc->mul.col)) = (DATA8)g;
   B_VAL(&(dc->mul.col)) = (DATA8)b;
   A_VAL(&(dc->mul.col)) = (DATA8)a;
}

/**
 *
 */
EAPI void
emage_draw_context_unset_multiplier(RGBA_Draw_Context *dc)
{
   dc->mul.use = 0;
}


/**
 *
 */
EAPI void
emage_draw_context_add_cutout(RGBA_Draw_Context *dc, int x, int y, int w, int h)
{
   emage_draw_context_cutouts_add(&dc->cutout, x, y, w, h);
}

/**
 *
 */
EAPI void
emage_draw_context_apply_clear_cutouts(Cutout_Rects* rects)
{
   emage_draw_context_apply_clean_cutouts(rects);
   free(rects);
}


/**
 *
 */
EAPI void
emage_draw_context_set_anti_alias(RGBA_Draw_Context *dc , unsigned char aa)
{
   dc->anti_alias = !!aa;
}

/**
 *
 */
EAPI void
emage_draw_context_set_color_interpolation(RGBA_Draw_Context *dc, int color_space)
{
   dc->interpolation.color_space = color_space;
}

/**
 *
 */
EAPI void
emage_draw_context_set_render_op(RGBA_Draw_Context *dc , int op)
{
   dc->render_op = op;
}

/**
 *
 */
EAPI void
emage_draw_context_set_sli(RGBA_Draw_Context *dc, int y, int h)
{
   dc->sli.y = y;
   dc->sli.h = h;
}

/**
 *
 */
EAPI Cutout_Rects*
emage_draw_context_apply_cutouts(RGBA_Draw_Context *dc)
{
   Cutout_Rects*        res;
   int                  i;
   int                  j;

   if (!dc->clip.use) return NULL;
   if ((dc->clip.w <= 0) || (dc->clip.h <= 0)) return NULL;

   res = emage_draw_context_cutouts_new();
   emage_draw_context_cutouts_add(res, dc->clip.x, dc->clip.y, dc->clip.w, dc->clip.h);

   for (i = 0; i < dc->cutout.active; ++i)
     {
        /* Don't loop on the element just added to the list as they are already correctly clipped. */
        int active = res->active;

        for (j = 0; j < active; )
          {
             if (emage_draw_context_cutout_split(res, j, dc->cutout.rects + i))
               ++j;
             else
               active--;
          }
     }
   return res;
}
