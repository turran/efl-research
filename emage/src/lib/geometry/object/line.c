#include <stdlib.h>

#include "Emage.h"
#include "emage_private.h"

/*============================================================================*
 *                                 Macros                                     * 
 *============================================================================*/

#define IN_RANGE(x, y, w, h) \
( ((unsigned)(x) < (w)) && ((unsigned)(y) < (h)) )

#define EXCHANGE_POINTS(x0, y0, x1, y1) \
{ \
	int _tmp = y0; \
  \
	y0 = y1;   \
	y1 = _tmp; \
  \
	_tmp = x0; \
	x0 = x1;   \
	x1 = _tmp; \
}

#define SETUP_LINE_SHALLOW		\
	if (x0 > x1)							\
	  {									\
	   EXCHANGE_POINTS(x0, y0, x1, y1)				\
	   dx = -dx;							\
	   dy = -dy;							\
	  }									\
										\
	px = x0;								\
	py = y0;								\
										\
	p0_in = (IN_RANGE(x0 , y0 , clw, clh) ? 1 : 0);		\
	p1_in = (IN_RANGE(x1 , y1 , clw, clh) ? 1 : 0);		\
										\
	dely = 1;								\
	dh = dstw;								\
	if (dy < 0)								\
	  {									\
	   dely = -1;							\
	   dh = -dstw;							\
	  }									\
										\
	dyy = ((dy) << 16) / (dx);						\
										\
	if (!p0_in)								\
	  {									\
	   dxx = ((dx) << 16) / (dy);					\
	   if (px < 0)							\
	     {								\
		x = -px;  px = 0;						\
		yy = x * dyy;						\
		y = yy >> 16;						\
	        if (!a_a)							\
                   y += (yy - (y << 16)) >> 15;			\
		py += y;							\
		if ((dely > 0) && (py >= clh))			\
		   return;							\
		else if ((dely < 0) && (py < -1))			\
		   return;							\
	     }								\
										\
	   y = 0;								\
	   if ((dely > 0) && (py < -1))				\
		y = (-1 - py);						\
	   else if ((dely < 0) && (py >= clh))			\
		y = (clh - 1 - py);					\
										\
	   xx = y * dxx;							\
	   x = xx >> 16;							\
	   if (!a_a)							\
              x += (xx - (x << 16)) >> 15;			\
	   px += x;								\
	   if (px >= clw) return;					\
										\
	   yy = x * dyy;							\
	   y = yy >> 16;							\
	   if (!a_a)							\
              y += (yy - (y << 16)) >> 15;			\
	   py += y;								\
	   if ((dely > 0) && (py >= clh))				\
		 return;							\
	   else if ((dely < 0) && (py < -1))			\
		 return;							\
	  }									\
										\
	/* p = data + (dstw * py) + px;	*/			 	\
	offset = (dstw * py) + px;					\
										\
	x = px - x0;							\
	yy = x * dyy;							\
	prev_y = (yy >> 16);						\
										\
	rx = MIN(x1 + 1, clw);						\
	by = clh - 1;


#define SETUP_LINE_STEEP		\
   if (y0 > y1)								\
     {									\
	EXCHANGE_POINTS(x0, y0, x1, y1)				\
	dx = -dx;								\
	dy = -dy;								\
     }									\
										\
   px = x0;									\
   py = y0;									\
										\
   p0_in = (IN_RANGE(x0 , y0 , clw, clh) ? 1 : 0);		\
   p1_in = (IN_RANGE(x1 , y1 , clw, clh) ? 1 : 0);		\
										\
   delx = 1;								\
   if (dx < 0)								\
	delx = -1;								\
										\
   dxx = ((dx) << 16) / (dy);						\
										\
   if (!p0_in)								\
     {									\
	dyy = ((dy) << 16) / (dx);						\
										\
	if (py < 0)								\
	  {									\
	   y = -py;  py = 0;						\
	   xx = y * dxx;							\
	   x = xx >> 16;							\
	   if (!a_a)							\
              x += (xx - (x << 16)) >> 15;			\
	   px += x;								\
	   if ((delx > 0) && (px >= clw))				\
		return;							\
	   else if ((delx < 0) && (px < -1))			\
		return;							\
	  }									\
										\
	x = 0;								\
	if ((delx > 0) && (px < -1))					\
	   x = (-1 - px);							\
	else if ((delx < 0) && (px >= clw))				\
	   x = (clw - 1 - px);						\
										\
	yy = x * dyy;							\
	y = yy >> 16;							\
	if (!a_a)								\
           y += (yy - (y << 16)) >> 15;				\
	py += y;								\
	if (py >= clh) return;						\
										\
	xx = y * dxx;							\
	x = xx >> 16;							\
        if (!a_a)								\
           x += (xx - (x << 16)) >> 15;				\
	px += x;								\
	if ((delx > 0) && (px >= clw))				\
	   return;								\
	else if ((delx < 0) && (px < -1))				\
	   return;								\
     }									\
										\
   /* p = data + (dstw * py) + px; */					\
   offset = (dstw * py) + px;					\
										\
   y = py - y0;								\
   xx = y * dxx;								\
   prev_x = (xx >> 16);							\
										\
   by = MIN(y1 + 1, clh);						\
   rx = clw - 1;



/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/

static void
_evas_draw_point(Emage_Surface *dst, Emage_Draw_Context *dc, int x, int y)
{
#if 0
   Emage_Pt_Func pfunc;
   //RGBA_Gfx_Pt_Func pfunc;

   if (!IN_RANGE(x, y, dst->w, dst->h))
	return;
   if ((dc->clip.use) && (!IN_RECT(x, y, dc->clip.r.x, dc->clip.r.y, dc->clip.r.w, dc->clip.r.h)))
	return;
	pfunc = emage_compositor_pt_color_get(dc, dst);
   //pfunc = evas_common_gfx_func_composite_color_pt_get(dc->col.col, dst, dc->render_op);
   //if (pfunc)
	//pfunc(0, 255, dc->col.col, dst->data + (dst->w * y) + x);
	pfunc(0, 255, dc->col.col, dst, (dst->w * y) + x);
#endif
}

static void
_evas_draw_simple_line(Emage_Surface *dst, Emage_Draw_Context *dc, int x0, int y0, int x1, int y1)
{

}

static void
_evas_draw_line(Emage_Surface *dst, Emage_Draw_Context *dc, int x0, int y0, int x1, int y1)
{
	int step, err, r;
	int y;
	int dx = x1 - x0;
	int dy = y1 - y0;
	int offset;
	Emage_Pt_Func pfunc;
	Emage_Sl_Func sfunc;

	int j = 0;
	/* first octant */
	step = (dx - dy)/ (dy - 1);
	r = (dx - dy) - (step * (dy - 1));
	err = r;

	offset = (y0 * dst->w) + x0;

	sfunc = emage_compositor_sl_color_get(dc, dst);
	pfunc = emage_compositor_pt_color_get(dc, dst);
	for (y = 0; y < dy; y++, offset += dst->w)
	{
		/* draw point (x,y) */
		if (err >= dy)
		{
			err -= dy;
			sfunc(NULL, 0, NULL, 0, dc->fill.color, dst, offset, 2);
			offset += 2;
		}
		else
		{
			pfunc(NULL, 0, 0, dc->fill.color, dst, offset);
			offset++;
		}
		err += r;
	}
}

static void
_evas_draw_line_aa(Emage_Surface *dst, Emage_Draw_Context *dc, int x0, int y0, int x1, int y1)
{
	int step, err, r;
	int i;
	int dx = x1 - x0;
	int dy = y1 - y0;
	int offset;

	int half = dx/2;

	Emage_Pt_Func func;

	/* first octant */
	step = dx / dy;
	r = dx - (step * dy);
	err = r;
	
	offset = (y0 * dst->w) + x0;
	func = emage_compositor_pt_color_get(dc, dst);
	for (i = 0; i <= dx; i++, offset++)
	{
		if (err >= dy)
		{
			err -= dy;
			offset += dst->w;
		}
		/* draw point (x,y) */
		func(NULL, 0, 0, dc->fill.color, dst, offset);
		err += r;
	}
}

#if 0
/*
   these functions use the dc->clip data as bounding
   data. they assume that such data has already been cut
   back to lie in the dst image rect and the object's
   (line) bounding rect.
*/
static void
_evas_draw_simple_line(Emage_Surface *dst, Emage_Draw_Context *dc, int x0, int y0, int x1, int y1)
{
   int     dx, dy, len, lx, ty, rx, by;
   int     clx, cly, clw, clh;
   int     dstw;
   int 	offset;
   DATA32  *p; //, color; no need for this color
   
   Emage_Pt_Func pfunc;
   Emage_Sl_Func sfunc;

   //RGBA_Gfx_Pt_Func pfunc;
   //RGBA_Gfx_Func    sfunc;

   dstw = dst->w;
   //color = dc->col.col; no need for this color

   if (y0 > y1)
      EXCHANGE_POINTS(x0, y0, x1, y1)
   if (x0 > x1)
      EXCHANGE_POINTS(x0, y0, x1, y1)

   dx = x1 - x0;
   dy = y1 - y0;

   clx = dc->clip.x;
   cly = dc->clip.y;
   clw = dc->clip.w;
   clh = dc->clip.h;

   lx = clx;
   rx = clx + clw - 1;
   ty = cly;
   by = cly + clh - 1;

   if (dy == 0)
     {
#ifdef EVAS_SLI
	if (((y0) % dc->sli.h) == dc->sli.y)
#endif
	  {
	     if ((y0 >= ty) && (y0 <= by))
	       {
		  if (dx < 0)
		    {
		       int  tmp = x1;
		       
		       x1 = x0;
		       x0 = tmp;
		    }
		  
		  if (x0 < lx) x0 = lx;
		  if (x1 > rx) x1 = rx;
		  
		  len = x1 - x0 + 1;
		  //p = dst->data + (dstw * y0) + x0;
		  offset = (dstw * y0) + x0;
		  //sfunc = evas_common_gfx_func_composite_color_span_get(color, dst, len, dc->render_op);
		  sfunc = emage_compositor_sl_color_get(dc, dst);
		  //if (sfunc)
		    sfunc(NULL, NULL, dc->col.col, dst, offset, len);
	       }
	  }
	return;
     }

   pfunc = emage_compositor_pt_color_get(dc, dst);
   //pfunc = evas_common_gfx_func_composite_color_pt_get(color, dst, dc->render_op);
   //if (!pfunc) return;

   if (dx == 0)
     {
	if ((x0 >= lx) && (x0 <= rx))
	  {
	     if (y0 < ty) y0 = ty;
	     if (y1 > by) y1 = by;
	     
	     len = y1 - y0 + 1;
	     //p = dst->data + (dstw * y0) + x0;
	     offset = (dstw * y0) + x0;
	     while (len--)
	       {
#ifdef EVAS_SLI
		  if (((y1 + 1 - len) % dc->sli.h) == dc->sli.y)
#endif
		    {
		       //pfunc(0, 255, color, p);
		       pfunc(0, 255, dc->col.col, dst, offset);
		    }
		  //p += dstw;
		  offset += dstw;
	       }
	  }
	return;
     }

   if ((dy == dx) || (dy == -dx))
     {
	int   p0_in, p1_in;

	p0_in = (IN_RECT(x0, y0, clx, cly, clw, clh) ? 1 : 0);	
	p1_in = (IN_RECT(x1, y1, clx, cly, clw, clh) ? 1 : 0);

	if (dx > 0)
	  {
	    if (!p0_in)
	      {
		x0 = x0 + (ty - y0);
		y0 = ty;
		if (x0 > rx) return;
		if (x0 < lx)
		  {
		    y0 = y0 + (lx - x0);
		    x0 = lx;
		    if ((y0 < ty) || (y0 > by)) return;
		  }
	      }
	    if (!p1_in)
	      {
		x1 = x0 + (by - y0);
		y1 = by;
		if (x1 < lx) return;
		if (x1 > rx)
		  {
		    y1 = y0 + (rx - x0);
		    x1 = rx;
		    if ((y1 < ty) || (y1 > by)) return;
		  }
	      }
	  }
	else
	  {
	    if (!p0_in)
	      {
		x0 = x0 - (ty - y0);
		y0 = ty;
		if (x0 < lx) return;
		if (x0 > rx)
		  {
		    y0 = y0 - (rx - x0);
		    x0 = rx;
		    if ((y0 < ty) || (y0 > by)) return;
		  }
	      }
	    if (!p1_in)
	      {
		x1 = x0 - (by - y0);
		y1 = by;
		if (x1 > rx) return;
		if (x1 < lx)
		  {
		    y1 = y0 - (lx - x0);
		    x1 = lx;
		    if ((y1 < ty) || (y1 > by)) return;
		  }
	      }
	  }
	if (y1 > y0)
	  {
	     //p = dst->data + (dstw * y0) + x0;
	     offset = (dstw * y0) + x0;
	     len = y1 - y0 + 1;
	     if (dx > 0)  dstw++;
	     else  dstw--;
	  }
	else
	  {
	     len = y0 - y1 + 1;
	     //p = dst->data + (dstw * y1) + x1;
	     offset = (dstw * y1) + x1;
	     if (dx > 0)  dstw--;
	     else  dstw++;
	  }

	while (len--)
	  {
#ifdef EVAS_SLI
	     if (((y1 + 1 - len) % dc->sli.h) == dc->sli.y)
#endif
	       {
		  //pfunc(0, 255, color, p);
		  pfunc(0, 255, dc->col.col, dst, offset);
	       }
	    //p += dstw;
	    offset += dstw;
	  }
     }
}


static void
_evas_draw_line(Emage_Surface *dst, Emage_Draw_Context *dc, int x0, int y0, int x1, int y1)
{
   int     px, py, x, y, prev_x, prev_y;
   int     dx, dy, rx, by, p0_in, p1_in, dh, a_a = 0;
   int     delx, dely, xx, yy, dxx, dyy;
   int     clx, cly, clw, clh;
   int     dstw;
   int 	offset;
   //DATA32  *p, *data, color;
   DATA32  color;
   Emage_Pt_Func pfunc;
   //RGBA_Gfx_Pt_Func pfunc;

   dx = x1 - x0;
   dy = y1 - y0;

   if ( (dx == 0) || (dy == 0) || (dx == dy) || (dx == -dy) )
     {
	_evas_draw_simple_line(dst, dc, x0, y0, x1, y1);
	return;
     }

   color = dc->col.col;
   //pfunc = evas_common_gfx_func_composite_color_pt_get(color, dst, dc->render_op);
   pfunc = emage_compositor_pt_color_get(dc, dst);
   //if (!pfunc) return;

   clx = dc->clip.x;
   cly = dc->clip.y;
   clw = dc->clip.w;
   clh = dc->clip.h;

   //data = dst->data;
   dstw = dst->w;

   //data += (dstw * cly) + clx;
   offset = (dstw * cly) + clx;
   x0 -= clx;
   y0 -= cly;
   x1 -= clx;
   y1 -= cly;

   /* shallow: x-parametric */
   if ((dy < dx) || (dy < -dx))
     {
	SETUP_LINE_SHALLOW

	while (px < rx)
	  {
	    y = (yy >> 16);
	    y += ((yy - (y << 16)) >> 15);
	    if (prev_y != y)
	      {
		prev_y = y;
		//p += dh;
		offset += dh;
		py += dely;
	      }
	    if (!p1_in)
	      {
		if ((py < 0) && (dely < 0)) return;
		if ((py > by) && (dely > 0)) return;
	      }
#ifdef EVAS_SLI
	     if (((py) % dc->sli.h) == dc->sli.y)
#endif
	       {
		  if (IN_RANGE(px, py, clw, clh))
		    pfunc(0, 255, color, dst, offset);
	       }
	    yy += dyy;
	    px++;
	    //p++;
	    offset++;
	  }
	return;
     }

   /* steep: y-parametric */

   SETUP_LINE_STEEP

   while (py < by)
     {
	x = (xx >> 16);
	x += ((xx - (x << 16)) >> 15);
	if (prev_x != x)
	  {
	    prev_x = x;
	    px += delx;
	    //p += delx;
	    offset += delx;
	  }
	if (!p1_in)
	  {
	    if ((px < 0) && (delx < 0)) return;
	    if ((px > rx) && (delx > 0)) return;
	  }
#ifdef EVAS_SLI
	if (((py) % dc->sli.h) == dc->sli.y)
#endif
	  {
	     if (IN_RANGE(px, py, clw, clh))
	       //pfunc(0, 255, color, p);
	       pfunc(0, 255, color, dst, offset);
	  }
	xx += dxx;
	py++;
	//p += dstw;
	offset += dstw;
     }
}

static void
_evas_draw_line_aa(Emage_Surface *dst, Emage_Draw_Context *dc, int x0, int y0, int x1, int y1)
{
   int     px, py, x, y, prev_x, prev_y;
   int     dx, dy, rx, by, p0_in, p1_in, dh, a_a = 1;
   int     delx, dely, xx, yy, dxx, dyy;
   int     clx, cly, clw, clh;
   int     dstw;
   DATA32  color;
   int offset;
   //RGBA_Gfx_Pt_Func pfunc;
   //DATA32  *p, *data, color;
   Emage_Pt_Func pfunc;

   if (y0 > y1)
      EXCHANGE_POINTS(x0, y0, x1, y1)
   dx = x1 - x0;
   dy = y1 - y0;

   if ( (dx == 0) || (dy == 0) || (dx == dy) || (dx == -dy) )
     {
	_evas_draw_simple_line(dst, dc, x0, y0, x1, y1);
	return;
     }

   color = dc->col.col;
   pfunc = emage_compositor_pt_mask_color_get(dc, dst);
   //pfunc = evas_common_gfx_func_composite_mask_color_pt_get(color, dst, dc->render_op);
   //if (!pfunc) return;

   clx = dc->clip.x;
   cly = dc->clip.y;
   clw = dc->clip.w;
   clh = dc->clip.h;

   //data = dst->data;
   dstw = dst->w;

   //data += (dstw * cly) + clx;
   offset = (dstw * cly) + clx;
   x0 -= clx;
   y0 -= cly;
   x1 -= clx;
   y1 -= cly;

   /* shallow: x-parametric */
   if ((dy < dx) || (dy < -dx))
     {
	SETUP_LINE_SHALLOW

	while (px < rx)
	  {
	    DATA8   aa;

	    y = (yy >> 16);
	    if (prev_y != y)
	      {
		prev_y = y;
		//p += dh;
		offset += dh;
		py += dely;
	      }
	    if (!p1_in)
	      {
		if ((py < -1) && (dely < 0)) return;
		if ((py > by) && (dely > 0)) return;
	      }
	    if ((unsigned)(px) < clw)
	      {
		aa = ((yy - (y << 16)) >> 8);
	   	if ((unsigned)(py) < clh)
		   //pfunc(0, 255 - aa, color, p);
		   pfunc(0, 255 - aa, color, dst, offset);
	   	if ((unsigned)(py + 1) < clh)
		   pfunc(0, aa, color, dst, offset + dstw);
		   //pfunc(0, aa, color, p + dstw);
	      }
	    yy += dyy;
	    px++;
	    //p++;
	    offset++;
	  }
	return;
     }

   /* steep: y-parametric */

   SETUP_LINE_STEEP

   while (py < by)
     {
	DATA8   aa;

	x = (xx >> 16);
	if (prev_x != x)
	  {
	    prev_x = x;
	    px += delx;
	    //p += delx;
	    offset += delx;
	  }
	if (!p1_in)
	  {
	    if ((px < -1) && (delx < 0)) return;
	    if ((px > rx) && (delx > 0)) return;
	  }
	if ((unsigned)(py) < clh)
	  {
	    aa = ((xx - (x << 16)) >> 8);
	    if ((unsigned)(px) < clw)
		//pfunc(0, 255 - aa, color, p);
		pfunc(0, 255 - aa, color, dst, offset);
	    if ((unsigned)(px + 1) < clw)
		//pfunc(0, aa, color, p + 1);
		pfunc(0, aa, color, dst, offset + 1);
	  }
	xx += dxx;
	py++;
	//p += dstw;
	offset += dstw;
     }
}
#endif
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
EAPI void
emage_line_draw(Emage_Surface *dst, Emage_Draw_Context *dc, int x0, int y0, int x1, int y1)
{
   int  x, y, w, h;
   int  clx, cly, clw, clh;
   int  cuse, cx, cy, cw, ch;

   if ((x0 == x1) && (y0 == y1))
     {
	_evas_draw_point(dst, dc, x0, y0);
	return;
     }

   clx = cly = 0;
   clw = dst->w;
   clh = dst->h;

   /* save out clip info */
   cuse = dc->clip.use;
   cx = dc->clip.r.x;
   cy = dc->clip.r.y;
   cw = dc->clip.r.w;
   ch = dc->clip.r.h;

   if (cuse)
     {
	RECTS_CLIP_TO_RECT(clx, cly, clw, clh, cx, cy, cw, ch);
	if ((clw < 1) || (clh < 1))
	   return;
     }

   x = MIN(x0, x1);
   y = MIN(y0, y1);
   w = MAX(x0, x1) - x + 1;
   h = MAX(y0, y1) - y + 1;

   RECTS_CLIP_TO_RECT(clx, cly, clw, clh, x, y, w, h);
   if ((clw < 1) || (clh < 1))
	return;

   dc->clip.use = 1;
   dc->clip.r.x = clx;
   dc->clip.r.y = cly;
   dc->clip.r.w = clw;
   dc->clip.r.h = clh;

   if (dc->anti_alias)
	_evas_draw_line_aa(dst, dc, x0, y0, x1, y1);
   else
	_evas_draw_line(dst, dc, x0, y0, x1, y1);

   /* restore clip info */
   dc->clip.use = cuse;
   dc->clip.r.x = cx;
   dc->clip.r.y = cy;
   dc->clip.r.w = cw;
   dc->clip.r.h = ch;
   evas_common_cpu_end_opt();
}
