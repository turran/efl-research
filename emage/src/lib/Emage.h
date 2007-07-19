#ifndef _EMAGE_H
#define _EMAGE_H

#ifdef EAPI
#undef EAPI
#endif
#ifdef _MSC_VER
# ifdef BUILDING_DLL
#  define EAPI __declspec(dllexport)
# else
#  define EAPI __declspec(dllimport)
# endif
#else
# ifdef __GNUC__
#  if __GNUC__ >= 4
#   define EAPI __attribute__ ((visibility("default")))
#  else
#   define EAPI
#  endif
# else
#  define EAPI
# endif
#endif

/* Data Types */
typedef unsigned int 	DATA32;
typedef unsigned short 	DATA16;
typedef unsigned char	DATA8;

/* Data */
typedef struct _Evas_Object_List      Evas_Object_List;
struct _Evas_Object_List
{
   Evas_Object_List  *next, *prev;
   Evas_Object_List  *last;
};


/* Draw */
typedef struct _RGBA_Draw_Context RGBA_Draw_Context;
typedef struct _RGBA_Surface          RGBA_Surface;
typedef struct _Cutout_Rects            Cutout_Rects;
typedef struct _Cutout_Rect           Cutout_Rect;

struct _Cutout_Rect
{
   int               x, y, w, h;
};

struct _Cutout_Rects
{
   Cutout_Rect*      rects;
   int               active;
   int               max;
};


/* FIXME the above is duplicated internally in _private.h */
typedef enum _Evas_Render_Op
{
   EVAS_RENDER_BLEND = 0, /**< default op: d = d*(1-sa) + s */
   EVAS_RENDER_BLEND_REL = 1, /**< d = d*(1 - sa) + s*da */
   EVAS_RENDER_COPY = 2, /**< d = s */
   EVAS_RENDER_COPY_REL = 3, /**< d = s*da */
   EVAS_RENDER_ADD = 4, /**< d = d + s */
   EVAS_RENDER_ADD_REL = 5, /**< d = d + s*da */
   EVAS_RENDER_SUB = 6, /**< d = d - s */
   EVAS_RENDER_SUB_REL = 7, /**< d = d - s*da */
   EVAS_RENDER_TINT = 8, /**< d = d*s + d*(1 - sa) + s*(1 - da) */
   EVAS_RENDER_TINT_REL = 9, /**< d = d*(1 - sa + s) */
   EVAS_RENDER_MASK = 10, /**< d = d*sa */
   EVAS_RENDER_MUL = 11 /**< d = d*s */
} Evas_Render_Op; /**<  */

/* Context
 * ~~~~~~~ 
 */

struct _RGBA_Draw_Context
{
   struct {
      char   use : 1;
      DATA32 col;
   } mul;
   struct {
      DATA32 col;
   } col;
   struct RGBA_Draw_Context_clip {
      int    x, y, w, h;
      char   use : 1;
   } clip;
   Cutout_Rects cutout;
#if 0
   struct {
      struct {
	 void *(*gl_new)  (void *data, RGBA_Font_Glyph *fg);
	 void  (*gl_free) (void *ext_dat);
	 void  (*gl_draw) (void *data, void *dest, void *context, RGBA_Font_Glyph *fg, int x, int y);
      } func;
      void *data;
   } font_ext;
#endif
   struct {
      int color_space;
   } interpolation;
   struct {
      int y, h;
   } sli;
   int            render_op;
   unsigned char  anti_alias : 1;
};



/* Polygon */
typedef struct _RGBA_Polygon_Point    RGBA_Polygon_Point;
struct _RGBA_Polygon_Point
{
   Evas_Object_List  _list_data;
   int               x, y;
};


/* Scale 
 * ~~~~~
 */

EAPI void
evas_common_scale_rgba_in_to_out_clip_smooth(RGBA_Surface *src, RGBA_Surface *dst,
				 RGBA_Draw_Context *dc,
				 int src_region_x, int src_region_y,
				 int src_region_w, int src_region_h,
				 int dst_region_x, int dst_region_y,
				 int dst_region_w, int dst_region_h);


/* possible API
 * ~~~~~~~~~~~~
 */
EAPI int 		emage_init(void);
EAPI void 		emage_shutdown(void);
/* Context */
/* FIXME change the name of the above */
EAPI RGBA_Draw_Context 	*emage_draw_context_new(void);
EAPI void               emage_draw_context_clip_clip         (RGBA_Draw_Context *dc, int x, int y, int w, int h);
EAPI void               emage_draw_context_set_clip          (RGBA_Draw_Context *dc, int x, int y, int w, int h);
EAPI void               emage_draw_context_unset_clip        (RGBA_Draw_Context *dc);
EAPI void               emage_draw_context_set_color         (RGBA_Draw_Context *dc, int r, int g, int b, int a);
EAPI void               emage_draw_context_set_multiplier    (RGBA_Draw_Context *dc, int r, int g, int b, int a);
EAPI void               emage_draw_context_unset_multiplier  (RGBA_Draw_Context *dc);
EAPI void               emage_draw_context_add_cutout        (RGBA_Draw_Context *dc, int x, int y, int w, int h);
EAPI Cutout_Rect*	emage_draw_context_cutouts_add(Cutout_Rects* rects, int x, int y, int w, int h);
EAPI void               emage_draw_context_clear_cutouts     (RGBA_Draw_Context *dc);
EAPI Cutout_Rects      *emage_draw_context_apply_cutouts     (RGBA_Draw_Context *dc);
EAPI void               emage_draw_context_apply_clear_cutouts(Cutout_Rects* rects);
EAPI void               emage_draw_context_apply_clean_cutouts(Cutout_Rects* rects);
EAPI void               emage_draw_context_set_anti_alias    (RGBA_Draw_Context *dc, unsigned char aa);
EAPI void               emage_draw_context_set_color_interpolation(RGBA_Draw_Context *dc, int color_space);
EAPI void               emage_draw_context_set_render_op     (RGBA_Draw_Context *dc, int op);
EAPI void               emage_draw_context_set_sli           (RGBA_Draw_Context *dc, int y, int h);
/* Surface */
EAPI RGBA_Surface *	emage_surface_new(void *data, int w, int h);
EAPI inline void	emage_surface_size_get(RGBA_Surface *s, int *w, int *h);
EAPI inline void *	emage_surface_data_get(RGBA_Surface *s);
EAPI inline void	emage_surface_data_set(RGBA_Surface *s, void *data);
/* Objects */
EAPI void 		emage_line_draw(RGBA_Surface *dst, RGBA_Draw_Context *dc, int x0, int y0, int x1, int y1);
EAPI void 		emage_rectangle_draw(RGBA_Surface *dst, RGBA_Draw_Context *dc, int x, int y, int w, int h);
EAPI void 		emage_polygon_draw(RGBA_Surface *dst, RGBA_Draw_Context *dc, RGBA_Polygon_Point *points);
/* Scale */

typedef enum _Emage_Colorspace
{
	EMAGE_COLORSPACE_RGBA
} Emage_Colorspace;


#endif
