#include "Emage.h"
#include "emage_private.h"

#include <stdlib.h>

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/

/* source rect coordinate: src
 * source rect length: srl
 * dest rect coordinate: drc
 * dest rect length: drl
 * dest clip coordinate: dcc
 * dest clip length: dcl
 * source surface coordinate: sc
 * source surface length: sl
 * dest surface coordinate: dc
 * dest surface length: dl
 */
#define SANITISE(src, srl, drc, drl, dcc, dcl, sc, sl, dc, dl) 	\
if (src < 0) 							\
{ 								\
	drc -= (src * drl) / srl; 				\
	drl += (src * drl) / srl; 				\
	srl += src; 						\
	src = 0; 						\
} 								\
if (src >= sl) return; 						\
if ((src + srl) > sl) 						\
{ 								\
	drl = (drl * (sl - src)) /  (srl); 			\
	srl = sl - src; 					\
} 								\
if (drl <= 0) return; 						\
if (srl <= 0) return; 						\
if (dcc < 0) 							\
{ 								\
	dcl += dcc; 						\
	dcc = 0; 						\
} 								\
if (dcl <= 0) return; 						\
if (dcc >= dl) return; 						\
if (dcc < drc) 							\
{ 								\
	dcl += (dcc - drc); 					\
	dcc = drc; 						\
} 								\
if ((dcc + dcl) > dl) 						\
{ 								\
	dcl = dl - dcc; 					\
} 								\
if (dcl <= 0) return;

#if 0
#ifdef BUILD_SCALE_SAMPLE
static void _sampled_func(Emage_Surface *src, Emage_Surface *dst, Emage_Rectangle srect, Emage_Rectangle drect, Emage_Draw_Context *dc)
{
   int      x, y;
   int     *lin_ptr;
   DATA32  *buf, *dptr;
   DATA32 **row_ptr;
   DATA32  *ptr, *dst_ptr, *src_data, *dst_data;
   int      dst_jump;
   int      dst_clip_x, dst_clip_y, dst_clip_w, dst_clip_h;
   int      src_w, src_h, dst_w, dst_h;
   Emage_Sl_Func func;

   if (!(RECTS_INTERSECT(drect.x, drect.y, drect.w, drect.h, 0, 0, dst->w, dst->h)))
     return;
   if (!(RECTS_INTERSECT(srect.x, srect.y, srect.w, srect.h, 0, 0, src->w, src->h)))
     return;

   src_w = src->w;
   src_h = src->h;
   dst_w = dst->w;
   dst_h = dst->h;

   src_data = src->data;
   dst_data = dst->data;

   if (dc->clip.use)
     {
	dst_clip_x = dc->clip.r.x;
	dst_clip_y = dc->clip.r.y;
	dst_clip_w = dc->clip.r.w;
	dst_clip_h = dc->clip.r.h;
	if (dst_clip_x < 0)
	  {
	     dst_clip_w += dst_clip_x;
	     dst_clip_x = 0;
	  }
	if (dst_clip_y < 0)
	  {
	     dst_clip_h += dst_clip_y;
	     dst_clip_y = 0;
	  }
	if ((dst_clip_x + dst_clip_w) > dst_w)
	  dst_clip_w = dst_w - dst_clip_x;
	if ((dst_clip_y + dst_clip_h) > dst_h)
	  dst_clip_h = dst_h - dst_clip_y;
     }
   else
     {
	dst_clip_x = 0;
	dst_clip_y = 0;
	dst_clip_w = dst_w;
	dst_clip_h = dst_h;
     }

   if (dst_clip_x < drect.x)
     {
	dst_clip_w += dst_clip_x - drect.x;
	dst_clip_x = drect.x;
     }
   if ((dst_clip_x + dst_clip_w) > (drect.x + drect.w))
     dst_clip_w = drect.x + drect.w - dst_clip_x;
   if (dst_clip_y < drect.y)
     {
	dst_clip_h += dst_clip_y - drect.y;
	dst_clip_y = drect.y;
     }
   if ((dst_clip_y + dst_clip_h) > (drect.y + drect.h))
     dst_clip_h = drect.y + drect.h - dst_clip_y;

   if ((srect.w <= 0) || (srect.h <= 0) ||
       (drect.w <= 0) || (drect.h <= 0) ||
       (dst_clip_w <= 0) || (dst_clip_h <= 0))
     return;

   /* sanitise x and y */
   SANITISE(srect.x, srect.w, drect.x, drect.w, dst_clip_x, 
   		dst_clip_w, src->x, src->w, dst->x, dst->w)
   SANITISE(srect.y, srect.h, drect.y, drect.h, dst_clip_y,
   		dst_clip_h, src->y, src->h, dst->y, dst->h)

   /* allocate scale lookup tables */
   lin_ptr = alloca(dst_clip_w * sizeof(int));
   row_ptr = alloca(dst_clip_h * sizeof(DATA32 *));

   /* figure out dst jump */
   dst_jump = dst_w - dst_clip_w;

   /* figure out dest start ptr */
   dst_ptr = dst_data + dst_clip_x + (dst_clip_y * dst_w);

/* HOW TO FIX THIS??? put on the compositor the switch between pixel_color or 
 * pixel only based on the drawing context?
 */
#if 0
   if (dc->mul.use)
     func = emage_compositor_sl_pixel_color_get(dc, src, dst);
     //func = evas_common_gfx_func_composite_pixel_color_span_get(src, dc->mul.col, dst, dst_clip_w, dc->render_op);
   else
     func = evas_common_gfx_func_composite_pixel_span_get(src, dst, dst_clip_w, dc->render_op);
#endif

	/* both surface have the same size */
   if ((drect.w == srect.w) && (drect.h == srect.h))
	//_sampled_same_size
	
     {
	ptr = src_data + ((dst_clip_y - drect.y + srect.y) * src_w) + (dst_clip_x - drect.x) + srect.x;
	for (y = 0; y < dst_clip_h; y++)
	  {
	    /* * blend here [clip_w *] ptr -> dst_ptr * */
#ifdef EVAS_SLI
	     if (((y + dst_clip_y) % dc->sli.h) == dc->sli.y)
#endif
	       {
		  //func(ptr, NULL, dc->mul.col, dst_ptr, dst_clip_w);
	       }
	    ptr += src_w;
	    dst_ptr += dst_w;
	  }
     }
   else
     {
       /* fill scale tables */
	for (x = 0; x < dst_clip_w; x++)
	    lin_ptr[x] = (((x + dst_clip_x - drect.x) * srect.w) / drect.w) + srect.x;
	for (y = 0; y < dst_clip_h; y++)
	    row_ptr[y] = src_data + (((((y + dst_clip_y - drect.y) * srect.h) / drect.h)
			+ srect.y) * src_w);
	/* scale to dst */
	dptr = dst_ptr;
#ifdef DIRECT_SCALE	     
	if ((!(src->flags & RGBA_IMAGE_HAS_ALPHA)) &&
	     (!(dst->flags & RGBA_IMAGE_HAS_ALPHA)) &&
	     (!dc->mul.use))
	  {
	     for (y = 0; y < dst_clip_h; y++)
	       {
#ifdef EVAS_SLI
		 if (((y + dst_clip_y) % dc->sli.h) == dc->sli.y)
#endif
		   {
		      dst_ptr = dptr;
		      for (x = 0; x < dst_clip_w; x++)
			{
			   ptr = row_ptr[y] + lin_ptr[x];
			   *dst_ptr = *ptr;
			   dst_ptr++;
			}
		   }
		 dptr += dst_w;
	      }
	  }
	else
#endif	       
	  {
	    /* a scanline buffer */
	    buf = alloca(dst_clip_w * sizeof(DATA32));
	    for (y = 0; y < dst_clip_h; y++)
	      {
#ifdef EVAS_SLI
		 if (((y + dst_clip_y) % dc->sli.h) == dc->sli.y)
#endif
		   {
		      dst_ptr = buf;
		      for (x = 0; x < dst_clip_w; x++)
			{
			   ptr = row_ptr[y] + lin_ptr[x];
			   *dst_ptr = *ptr;
			   dst_ptr++;
			}
		      /* * blend here [clip_w *] buf -> dptr * */
		      //func(buf, NULL, dc->mul.col, dptr, dst_clip_w);
		   }
		dptr += dst_w;
	      }
	  }
     }
}
#endif
#endif

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
void emage_scaler_sampled_init(void)
{
#if 0
#ifdef BUILD_SCALE_SAMPLE
	Emage_Scalers[EMAGE_SCALER_SAMPLED].func = _sampled_func;
#endif
#endif
}
