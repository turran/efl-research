#ifndef _EMAGE_PRIVATE_H
#define _EMAGE_PRIVATE_H

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

   RGBA_Gfx_Func  (*composite_pixel_span_get)(RGBA_Surface *src, RGBA_Surface *dst, int pixels);
   RGBA_Gfx_Func  (*composite_color_span_get)(DATA32 col, RGBA_Surface *dst, int pixels);
   RGBA_Gfx_Func  (*composite_pixel_color_span_get)(RGBA_Surface *src, DATA32 col, RGBA_Surface *dst, int pixels);
   RGBA_Gfx_Func  (*composite_mask_color_span_get)(DATA32 col, RGBA_Surface *dst, int pixels);
   RGBA_Gfx_Func  (*composite_pixel_mask_span_get)(RGBA_Surface *src, RGBA_Surface *dst, int pixels);

   RGBA_Gfx_Pt_Func  (*composite_pixel_pt_get)(int src_flags, RGBA_Surface *dst);
   RGBA_Gfx_Pt_Func  (*composite_color_pt_get)(DATA32 col, RGBA_Surface *dst);
   RGBA_Gfx_Pt_Func  (*composite_pixel_color_pt_get)(int src_flags, DATA32 col, RGBA_Surface *dst);
   RGBA_Gfx_Pt_Func  (*composite_mask_color_pt_get)(DATA32 col, RGBA_Surface *dst);
   RGBA_Gfx_Pt_Func  (*composite_pixel_mask_pt_get)(int src_flags, RGBA_Surface *dst);
};

#define _EVAS_RENDER_FILL        -1
#define _EVAS_RENDER_BLEND        0
#define _EVAS_RENDER_BLEND_REL    1
#define _EVAS_RENDER_COPY         2
#define _EVAS_RENDER_COPY_REL     3
#define _EVAS_RENDER_ADD          4
#define _EVAS_RENDER_ADD_REL      5
#define _EVAS_RENDER_SUB          6
#define _EVAS_RENDER_SUB_REL      7
#define _EVAS_RENDER_TINT         8
#define _EVAS_RENDER_TINT_REL     9
#define _EVAS_RENDER_MASK         10
#define _EVAS_RENDER_MUL          11
#define _EVAS_RENDER_CLIP         12

#ifndef WORDS_BIGENDIAN
/* x86 */
#define A_VAL(p) ((DATA8 *)(p))[3]
#define R_VAL(p) ((DATA8 *)(p))[2]
#define G_VAL(p) ((DATA8 *)(p))[1]
#define B_VAL(p) ((DATA8 *)(p))[0]
#define AR_VAL(p) ((DATA16 *)(p)[1])
#define GB_VAL(p) ((DATA16 *)(p)[0])
#else
/* ppc */
#define A_VAL(p) ((DATA8 *)(p))[0]
#define R_VAL(p) ((DATA8 *)(p))[1]
#define G_VAL(p) ((DATA8 *)(p))[2]
#define B_VAL(p) ((DATA8 *)(p))[3]
#define AR_VAL(p) ((DATA16 *)(p)[0])
#define GB_VAL(p) ((DATA16 *)(p)[1])
#endif

#define RGB_JOIN(r,g,b) \
        (((r) << 16) + ((g) << 8) + (b))

#define ARGB_JOIN(a,r,g,b) \
        (((a) << 24) + ((r) << 16) + ((g) << 8) + (b))

typedef enum _CPU_Features
{
   CPU_FEATURE_C       = 0,
   CPU_FEATURE_MMX     = (1 << 0),
   CPU_FEATURE_MMX2    = (1 << 1),
   CPU_FEATURE_SSE     = (1 << 2),
   CPU_FEATURE_ALTIVEC = (1 << 3),
   CPU_FEATURE_VIS     = (1 << 4),
   CPU_FEATURE_VIS2    = (1 << 5)
} CPU_Features;

#endif
