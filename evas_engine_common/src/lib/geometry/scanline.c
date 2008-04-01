#include <stdlib.h>
#include <stdio.h>

#include "Emage.h"
#include "emage_private.h"

/*============================================================================*
 *                                 Macros                                     * 
 *============================================================================*/

#define SRECT dc->fill.surface.srect
#define DRECT dc->fill.surface.drect

/* this macro assumes that the scanline sl intersects at least with one
 * of the vertical sides of the rectangle
 */
#define CALL_SCANLINES(rect, fn_inside, fn_outside)		 	\
/* backup the scanline */ 						\
sl_out = *sl; 								\
sl_in = sl_out; 							\
/* get the sub scanlines from left to right 				\
 *     +---+   +---+ 							\
 * s---|-R | s-|-R-|- 							\
 *     +---+   +---+ 							\
 */ 									\
if (_sl_split(&sl_out, &sl_in, rect.x)) 				\
{									\
	/*   +---+ 							\
	 * s-|-R-|- 							\
	 *   +---+  							\
	 * left								\
	 */ 								\
	fn_outside; 							\
	if (_sl_split(&sl_in, &sl_out, rect.x + rect.w)) 		\
	{ 								\
		/* 							\
		 * +---+      +---+ 					\
		 * | Rs|---   |sR | 					\
		 * +---+      +---+ 					\
		 * right						\
		 */ 							\
		fn_outside; 						\
	} 								\
	fn_inside; 							\
} 									\
/* 									\
 * +---+    								\
 * | Rs|---	 							\
 * +---+ 								\
 */ 									\
else 									\
{ 									\
	if (_sl_split(&sl_in, &sl_out, rect.x + rect.w))	 	\
	{								\
		/* right */						\
		fn_outside; 						\
	} 								\
	/* left */ 							\
	fn_inside; 							\
} 									


/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
/* TODO remove this and use the one provided by Emage.h */
static inline int
_sl_split(Emage_Scanline *sl, Emage_Scanline *rsl, int x)
{
	if ((sl->x <= x) && (sl->x + sl->w > x))
	{
		int x2;

		x2 = sl->x + sl->w;
		sl->w = x - sl->x;
		rsl->y = sl->y;
		rsl->x = x;
		rsl->w = x2 - x;
		return 1;
	}
	return 0;
}

static inline void
emage_scanline_draw_color(Emage_Scanline *sl, Emage_Surface *dst, Emage_Draw_Context *dc)
{
	Emage_Sl_Func func;
	int offset;
	
	func = emage_compositor_sl_color_get(dc, dst);
	offset = ((sl->y * dst->w)) + sl->x;
	//printf("from %d,%d width %d\n", sl->y, sl->x, sl->w);
	func(NULL, 0, NULL, 0, dc->fill.color, dst, offset, sl->w);
}


static inline void
emage_scanline_draw_pixel(Emage_Scanline *sl, Emage_Surface *dst, Emage_Draw_Context *dc, int soffset)
{
	Emage_Sl_Func func;
	int offset;
	
	/* TODO
	 * Split this function in three: _color (mul), _mask, and this 
	 * (only _pixel)
	 */
	func = emage_compositor_sl_pixel_get(dc, dc->fill.surface.s, dst);
	offset = ((sl->y * dst->w)) + sl->x;
	//printf("from %d,%d width %d\n", sl->y, sl->x, sl->w);
	func(dc->fill.surface.s, soffset, NULL, 0, 0, dst, offset, sl->w);
}

static inline void
_surface_norepeat_x(Emage_Scanline *sl, Emage_Surface *dst, Emage_Draw_Context *dc, int soffset)
{
	soffset += sl->x - DRECT.x;
	emage_scanline_draw_pixel(sl, dst, dc, soffset);
}

static inline void
_surface_repeat_x(Emage_Scanline *sl, Emage_Surface *dst, Emage_Draw_Context *dc, int soffset)
{
	Emage_Scanline tmp;
	int off;
	int w;

	/* the initial subscanline */
	off = (sl->x - DRECT.x) % SRECT.w;
	tmp = *sl;
	tmp.w = MIN(sl->w, SRECT.w - off); 
	emage_scanline_draw_pixel(&tmp, dst, dc, soffset + off);
	w = sl->w - tmp.w;
	/* the rest */
	while (w > 0)
	{
		tmp.x += tmp.w;
		tmp.w = MIN(SRECT.w, w);
		emage_scanline_draw_pixel(&tmp, dst, dc, soffset);
		w -= tmp.w;
	}
}


static inline void
emage_scanline_draw_surface(Emage_Scanline *sl, Emage_Surface *dst, Emage_Draw_Context *dc)
{
	Emage_Scanline sl_out, sl_in;
	int soffset;


	if (!(dc->fill.surface.type & EMAGE_FILL_SURFACE_REPEAT_Y))
	{
		/*
		 * +---+---+
		 * | S | S |
		 * +---+---+
		 * |  s----|----
		 * +-------+
		 */
		if (sl->y > DRECT.y + SRECT.h - 1)
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
		soffset = ((sl->y - DRECT.y) + SRECT.y) 
			* dc->fill.surface.s->w + SRECT.x;
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
		soffset = (((sl->y - DRECT.y) % SRECT.h)  + SRECT.y)
			* dc->fill.surface.s->w + SRECT.x;
	}
	/* simple cases are done, now the complex ones */
	if (!(dc->fill.surface.type & EMAGE_FILL_SURFACE_REPEAT_X))
	{
		Emage_Rectangle sd_rect; /* source rect inside dest rect */

		sd_rect.x = DRECT.x;
		sd_rect.y = DRECT.y;
		sd_rect.w = MIN(SRECT.w, DRECT.w);
		sd_rect.h = DRECT.h; /* dont need it to get the subscanlines */

		if (!(RECTS_INTERSECT(sl->x, sl->y, sl->w, 1, sd_rect.x,
			sd_rect.y, sd_rect.w, sd_rect.h)))
		{
			emage_scanline_draw_color(sl, dst, dc);
			return;
		}

		CALL_SCANLINES(sd_rect,
			_surface_norepeat_x(&sl_in, dst, dc, soffset),
			emage_scanline_draw_color(&sl_out, dst, dc))
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
		/* check if the scanline is inside the dst rect
		 * +---+          +---+      +---+     +---+
		 * | Ds|---   s---|-D |    s-|-D-|-    |sD |
		 * +---+          +----      +---+     +---+
		 */
		if (!(RECTS_INTERSECT(sl->x, sl->y, sl->w, 1, DRECT.x,
			DRECT.y, DRECT.w, DRECT.h)))
		{
			emage_scanline_draw_color(sl, dst, dc);
			return;
		}
		CALL_SCANLINES(DRECT,
			_surface_repeat_x(&sl_in, dst, dc, soffset),
			emage_scanline_draw_color(&sl_out, dst, dc))
	}
}

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * TODO
 */
EAPI void
emage_scanline_draw(Emage_Scanline *sl, Emage_Surface *dst, Emage_Draw_Context *dc)
{
	/* TODO
	 * handle special cases like drwing a surface to be repeated
	 * on X and the surface has only 1 pixel, so no need for
	 * pixel but use color
	 */
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
