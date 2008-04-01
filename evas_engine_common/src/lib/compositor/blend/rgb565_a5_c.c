#include "Emage.h"
#include "emage_private.h"

#include "main.h"

#ifdef BUILD_C
static inline void
_pt_pixel_argb_rgb(DATA16 *p_dst, DATA16 src, DATA8 alpha)
{
	if (alpha == 31) *p_dst = src;
	else if (alpha != 0)
	{
		DATA32 a, b;
		a = RGB_565_UNPACK(src);
		b = RGB_565_UNPACK(*p_dst);
		b = RGB_565_UNPACKED_BLEND(a, b, alpha);
		*p_dst = RGB_565_PACK(b);
	}
}

static inline void
_sl_pixel_rgb_rgb(DATA16 *src, DATA16 *dst, int size)
{
	memcpy(dst, src, size * sizeof(DATA16));
}

static inline void
_sl_pixel_argb_rgb(DATA16 *src, DATA8 *alpha, DATA16 *dst, int size)
{
	DATA16 *start, *end;

	start = dst;
	end = start + (size & ~7);

	pld(alpha, 0);
	pld(src, 0);

	/* work on 8 pixels per time, do data preload */
	while (start < end)
	{
		DATA8 alpha1, alpha2;

		alpha1 = alpha[0];
		alpha += 8;

		/* empirical tests show these give the best performance */
		pld(alpha, 8);
		pld(src, 32);

		src += 8;
		start += 8;

		alpha2 = alpha[-7];
		_pt_pixel_argb_rgb(start - 8, src[-8], alpha1);

		alpha1 = alpha[-6];
		_pt_pixel_argb_rgb(start - 7, src[-7], alpha2);

		alpha2 = alpha[-5];
		_pt_pixel_argb_rgb(start - 6, src[-6], alpha1);

		alpha1 = alpha[-4];
		_pt_pixel_argb_rgb(start - 5, src[-5], alpha2);

		alpha2 = alpha[-3];
		_pt_pixel_argb_rgb(start - 4, src[-4], alpha1);

		alpha1 = alpha[-2];
		_pt_pixel_argb_rgb(start - 3, src[-3], alpha2);

		alpha2 = alpha[-1];
		_pt_pixel_argb_rgb(start - 2, src[-2], alpha1);

		_pt_pixel_argb_rgb(start - 1, src[-1], alpha2);
	}

	/* remaining pixels (up to 7) */
	end = start + (size & 7);
	for (; start < end; start++, src++, alpha++)
		_pt_pixel_argb_rgb(start, *src, *alpha);
}

static inline void
_pt_pixel_rgb_rgb(DATA16 *p_dst, DATA16 src)
{
	*p_dst = src;
}


SL_FUNC(blend, rgb565_a5, c, pixel)
{
	DATA16 *s, *d;
	DATA8 *sa, *da;

	d = dst->data.rgb565_a5.data + doffset;
	da = dst->data.rgb565_a5.alpha + doffset;
	s = src->data.rgb565_a5.data + soffset;
	sa = src->data.rgb565_a5.alpha + soffset;
	
	if ((src->flags & SURFACE_FLAG_HAS_ALPHA) && 
		(!(dst->flags & SURFACE_FLAG_HAS_ALPHA)))
		_sl_pixel_argb_rgb(s, sa, d, len);

	else if (!(src->flags & SURFACE_FLAG_HAS_ALPHA) && 
		(!(dst->flags & SURFACE_FLAG_HAS_ALPHA)))
		_sl_pixel_rgb_rgb(s, d, len);
	else
	{
		PRINTF("Not Implemented\n");
	}
}

SL_FUNC(blend, rgb565_a5, c, color)
{

}

SL_FUNC(blend, rgb565_a5, c, pixel_color)
{

}

SL_FUNC(blend, rgb565_a5, c, mask_color)
{

}

SL_FUNC(blend, rgb565_a5, c, pixel_mask)
{

}

PT_FUNC(blend, rgb565_a5, c, pixel)
{
	DATA16 *s, *d;
	
	d = dst->data.rgb565_a5.data + doffset;
	s = src->data.rgb565_a5.data + soffset;

	if (!(src->flags & SURFACE_FLAG_HAS_ALPHA) && 
		(!(dst->flags & SURFACE_FLAG_HAS_ALPHA)))
		_pt_pixel_rgb_rgb(s, d);
	else
	{
		PRINTF("Not Implemented\n");
	}
}

PT_FUNC(blend, rgb565_a5, c, color)
{
}

PT_FUNC(blend, rgb565_a5, c, pixel_color)
{
}

PT_FUNC(blend, rgb565_a5, c, mask_color)
{
}

PT_FUNC(blend, rgb565_a5, c, pixel_mask)
{
}

#else

/*
#define rgb565_a5_c_sl_pixel 		dummy_sl
#define rgb565_a5_c_sl_color 		dummy_sl
#define rgb565_a5_c_sl_pixel_color 	dummy_sl
#define rgb565_a5_c_sl_mask_color 	dummy_sl
#define rgb565_a5_c_sl_pixel_mask 	dummy_sl
#define rgb565_a5_c_pt_color 		dummy_pt
#define rgb565_a5_c_pt_pixel_color 	dummy_pt
#define rgb565_a5_c_pt_mask_color 	dummy_pt
#define rgb565_a5_c_pt_pixel_mask 	dummy_pt
*/

#endif

#if 0
/** NOTE: This file is meant to be included by users **/

/** NOTE2: r, g, b parameters are 16bits, so you can pass 0 to 256 inclusive.
 **        this is due our division by 256 when multiplying the color.
 **/

/*****************************************************************************
 * Scanline processing
 *
 *    _soft16_scanline_<description>_<src>_<dst>[_<modifier>]()
 *
 ****************************************************************************/

/* DONE */
static inline void
_soft16_pt_blend_transp_solid(DATA16 *p_dst, DATA16 src, DATA8 alpha)
{
}

/***********************************************************************
 * Regular blend operations
 */
/* DONE */
static inline void
_soft16_scanline_blend_transp_solid(DATA16 *src, DATA8 *alpha, DATA16 *dst, int size)
{
}

/* DONE */
static inline void
_soft16_pt_blend_solid_solid(DATA16 *p_dst, DATA16 src)
{
   *p_dst = src;
}

/* DONE */
static inline void
_soft16_scanline_blend_solid_solid(DATA16 *src, DATA16 *dst, int size)
{
}

/***********************************************************************
 * Blend operations taking an extra alpha (fade in, out)
 */

static inline void
_soft16_pt_blend_transp_solid_mul_alpha(DATA16 *p_dst, DATA16 src, DATA8 alpha, DATA8 rel_alpha)
{
   /* rel_alpha is always > 0, so (alpha - rel_alpha) is always < 31 */
   if (alpha > rel_alpha)
     {
        DATA32 a, b;
        a = RGB_565_UNPACK(src);
        b = RGB_565_UNPACK(*p_dst);
        b = RGB_565_UNPACKED_BLEND(a, b, alpha - rel_alpha);
        *p_dst = RGB_565_PACK(b);
     }
}

static inline void
_soft16_scanline_blend_transp_solid_mul_alpha(DATA16 *src, DATA8 *alpha, DATA16 *dst, int size, const DATA8 rel_alpha)
{
   DATA16 *start, *end;

   start = dst;
   end = start + (size & ~7);

   pld(alpha, 0);
   pld(src, 0);

   while (start < end)
     {
	DATA8 alpha1, alpha2;

	alpha1 = alpha[0];
	alpha += 8;

	pld(alpha, 8);
	pld(src, 32);

	src += 8;
	start += 8;

	alpha2 = alpha[-7];
	_soft16_pt_blend_transp_solid_mul_alpha
           (start - 8, src[-8], alpha1, rel_alpha);

	alpha1 = alpha[-6];
	_soft16_pt_blend_transp_solid_mul_alpha
           (start - 7, src[-7], alpha2, rel_alpha);

	alpha2 = alpha[-5];
	_soft16_pt_blend_transp_solid_mul_alpha
           (start - 6, src[-6], alpha1, rel_alpha);

	alpha1 = alpha[-4];
	_soft16_pt_blend_transp_solid_mul_alpha
           (start - 5, src[-5], alpha2, rel_alpha);

	alpha2 = alpha[-3];
	_soft16_pt_blend_transp_solid_mul_alpha
           (start - 4, src[-4], alpha1, rel_alpha);

	alpha1 = alpha[-2];
	_soft16_pt_blend_transp_solid_mul_alpha
           (start - 3, src[-3], alpha2, rel_alpha);

	alpha2 = alpha[-1];
	_soft16_pt_blend_transp_solid_mul_alpha
           (start - 2, src[-2], alpha1, rel_alpha);

	_soft16_pt_blend_transp_solid_mul_alpha
           (start - 1, src[-1], alpha2, rel_alpha);
     }

   end = start + (size & 7);
   for (; start < end; start++, src++, alpha++)
      _soft16_pt_blend_transp_solid_mul_alpha(start, *src, *alpha, rel_alpha);
}

static inline void
_soft16_pt_blend_solid_solid_mul_alpha(DATA16 *p_dst, DATA16 src, DATA8 rel_alpha)
{
   DATA32 a, b;
   a = RGB_565_UNPACK(src);
   b = RGB_565_UNPACK(*p_dst);
   b = RGB_565_UNPACKED_BLEND(a, b, rel_alpha);
   *p_dst = RGB_565_PACK(b);
}

static inline void
_soft16_scanline_blend_solid_solid_mul_alpha(DATA16 *src, DATA16 *dst, int size, DATA8 rel_alpha)
{
   DATA16 *start, *end;

   start = dst;
   end = start + (size & ~7);

   pld(src, 0);

   while (start < end)
     {
	pld(src, 32);
        UNROLL8({
           _soft16_pt_blend_solid_solid_mul_alpha(start, *src, rel_alpha);
           start++;
           src++;
        });
     }

   end = start + (size & 7);
   for (; start < end; start++, src++)
     _soft16_pt_blend_solid_solid_mul_alpha(start, *src, rel_alpha);
}

/***********************************************************************
 * Blend operations with extra alpha and multiply color
 */

static inline void
_soft16_pt_blend_transp_solid_mul_color_transp(DATA16 *p_dst, DATA16 src, DATA8 alpha, DATA8 rel_alpha, DATA16 r, DATA16 g, DATA16 b)
{
   /* rel_alpha is always > 0, so (alpha - rel_alpha) is always < 31 */
   if (alpha > rel_alpha)
     {
        int r1, g1, b1;
        DATA32 rgb, d;
        r1 = ((((src) >> 11) & 0x1f) * r) >> 8;
        g1 = ((((src) >> 5) & 0x3f) * g) >> 8;
        b1 = (((src) & 0x1f) * b) >> 8;
        rgb = ((r1 << 11) | (g1 << 21) | b1) & RGB_565_UNPACKED_MASK;
        d = RGB_565_UNPACK(*p_dst);
        d = RGB_565_UNPACKED_BLEND(rgb, d, alpha - rel_alpha);
        *p_dst = RGB_565_PACK(d);
     }
}

static inline void
_soft16_scanline_blend_transp_solid_mul_color_transp(DATA16 *src, DATA8 *alpha, DATA16 *dst, int size, DATA8 rel_alpha, DATA16 r, DATA16 g, DATA16 b)
{
   DATA16 *start, *end;

   start = dst;
   end = start + (size & ~7);

   pld(alpha, 0);
   pld(src, 0);

   while (start < end)
     {
	DATA8 alpha1, alpha2;

	alpha1 = alpha[0];
	alpha += 8;

        pld(src, 32);
        pld(start, 32);

	src += 8;
	start += 8;

	alpha2 = alpha[-7];
        _soft16_pt_blend_transp_solid_mul_color_transp
           (start - 8, src[-8], alpha1, rel_alpha, r, g, b);

	alpha1 = alpha[-6];
        _soft16_pt_blend_transp_solid_mul_color_transp
           (start - 7, src[-7], alpha2, rel_alpha, r, g, b);

	alpha2 = alpha[-5];
        _soft16_pt_blend_transp_solid_mul_color_transp
           (start - 6, src[-6], alpha1, rel_alpha, r, g, b);

	alpha1 = alpha[-4];
        _soft16_pt_blend_transp_solid_mul_color_transp
           (start - 5, src[-5], alpha2, rel_alpha, r, g, b);

	alpha2 = alpha[-3];
        _soft16_pt_blend_transp_solid_mul_color_transp
           (start - 4, src[-4], alpha1, rel_alpha, r, g, b);

	alpha1 = alpha[-2];
        _soft16_pt_blend_transp_solid_mul_color_transp
           (start - 3, src[-3], alpha2, rel_alpha, r, g, b);

	alpha2 = alpha[-1];
        _soft16_pt_blend_transp_solid_mul_color_transp
           (start - 2, src[-2], alpha1, rel_alpha, r, g, b);

        _soft16_pt_blend_transp_solid_mul_color_transp
           (start - 1, src[-1], alpha2, rel_alpha, r, g, b);
     }

   end = start + (size & 7);
   for (; start < end; start++, src++, alpha++)
      _soft16_pt_blend_transp_solid_mul_color_transp
         (start, *src, *alpha, rel_alpha, r, g, b);
}

static inline void
_soft16_pt_blend_solid_solid_mul_color_transp(DATA16 *p_dst, DATA16 src, DATA8 rel_alpha, DATA16 r, DATA16 g, DATA16 b)
{
   int r1, g1, b1;
   DATA32 rgb, d;

   r1 = ((((src) >> 11) & 0x1f) * r) >> 8;
   g1 = ((((src) >> 5) & 0x3f) * g) >> 8;
   b1 = (((src) & 0x1f) * b) >> 8;

   rgb = ((r1 << 11) | (g1 << 21) | b1) & RGB_565_UNPACKED_MASK;
   d = RGB_565_UNPACK(*p_dst);
   d = RGB_565_UNPACKED_BLEND(rgb, d, rel_alpha);
   *p_dst = RGB_565_PACK(d);
}

static inline void
_soft16_scanline_blend_solid_solid_mul_color_transp(DATA16 *src, DATA16 *dst, int size, DATA8 rel_alpha, DATA16 r, DATA16 g, DATA16 b)
{
   DATA16 *start, *end;

   start = dst;
   end = start + (size & ~7);

   pld(src, 0);

   while (start < end)
     {
	pld(src, 32);
        UNROLL8({
           _soft16_pt_blend_solid_solid_mul_color_transp
              (start, *src, rel_alpha, r, g, b);
           start++;
           src++;
        });
     }

   end = start + (size & 7);
   for (; start < end; start++, src++)
      _soft16_pt_blend_solid_solid_mul_color_transp
         (start, *src, rel_alpha, r, g, b);
}

/***********************************************************************
 * Blend operations with extra multiply color
 */
static inline void
_soft16_pt_blend_transp_solid_mul_color_solid(DATA16 *p_dst, DATA16 src, DATA8 alpha, DATA16 r, DATA16 g, DATA16 b)
{
   int r1, g1, b1;

   if (alpha == 0) return;

   r1 = ((((src >> 11) & 0x1f) * r) >> 8) & 0x1f;
   g1 = ((((src >> 5) & 0x3f) * g) >> 8) & 0x3f;
   b1 = (((src & 0x1f) * b) >> 8) & 0x1f;

   if (alpha == 31) *p_dst = (r1 << 11) | (g1 << 5) | b1;
   else
     {
        DATA32 rgb_unpack, d;

        rgb_unpack = ((r1 << 11) | (g1 << 21) | b1) & RGB_565_UNPACKED_MASK;
        d = RGB_565_UNPACK(*p_dst);
        d = RGB_565_UNPACKED_BLEND(rgb_unpack, d, alpha);
        *p_dst = RGB_565_PACK(d);
     }
}

static inline void
_soft16_scanline_blend_transp_solid_mul_color_solid(DATA16 *src, DATA8 *alpha, DATA16 *dst, int size, DATA16 r, DATA16 g, DATA16 b)
{
   DATA16 *start, *end;

   start = dst;
   end = start + (size & ~7);

   pld(alpha, 0);
   pld(src, 0);

   while (start < end)
     {
	DATA8 alpha1, alpha2;

	alpha1 = alpha[0];
	alpha += 8;

	pld(alpha, 8);
	pld(src, 32);

	src += 8;
	start += 8;

	alpha2 = alpha[-7];
        _soft16_pt_blend_transp_solid_mul_color_solid
           (start - 8, src[-8], alpha1, r, g, b);

	alpha1 = alpha[-6];
        _soft16_pt_blend_transp_solid_mul_color_solid
           (start - 7, src[-7], alpha2, r, g, b);

	alpha2 = alpha[-5];
        _soft16_pt_blend_transp_solid_mul_color_solid
           (start - 6, src[-6], alpha1, r, g, b);

	alpha1 = alpha[-4];
        _soft16_pt_blend_transp_solid_mul_color_solid
           (start - 5, src[-5], alpha2, r, g, b);

	alpha2 = alpha[-3];
        _soft16_pt_blend_transp_solid_mul_color_solid
           (start - 4, src[-4], alpha1, r, g, b);

	alpha1 = alpha[-2];
        _soft16_pt_blend_transp_solid_mul_color_solid
           (start - 3, src[-3], alpha2, r, g, b);

	alpha2 = alpha[-1];
        _soft16_pt_blend_transp_solid_mul_color_solid
           (start - 2, src[-2], alpha1, r, g, b);

        _soft16_pt_blend_transp_solid_mul_color_solid
           (start - 1, src[-1], alpha2, r, g, b);
     }

   end = start + (size & 7);
   for (; start < end; start++, src++, alpha++)
     _soft16_pt_blend_transp_solid_mul_color_solid
        (start, *src, *alpha, r, g, b);
}

static inline void
_soft16_pt_blend_solid_solid_mul_color_solid(DATA16 *p_dst, DATA16 src, DATA16 r, DATA16 g, DATA16 b)
{
   int r1, g1, b1;

   r1 = ((((src >> 11) & 0x1f) * r) >> 8) & 0x1f;
   g1 = ((((src >> 5) & 0x3f) * g) >> 8) & 0x3f;
   b1 = (((src & 0x1f) * b) >> 8) & 0x1f;

   *p_dst = (r1 << 11) | (g1 << 5) | b1;
}

static inline void
_soft16_scanline_blend_solid_solid_mul_color_solid(DATA16 *src, DATA16 *dst, int size, DATA16 r, DATA16 g, DATA16 b)
{
   DATA16 *start, *end;

   start = dst;
   end = start + (size & ~7);

   pld(src, 0);

   while (start < end)
     {
	pld(src, 32);
        UNROLL8({
           _soft16_pt_blend_solid_solid_mul_color_solid(start, *src, r, g, b);
           start++;
           src++;
        });
     }

   end = start + (size & 7);
   for (; start < end; start++, src++)
     _soft16_pt_blend_solid_solid_mul_color_solid(start, *src, r, g, b);
}

static inline void
_soft16_pt_fill_solid_solid(DATA16 *dst, DATA16 rgb565)
{
   *dst = rgb565;
}

static inline void
_soft16_scanline_fill_solid_solid(DATA16 *dst, int size, DATA16 rgb565)
{
   DATA16 *start, *end;
   DATA32 rgb565_double;

   start = dst;
   end = start + (size & ~7);

   rgb565_double = (rgb565 << 16) | rgb565;

   while (start < end)
     {
        DATA32 *p = (DATA32 *)start;

        p[0] = rgb565_double;
        p[1] = rgb565_double;
        p[2] = rgb565_double;
        p[3] = rgb565_double;

        start += 8;
     }

   end = start + (size & 7);
   for (; start < end; start++)
      *start = rgb565;
}

static inline void
_soft16_pt_fill_transp_solid(DATA16 *dst, DATA32 rgb565_unpack, DATA8 alpha)
{
   DATA32 d;

   d = RGB_565_UNPACK(*dst);
   d = RGB_565_UNPACKED_BLEND(rgb565_unpack, d, alpha);
   *dst = RGB_565_PACK(d);
}

static inline void
_soft16_scanline_fill_transp_solid(DATA16 *dst, int size, DATA32 rgb565_unpack, DATA8 alpha)
{
   DATA16 *start, *end;
   DATA32 a;

   start = dst;
   pld(start, 0);
   end = start + (size & ~7);

   while (start < end)
     {
        pld(start, 32);
        UNROLL8({
           _soft16_pt_fill_transp_solid(start, rgb565_unpack, alpha);
           start++;
        });
     }

   end = start + (size & 7);
   for (; start < end; start++)
     _soft16_pt_fill_transp_solid(start, rgb565_unpack, alpha);
}

#endif
