#ifndef _EMAGE_PRIVATE_H
#define _EMAGE_PRIVATE_H

/**
 * @file
 * @brief Internal Implementation
 * @defgroup Internal_Group Internal Implementation
 * @todo
 * - Port scale code
 * - Port blit code
 * - Port convert code
 * @{
 */

#include "config.h"
#include "debug.h"
#include "cpu.h"
#include "surface.h"
#include "context.h"
#include "compositor.h"
#include "scaler.h"
#include "data.h"
#include "scanline.h"
#include "converter.h"

/* some useful constants */

extern const DATA32 ALPHA_255;
extern const DATA32 ALPHA_256;


/* SOME COMMON CODE FOUND EVERYWHERE */

/* if the destination rectangle doesnt cut a surface, return */
#define HANDLE_CUTOUTS(r, s) 						\
if ((r.w <= 0) || (r.h <= 0)) return;					\
if (!(RECTS_INTERSECT(r.x, r.y, r.w, r.h, 0, 0, s->w, s->h)))		\
	return;



/* TODO
 * most of the code has a "save clip info and restore it after" put 
 * those on a function!
 * 
 *
 */

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/


void evas_common_convert_yuv_420p_601_rgba                     (DATA8 **src, DATA8 *dst, int w, int h);

EAPI void evas_common_convert_argb_premul                          (DATA32 *src, unsigned int len);
EAPI void evas_common_convert_argb_unpremul                        (DATA32 *src, unsigned int len);
EAPI void evas_common_convert_color_argb_premul                    (int a, int *r, int *g, int *b);
EAPI void evas_common_convert_color_argb_unpremul                  (int a, int *r, int *g, int *b);

EAPI void evas_common_convert_color_hsv_to_rgb                     (float h, float s, float v, int *r, int *g, int *b);
EAPI void evas_common_convert_color_rgb_to_hsv                     (int r, int g, int b, float *h, float *s, float *v);
EAPI void evas_common_convert_color_hsv_to_rgb_int                 (int h, int s, int v, int *r, int *g, int *b);
EAPI void evas_common_convert_color_rgb_to_hsv_int                 (int r, int g, int b, int *h, int *s, int *v);

#ifdef BUILD_SMALL_DITHER_MASK
# define DM_TABLE     _evas_dither_44
# define DM_SIZE      4
# define DM_BITS      4
# define DM_DIV       16
# define USE_DITHER_44 1
# define DM_MSK       (DM_SIZE - 1)
# define DM_SHF(_b)   (DM_BITS - (8 - _b))
#else
# define DM_TABLE     _evas_dither_128128
# define DM_SIZE      128
# define DM_BITS      6
# define DM_DIV       64
# define USE_DITHER_128128 1
# define DM_MSK       (DM_SIZE - 1)
# define DM_SHF(_b)   (DM_BITS - (8 - _b))
#endif

#define CONVERT_LOOP_START_ROT_0() \
   src_ptr = src; \
   for (y = 0; y < h; y++) \
     { \
	for (x = 0; x < w; x++) \
	  {

#define CONVERT_LOOP_END_ROT_0() \
             dst_ptr++; \
             src_ptr++; \
          } \
        src_ptr += src_jump; \
        dst_ptr += dst_jump; \
     }

#define CONVERT_LOOP_START_ROT_180() \
   src_ptr = src + (w - 1) + ((h - 1) * (w + src_jump)); \
   for (y = 0; y < h; y++) \
     { \
        for (x = 0; x < w; x++) \
          {

#define CONVERT_LOOP_END_ROT_180() \
             dst_ptr++; \
             src_ptr--; \
          } \
        src_ptr = src + (w - 1) + ((h - y - 2) * (w + src_jump)); \
        dst_ptr += dst_jump; \
     }

#define CONVERT_LOOP_START_ROT_270() \
   src_ptr = src + ((w - 1) * (h + src_jump)); \
   for (y = 0; y < h; y++) \
     { \
	for (x = 0; x < w; x++) \
	  {

#define CONVERT_LOOP_END_ROT_270() \
             dst_ptr++; \
             src_ptr -= (h + src_jump); \
          } \
        src_ptr = src + ((w - 1) * (h + src_jump)) + (y + 1); \
        dst_ptr += dst_jump; \
     }

#define CONVERT_LOOP_START_ROT_90() \
   src_ptr = src + (h - 1); \
   for (y = 0; y < h; y++) \
     { \
	for (x = 0; x < w; x++) \
	  {

#define CONVERT_LOOP_END_ROT_90() \
             dst_ptr++; \
             src_ptr += (h + src_jump); \
          } \
        src_ptr = src + (h - 1) - y - 1; \
        dst_ptr += dst_jump; \
     }

#define CONVERT_LOOP2_START_ROT_0() \
   src_ptr = src; \
   for (y = 0; y < h; y++) \
     { \
	for (x = 0; x < w; x++) \
	  {

#define CONVERT_LOOP2_INC_ROT_0() \
src_ptr++; \
x++;

#define CONVERT_LOOP2_END_ROT_0() \
             dst_ptr+=2; \
             src_ptr++; \
          } \
        src_ptr += src_jump; \
        dst_ptr += dst_jump; \
     }

#define CONVERT_LOOP2_START_ROT_180() \
   src_ptr = src + (w - 1) + ((h - 1) * (w + src_jump)); \
   for (y = 0; y < h; y++) \
     { \
        for (x = 0; x < w; x++) \
          {

#define CONVERT_LOOP2_INC_ROT_180() \
src_ptr--; \
x++;

#define CONVERT_LOOP2_END_ROT_180() \
             dst_ptr+=2; \
             src_ptr--; \
          } \
        src_ptr = src + (w - 1) + ((h - y - 2) * (w + src_jump)); \
        dst_ptr += dst_jump; \
     }

#define CONVERT_LOOP2_START_ROT_270() \
   src_ptr = src + ((w - 1) * (h + src_jump)); \
   for (y = 0; y < h; y++) \
     { \
	for (x = 0; x < w; x++) \
	  {

#define CONVERT_LOOP2_INC_ROT_270() \
src_ptr -= (h + src_jump); \
x++;

#define CONVERT_LOOP2_END_ROT_270() \
             dst_ptr+=2; \
             src_ptr -= (h + src_jump); \
          } \
        src_ptr = src + ((w - 1) * (h + src_jump)) + (y + 1); \
        dst_ptr += dst_jump; \
     }

#define CONVERT_LOOP2_START_ROT_90() \
   src_ptr = src + (h - 1); \
   for (y = 0; y < h; y++) \
     { \
	for (x = 0; x < w; x++) \
	  {

#define CONVERT_LOOP2_INC_ROT_90() \
src_ptr += (h + src_jump); \
x++;

#define CONVERT_LOOP2_END_ROT_90() \
             dst_ptr+=2; \
             src_ptr += (h + src_jump); \
          } \
        src_ptr = src + (h - 1) - y - 1; \
        dst_ptr += dst_jump; \
     }

/*============================================================================*
 *                                 Macros                                     * 
 *============================================================================*/
/* FIXME, place the above macros in other place */
#define RGB_565_UNPACKED_MASK 0x07e0f81f
#define RGB_565_UNPACK(rgb)                                             \
   (((rgb) | ((rgb) << 16)) & RGB_565_UNPACKED_MASK)
#define RGB_565_PACK(rgb)                                               \
  ((((rgb) & RGB_565_UNPACKED_MASK) |                                   \
   ((rgb) & RGB_565_UNPACKED_MASK) >> 16) & 0xffff)
#define RGB_565_UNPACKED_BLEND(a, b, alpha)                             \
   ((b) + ((((a) - (b)) * (alpha)) >> 5))

#define RGB_565_FROM_COMPONENTS(r, g, b)                                \
  (((((r) >> 3) & 0x1f) << 11) |                                        \
   ((((g) >> 2) & 0x3f) << 5) |                                         \
   (((b) >> 3) & 0x1f))



#define UNROLL2(op...) op op
#define UNROLL4(op...) UNROLL2(op) UNROLL2(op)
#define UNROLL8(op...) UNROLL4(op) UNROLL4(op)
#define UNROLL16(op...) UNROLL8(op) UNROLL8(op)


#define RGB_JOIN(r,g,b) \
        (((r) << 16) + ((g) << 8) + (b))

#define ARGB_JOIN(a,r,g,b) \
        (((a) << 24) + ((r) << 16) + ((g) << 8) + (b))

#undef MIN
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#undef MAX
#define MAX(x, y) (((x) > (y)) ? (x) : (y))

#define SPANS_COMMON(x1, w1, x2, w2) \
(!((((x2) + (w2)) <= (x1)) || ((x2) >= ((x1) + (w1)))))

#define IN_RECT(x, y, rx, ry, rw, rh) \
( ((unsigned)((x) - (rx)) < (rw)) && ((unsigned)((y) - (ry)) < (rh)) )

#define RECTS_INTERSECT(x, y, w, h, xx, yy, ww, hh) \
((SPANS_COMMON((x), (w), (xx), (ww))) && (SPANS_COMMON((y), (h), (yy), (hh))))

#define RECTS_CLIP_TO_RECT(_x, _y, _w, _h, _cx, _cy, _cw, _ch) \
{ \
   if (RECTS_INTERSECT(_x, _y, _w, _h, _cx, _cy, _cw, _ch)) \
     { \
	if (_x < (_cx)) \
	  { \
	     _w += _x - (_cx); \
	     _x = (_cx); \
	     if (_w < 0) _w = 0; \
	  } \
	if ((_x + _w) > ((_cx) + (_cw))) \
	  _w = (_cx) + (_cw) - _x; \
	if (_y < (_cy)) \
	  { \
	     _h += _y - (_cy); \
	     _y = (_cy); \
	     if (_h < 0) _h = 0; \
	  } \
	if ((_y + _h) > ((_cy) + (_ch))) \
	  _h = (_cy) + (_ch) - _y; \
     } \
   else \
     { \
	_w = 0; _h = 0; \
     } \
}

/* some useful C macros */

#define MUL4_256(a, r, g, b, c) \
 ( (((((c) >> 8) & 0xff0000) * (a)) & 0xff000000) + \
   (((((c) & 0xff0000) * (r)) >> 8) & 0xff0000) + \
   (((((c) & 0xff00) * (g)) >> 8) & 0xff00) + \
   ((((c) & 0xff) * (b)) >> 8) )

#define MUL3_256(r, g, b, c) \
 ( (((((c) & 0xff0000) * (r)) >> 8) & 0xff0000) + \
   (((((c) & 0xff00) * (g)) >> 8) & 0xff00) + \
   ((((c) & 0xff) * (b)) >> 8) )

#define MUL_256(a, c) \
 ( (((((c) >> 8) & 0x00ff00ff) * (a)) & 0xff00ff00) + \
   (((((c) & 0x00ff00ff) * (a)) >> 8) & 0x00ff00ff) )

#define MUL4_SYM(x, y) \
 ( ((((((x) >> 16) & 0xff00) * (((y) >> 16) & 0xff00)) + 0xff0000) & 0xff000000) + \
   ((((((x) >> 8) & 0xff00) * (((y) >> 16) & 0xff)) + 0xff00) & 0xff0000) + \
   ((((((x) & 0xff00) * ((y) & 0xff00)) + 0xff00) >> 16) & 0xff00) + \
   (((((x) & 0xff) * ((y) & 0xff)) + 0xff) >> 8) )

#define MUL3_SYM(x, y) \
 ( ((((((x) >> 8) & 0xff00) * (((y) >> 16) & 0xff)) + 0xff00) & 0xff0000) + \
   ((((((x) & 0xff00) * ((y) & 0xff00)) + 0xff00) >> 16) & 0xff00) + \
   (((((x) & 0xff) * ((y) & 0xff)) + 0xff) >> 8) )

#define MUL_SYM(a, x) \
 ( (((((x) >> 8) & 0x00ff00ff) * (a) + 0xff00ff) & 0xff00ff00) + \
   (((((x) & 0x00ff00ff) * (a) + 0xff00ff) >> 8) & 0x00ff00ff) )

#define MUL_A_256(a, c) \
 ( ((((c) >> 8) & 0x00ff0000) * (a)) & 0xff000000 )

#define MUL_A_SYM(a, c) \
 ( (((((c) >> 8) & 0x00ff0000) * (a)) + 0x00ff0000) & 0xff000000 )

#define INTERP_256(a, c0, c1) \
 ( (((((((c0) >> 8) & 0xff00ff) - (((c1) >> 8) & 0xff00ff)) * (a)) \
   + ((c1) & 0xff00ff00)) & 0xff00ff00) + \
   (((((((c0) & 0xff00ff) - ((c1) & 0xff00ff)) * (a)) >> 8) \
   + ((c1) & 0xff00ff)) & 0xff00ff) )

#define INTERP_RGB_256(a, c0, c1) \
 ( (((((((c0) >> 8) & 0xff) - (((c1) >> 8) & 0xff)) * (a)) \
   + ((c1) & 0xff00)) & 0xff00) + \
   (((((((c0) & 0xff00ff) - ((c1) & 0xff00ff)) * (a)) >> 8) \
   + ((c1) & 0xff00ff)) & 0xff00ff) )

#define INTERP_A_256(a, c0, c1) \
 ( (((((((c0) >> 8) & 0xff0000) - (((c1) >> 8) & 0xff0000)) * (a)) \
   + ((c1) & 0xff000000)) & 0xff000000) )

/* some useful MMX macros */

#ifdef BUILD_MMX
#define MOV_A2R(a, mma) \
	movd_m2r(a, mma); \
	punpcklwd_r2r(mma, mma); \
	punpckldq_r2r(mma, mma);

#define MOV_P2R(c, mmc, mmz) \
	movd_m2r(c, mmc); \
	punpcklbw_r2r(mmz, mmc);

#define MOV_R2P(mmc, c, mmz) \
	packuswb_r2r(mmz, mmc); \
	movd_r2m(mmc, c);

#define MUL4_256_R2R(mmx, mmy) \
	pmullw_r2r(mmx, mmy); \
	psrlw_i2r(8, mmy);

#define MUL4_SYM_R2R(mmx, mmy, mm255) \
	pmullw_r2r(mmx, mmy); \
	paddw_r2r(mm255, mmy); \
	psrlw_i2r(8, mmy);

#define MOV_RA2R(mmx, mma) \
	movq_r2r(mmx, mma); \
	punpckhwd_r2r(mma, mma); \
	punpckhdq_r2r(mma, mma);

#define MOV_PA2R(c, mma) \
	movd_m2r(c, mma); \
	punpcklbw_r2r(mma, mma); \
	punpckhwd_r2r(mma, mma); \
	punpckhdq_r2r(mma, mma);

#define INTERP_256_R2R(mma, mmx, mmy, mm255) \
	psubw_r2r(mmy, mmx); \
	pmullw_r2r(mma, mmx); \
	psrlw_i2r(8, mmx); \
	paddw_r2r(mmx, mmy); \
	pand_r2r(mm255, mmy);

#endif

/** @} */

#endif
