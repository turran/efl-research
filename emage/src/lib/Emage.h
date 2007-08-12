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

/* Surface */
typedef enum _Emage_Data_Format
{
	EMAGE_DATA_ARGB8888,
	EMAGE_DATA_RGB565_A5,
	EMAGE_DATA_FORMATS
} Emage_Data_Format;

typedef struct _Emage_Rectangle
{
	int x, y, w, h;
} Emage_Rectangle;

/* Draw */
typedef struct _Emage_Draw_Context Emage_Draw_Context;
typedef struct _Emage_Surface          Emage_Surface;
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
typedef enum _Emage_Scaler_Type
{
	EMAGE_SCALER_SMOOTH,
	EMAGE_SCALER_SAMPLED,
	EMAGE_SCALER_TYPES
} Emage_Scaler_Type;

EAPI void emage_scale(Emage_Surface *src, Emage_Surface *dst, Emage_Rectangle srect, Emage_Rectangle drect, Emage_Draw_Context *dc);

/*EAPI void
evas_common_scale_rgba_in_to_out_clip_smooth(Emage_Surface *src, Emage_Surface *dst,
				 Emage_Draw_Context *dc,
				 int src_region_x, int src_region_y,
				 int src_region_w, int src_region_h,
				 int dst_region_x, int dst_region_y,
				 int dst_region_w, int dst_region_h);

*/
/* possible API
 * ~~~~~~~~~~~~
 */

EAPI int 		emage_init(void);
EAPI void 		emage_shutdown(void);
/* Context */
/* FIXME the above is duplicated internally in _private.h
 * remove the =
 */
typedef enum _Emage_Render_Op
{
	EMAGE_RENDER_BLEND = 0, /**< default op: d = d*(1-sa) + s */
	EMAGE_RENDER_BLEND_REL = 1, /**< d = d*(1 - sa) + s*da */
	EMAGE_RENDER_COPY = 2, /**< d = s */
	EMAGE_RENDER_COPY_REL = 3, /**< d = s*da */
	EMAGE_RENDER_ADD = 4, /**< d = d + s */
	EMAGE_RENDER_ADD_REL = 5, /**< d = d + s*da */
	EMAGE_RENDER_SUB = 6, /**< d = d - s */
	EMAGE_RENDER_SUB_REL = 7, /**< d = d - s*da */
	EMAGE_RENDER_TINT = 8, /**< d = d*s + d*(1 - sa) + s*(1 - da) */
	EMAGE_RENDER_TINT_REL = 9, /**< d = d*(1 - sa + s) */
	EMAGE_RENDER_MASK = 10, /**< d = d*sa */
	EMAGE_RENDER_MUL = 11, /**< d = d*s */
	//EMAGE_RENDER_CLIP
	EMAGE_RENDER_OPS
} Emage_Render_Op; /**<  */

/* FIXME normalize the name of the above */
EAPI Emage_Draw_Context 	*emage_draw_context_new(void);
EAPI void               emage_draw_context_clip_clip         (Emage_Draw_Context *dc, int x, int y, int w, int h);
EAPI void               emage_draw_context_set_clip          (Emage_Draw_Context *dc, int x, int y, int w, int h);
EAPI void               emage_draw_context_unset_clip        (Emage_Draw_Context *dc);
EAPI void               emage_draw_context_set_color         (Emage_Draw_Context *dc, int r, int g, int b, int a);
EAPI void               emage_draw_context_set_multiplier    (Emage_Draw_Context *dc, int r, int g, int b, int a);
EAPI void               emage_draw_context_unset_multiplier  (Emage_Draw_Context *dc);
EAPI void               emage_draw_context_add_cutout        (Emage_Draw_Context *dc, int x, int y, int w, int h);
EAPI Cutout_Rect*	emage_draw_context_cutouts_add(Cutout_Rects* rects, int x, int y, int w, int h);
EAPI void               emage_draw_context_clear_cutouts     (Emage_Draw_Context *dc);
EAPI Cutout_Rects      *emage_draw_context_apply_cutouts     (Emage_Draw_Context *dc);
EAPI void               emage_draw_context_apply_clear_cutouts(Cutout_Rects* rects);
EAPI void               emage_draw_context_apply_clean_cutouts(Cutout_Rects* rects);
EAPI void               emage_draw_context_set_anti_alias    (Emage_Draw_Context *dc, unsigned char aa);
EAPI void               emage_draw_context_set_color_interpolation(Emage_Draw_Context *dc, int color_space);
EAPI void               emage_draw_context_set_render_op     (Emage_Draw_Context *dc, int op);
EAPI void               emage_draw_context_set_sli           (Emage_Draw_Context *dc, int y, int h);
EAPI void 		emage_draw_context_cutouts_del(Cutout_Rects* rects, int index);
/* Surface */
EAPI Emage_Surface * 	emage_surface_new(Emage_Data_Format f, int w, int h, ...);
EAPI void	emage_surface_size_get(Emage_Surface *s, int *w, int *h);
EAPI void 	emage_surface_data_get(Emage_Surface *s, ...);
EAPI void 	emage_surface_data_set(Emage_Surface *s, Emage_Data_Format f, ...);
/* Objects */
EAPI void 		emage_line_draw(Emage_Surface *dst, Emage_Draw_Context *dc, int x0, int y0, int x1, int y1);
EAPI void 		emage_rectangle_draw(Emage_Surface *dst, Emage_Draw_Context *dc, int x, int y, int w, int h);
EAPI void 		emage_polygon_draw(Emage_Surface *dst, Emage_Draw_Context *dc, RGBA_Polygon_Point *points);


#endif
