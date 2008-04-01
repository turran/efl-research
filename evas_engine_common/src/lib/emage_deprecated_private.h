#ifndef _EMAGE_DEPRECATED_PRIVATE_H
#define _EMAGE_DEPRECATED_PRIVATE_H

#warning Including a deprecated header

typedef void (*RGBA_Gfx_Func)    (DATA32 *src, DATA8 *mask, DATA32 col, DATA32 *dst, int len);
typedef void (*RGBA_Gfx_Pt_Func) (DATA32 src, DATA8 mask, DATA32 col, DATA32 *dst);

typedef void (*Gfx_Func_Copy)    (DATA32 *src, DATA32 *dst, int len);

typedef void (*Gfx_Func_Convert) (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal);


typedef struct _RGBA_Gfx_Compositor   RGBA_Gfx_Compositor;
struct _RGBA_Gfx_Compositor
{
   const char *name;

   void              (*init)(void);
   void              (*shutdown)(void);

   RGBA_Gfx_Func  (*composite_pixel_span_get)(Emage_Surface *src, Emage_Surface *dst, int pixels);
   RGBA_Gfx_Func  (*composite_color_span_get)(DATA32 col, Emage_Surface *dst, int pixels);
   RGBA_Gfx_Func  (*composite_pixel_color_span_get)(Emage_Surface *src, DATA32 col, Emage_Surface *dst, int pixels);
   RGBA_Gfx_Func  (*composite_mask_color_span_get)(DATA32 col, Emage_Surface *dst, int pixels);
   RGBA_Gfx_Func  (*composite_pixel_mask_span_get)(Emage_Surface *src, Emage_Surface *dst, int pixels);

   RGBA_Gfx_Pt_Func  (*composite_pixel_pt_get)(int src_flags, Emage_Surface *dst);
   RGBA_Gfx_Pt_Func  (*composite_color_pt_get)(DATA32 col, Emage_Surface *dst);
   RGBA_Gfx_Pt_Func  (*composite_pixel_color_pt_get)(int src_flags, DATA32 col, Emage_Surface *dst);
   RGBA_Gfx_Pt_Func  (*composite_mask_color_pt_get)(DATA32 col, Emage_Surface *dst);
   RGBA_Gfx_Pt_Func  (*composite_pixel_mask_pt_get)(int src_flags, Emage_Surface *dst);
};

EAPI void evas_common_blend_init                        (void);

RGBA_Gfx_Compositor   *evas_common_gfx_compositor_blend_get(void);
RGBA_Gfx_Compositor   *evas_common_gfx_compositor_blend_rel_get(void);
RGBA_Gfx_Compositor   *evas_common_gfx_compositor_copy_get(void);
RGBA_Gfx_Compositor   *evas_common_gfx_compositor_copy_rel_get(void);
RGBA_Gfx_Compositor   *evas_common_gfx_compositor_add_get(void);
RGBA_Gfx_Compositor   *evas_common_gfx_compositor_add_rel_get(void);
/*
RGBA_Gfx_Compositor   *evas_common_gfx_compositor_sub_get(void);
RGBA_Gfx_Compositor   *evas_common_gfx_compositor_sub_rel_get(void);
*/
RGBA_Gfx_Compositor   *evas_common_gfx_compositor_mask_get(void);
RGBA_Gfx_Compositor   *evas_common_gfx_compositor_mul_get(void);

RGBA_Gfx_Func     evas_common_gfx_func_composite_pixel_span_get   (Emage_Surface *src, Emage_Surface *dst, int pixels, int op);
RGBA_Gfx_Func     evas_common_gfx_func_composite_color_span_get   (DATA32 col, Emage_Surface *dst, int pixels, int op);
RGBA_Gfx_Func     evas_common_gfx_func_composite_pixel_color_span_get (Emage_Surface *src, DATA32 col, Emage_Surface *dst, int pixels, int op);
RGBA_Gfx_Func     evas_common_gfx_func_composite_mask_color_span_get  (DATA32 col, Emage_Surface *dst, int pixels, int op);
RGBA_Gfx_Func     evas_common_gfx_func_composite_pixel_mask_span_get  (Emage_Surface *src, Emage_Surface *dst, int pixels, int op);

RGBA_Gfx_Pt_Func     evas_common_gfx_func_composite_pixel_pt_get   (int src_flags, Emage_Surface *dst, int op);
RGBA_Gfx_Pt_Func     evas_common_gfx_func_composite_color_pt_get   (DATA32 col, Emage_Surface *dst, int op);
RGBA_Gfx_Pt_Func     evas_common_gfx_func_composite_pixel_color_pt_get (int src_flags, DATA32 col, Emage_Surface *dst, int op);
RGBA_Gfx_Pt_Func     evas_common_gfx_func_composite_mask_color_pt_get  (DATA32 col, Emage_Surface *dst, int op);
RGBA_Gfx_Pt_Func     evas_common_gfx_func_composite_pixel_mask_pt_get  (int src_flags, Emage_Surface *dst, int op);

EAPI Gfx_Func_Copy        evas_common_draw_func_copy_get        (int pixels, int reverse);

/* BLENDING */
/* src pixel flags: */

/* pixels none */
#define SP_N 0
/* pixels (argb default) */
#define SP 1
/* pixels are rgb (ie. alphas == 255) */
#define SP_AN 2
/* pixels alpha are sparse */
#define SP_AS 3
/* src pixels flags count */
#define SP_LAST 4

/* src mask flags: */

/* mask none */
#define SM_N 0
/* mask (alpha) */
#define SM 1
/* mask alphas are 'trivial - ie. only 0 or 255 */
#define SM_AT 2
/* mask alphas are sparse */
#define SM_AS 3
/* src mask flags count */
#define SM_LAST 4

/* src color flags: */

/* color is 0xffffffff */
#define SC_N 0
/* color (argb default) */
#define SC 1
/* color is rgb (ie. 0xffrrggbb) */
#define SC_AN 2
/* color is 'alpha' (ie. 0xaaaaaaaa) */
#define SC_AA 3
/* src color flags count */
#define SC_LAST 4

/* dst pixels flags: */

/* pixels (argb default) */
#define DP  0
/* pixels are rgb (ie. alphas == 255) */
#define DP_AN  1
/* dst pixels flags count */
#define DP_LAST 2

/* cpu types flags */

/* none, bad news */
#define CPU_N  0
/* cpu C */
#define CPU_C  1
/* cpu MMX */
#define CPU_MMX 2
/* cpu SSE */
#define CPU_SSE 3
/* cpu SSE2 */
#define CPU_SSE2 4
/* cpu flags count */
#define CPU_LAST 5


#endif
