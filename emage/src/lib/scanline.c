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
	int offset;
	
	func = emage_compositor_sl_color_get(dc, dst);
	offset = ((sl->y * dst->w)) + sl->x;
	//printf("from %d,%d width %d\n", sl->y, sl->x, sl->w);
	func(NULL, 0, NULL, 0, dc->col.col, dst, offset, sl->w);
}

static inline void
emage_scanline_draw_pixel(Emage_Scanline *sl, Emage_Surface *dst, Emage_Draw_Context *dc, int soffset)
{
	Emage_Sl_Func func;
	int offset;
	
	func = emage_compositor_sl_pixel_get(dc, dc->fill.surface.s, dst);
	offset = ((sl->y * dst->w)) + sl->x;
	printf("from %d,%d width %d\n", sl->y, sl->x, sl->w);
	//printf("from %d,%d width %d\n", sl->y, sl->x, sl->w);
	func(dc->fill.surface.s, soffset, NULL, 0, 0, dst, offset, sl->w);
}

static inline void
emage_scanline_draw_surface(Emage_Scanline *sl, Emage_Surface *dst, Emage_Draw_Context *dc)
{
	Emage_Sl_Func func;

	Emage_Rectangle *drect = &dc->fill.surface.drect;
	Emage_Rectangle *srect = &dc->fill.surface.srect;

	int i;
	int offset;
	int doffset;
	int soffset;

	/* TODO
	 * Split this function in three: _color (mul), _mask, and this 
	 * (only _pixel)
	 */
	
	/* check if the scanline is inside the dst rect
	 * +---+          +---+      +---+     +---+
	 * | --|---s  s---|-- |    s-|---|-    |s- |
	 * +---+          +----      +---+     +---+
	 */
	if (!(RECTS_INTERSECT(sl->x, sl->y, sl->w, 1, drect->x,
		drect->y, drect->w, drect->h)))
	{
		emage_scanline_draw_color(sl, dst, dc);
		return;
	}
#if 0
	printf("no\n");
	printf("from %d,%d width %d\n", sl->y, sl->x, sl->w);
	printf("rect %d,%d x %d,%d\n", drect->x, drect->y, drect->w, drect->h);
#endif
	/* the span (or part) is inside the drect. if we dont repeat on y 
	 * and the span isnt inside the drect + srect use the color
	 */
	if (!(dc->fill.surface.type & EMAGE_FILL_SURFACE_REPEAT_Y))
	{
		/*
		 * +---+---+
		 * | S | S |
		 * +---+---+
		 * |  s----|----
		 * +-------+
		 */
		if (sl->y > drect->y + srect->h - 1)
		{
			emage_scanline_draw_color(sl, dst, dc);
			return;
		}
		/*
		 * +---+---+
		 * |Ss-|-S-|----
		 * +---+---+
		 * |       |
		 * +-------+
		 */
		soffset = (sl->y - drect->y) * dc->fill.surface.s->w;
	}
	/*
	 * +---+---+
	 * | S |   |
	 * +---+   +
	 * | S-|---|----s
	 * +-------+
	 */
	else
	{
		soffset = ((sl->y - drect->y) % srect->h )* dc->fill.surface.s->w;
	}
	/* TODO get the src surface and coordinate */
	/*
	 * +---+---+       +---+---+
	 * |Ss-|---|---    | S | s-|----
	 * +---+   +       +---+   |
	 * |   D   |       |   D   |
	 * +-------+       +-------+
	 */
	if (!(dc->fill.surface.type & EMAGE_FILL_SURFACE_REPEAT_X))
	{
		Emage_Scanline sl_tmp;
		int w;
			
		/* scanline outside srect just use color
		 *     +---+  +---+
		 * s---|   |  |   |s---
		 *     +---+  +---+
		 */
		if ((sl->x > drect->x + srect->w) ||
			(sl->x + sl->w < drect->x))
		{
			emage_scanline_draw_color(sl, dst, dc);
			return;
		}
		w = MIN(srect->w, drect->w);
		/* part of scanline inside srect
		 * +---+
		 * |Ss-|---
		 * +---+
		 */
		if (sl->x >= drect->x)
		{
			/* left */
			sl_tmp = *sl;
			sl_tmp.w = MIN(w, sl_tmp.w);
			soffset += sl->x - drect->x;
			emage_scanline_draw_pixel(&sl_tmp, dst, dc, soffset);
			printf("L\n");
			/* right */
			if (sl_tmp.w < sl->w)
			{
				sl_tmp.x += sl_tmp.w;
				sl_tmp.w = sl->w - sl_tmp.w;
				emage_scanline_draw_color(&sl_tmp, dst, dc);
			}
		}
		/*
		 *     +---+
		 * s---|-S |
		 *     +---+
		 */
		else
		{
			/* left */
			sl_tmp = *sl;
			sl_tmp.w = drect->x - sl_tmp.x;
			emage_scanline_draw_color(&sl_tmp, dst, dc);

			sl_tmp.w = sl->w - sl_tmp.w;
			sl_tmp.x = drect->x;
			if (sl_tmp.w <= w)
			{
				/* middle */
				emage_scanline_draw_pixel(&sl_tmp, dst, dc, soffset);
				printf("M1\n");
			}
			else
			{
				/* middle */
				sl_tmp.w = w;
				emage_scanline_draw_pixel(&sl_tmp, dst, dc, soffset);
				printf("M2\n");
				/* right */
				sl_tmp.x += sl_tmp.w;
				sl_tmp.w = sl->x + sl->w - sl_tmp.x;
				emage_scanline_draw_color(&sl_tmp, dst, dc);
			}
		}
	}
	/*
	 * +---+---+       +---+---+
	 * | S-|-S-|----s  | S-|-S-|----s
	 * +---+---+       +---+---|
	 * |       |       | S | S |
	 * +-------+       +-------+
	 */
	else
	{
		printf("TODO\n");
	}
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
