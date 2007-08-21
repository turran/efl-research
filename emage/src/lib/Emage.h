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

/**
 * @file
 * @brief Emage API
 * @defgroup Emage API
 *
 * @{
 */

/** 
 * @defgroup Core_Group Core
 * @{
 */
EAPI int 		emage_init(void);
EAPI void 		emage_shutdown(void);

/** @} */

/**
 * @defgroup Data_Types_Group Data Types
 * @{
 */
typedef unsigned int 	DATA32; /**< 32 bits */
typedef unsigned short 	DATA16; /**< 16 bits */
typedef unsigned char	DATA8;	/**< 8 bits */

/* FIXME remove this
 */
typedef struct _Evas_Object_List      Evas_Object_List;
struct _Evas_Object_List
{
   Evas_Object_List  *next, *prev;
   Evas_Object_List  *last;
};

/**
 * TODO
 */
typedef struct _Emage_Rectangle
{
	int x, y, w, h;
} Emage_Rectangle;

#define EMAGE_RECT_FROM_COORDS(r, xc, yc, wc, hc) 			\
r.x = xc;								\
r.y = yc;								\
r.w = wc;								\
r.h = hc;								

/** @} */

/**
 * @defgroup Color_Group Color
 * @todo
 * - Maybe add support for other components (like YUV) ? if so we need an union
 *
 * @{
 */

/**
 * Color Container
 */
typedef struct _Emage_Color
{
	int r; /**< Red Component */
	int g; /**< Green Component */
	int b; /**< Blue Component */
	int a; /**< Alpha Component */
} Emage_Color;
/** @} */

/**
 * @defgroup Surface_Group Surface
 * @todo
 * - Add functions to _set, _get surface flags
 * @{
 */

typedef struct _Emage_Surface 		Emage_Surface; /**< A Surface Handler */

/**
 * TODO
 */
typedef enum _Emage_Data_Format
{
	EMAGE_DATA_ARGB8888, 	/**< ARGB8888 */
	EMAGE_DATA_RGB565_A5, 	/**< RGB5656 A5 */
	EMAGE_DATA_FORMATS
} Emage_Data_Format;


EAPI Emage_Surface * 	emage_surface_new(Emage_Data_Format f, int w, int h, ...);
EAPI void	emage_surface_size_get(Emage_Surface *s, int *w, int *h);
EAPI void 	emage_surface_data_get(Emage_Surface *s, ...);
EAPI void 	emage_surface_data_set(Emage_Surface *s, Emage_Data_Format f, ...);
EAPI Emage_Data_Format emage_surface_format_get(Emage_Surface *s);

/** @} */


/**
 * @defgroup Draw_Context_Group Draw Context
 * @{
 */
typedef struct _Emage_Draw_Context 	Emage_Draw_Context; /**< A Draw Context Handler */
typedef struct _Cutout_Rects 		Cutout_Rects;
typedef struct _Cutout_Rect 		Cutout_Rect;

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
 
/**
 * d = Destination
 * s = Source
 * sa = Source Alpha
 * da = Destination Alpha
 * TODO remove the =
 */
typedef enum _Emage_Render_Op
{
	EMAGE_RENDER_BLEND = 0, /**< Default op: d = d*(1-sa) + s */
#if 0
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
#endif
	EMAGE_RENDER_OPS /**< Total number of rendering operations */
} Emage_Render_Op;

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

/**
 * @todo and fill with a gradient?
 */
typedef enum _Emage_Fill_Type
{
	EMAGE_FILL_COLOR, /**< TODO */
	EMAGE_FILL_SURFACE, /**< TODO */
	EMAGE_FILL_TYPES
} Emage_Fill_Type;

/**
 * TODO
 */
typedef enum _Emage_Fill_Surface_Type
{
	EMAGE_FILL_SURFACE_REPEAT_NONE 	= 0, /**< TODO */
	EMAGE_FILL_SURFACE_REPEAT_X	= (1 << 0), /**< TODO */
	EMAGE_FILL_SURFACE_REPEAT_Y 	= (1 << 1), /**< TODO */
	EMAGE_FILL_SURFACE_TYPES
} Emage_Fill_Surface_Type;

EAPI void emage_draw_context_fill_type_set(Emage_Draw_Context *dc, Emage_Fill_Type t);
EAPI void emage_draw_context_fill_surface_type_set(Emage_Draw_Context *dc, Emage_Fill_Surface_Type t);
EAPI void emage_draw_context_fill_surface_set(Emage_Draw_Context *dc, Emage_Surface *s, Emage_Rectangle *srect, Emage_Rectangle *drect);


/** @} */

/**
 * @defgroup Geometry_Group Geometry
 * @{
 */
typedef struct _Emage_Polygon_Point    Emage_Polygon_Point;
struct _Emage_Polygon_Point
{
   Evas_Object_List  _list_data;
   int               x, y;
};

EAPI void emage_line_draw(Emage_Surface *dst, Emage_Draw_Context *dc, int x0, int y0, int x1, int y1);
EAPI void emage_rectangle_draw(Emage_Rectangle *r, Emage_Surface *dst, Emage_Draw_Context *dc);
EAPI Emage_Polygon_Point *emage_polygon_point_add(Emage_Polygon_Point *points, int x, int y);
EAPI Emage_Polygon_Point *emage_polygon_points_clear(Emage_Polygon_Point *points);

/** @} */

/**
 * @defgroup Scaler_Group Scaler
 * @{
 */

/**
 * TODO
 * FIXME what about unscaled? 
 */
typedef enum _Emage_Scaler_Type
{
	EMAGE_SCALER_SMOOTH, 	/**< TODO */
	EMAGE_SCALER_SAMPLED, 	/**< TODO */
	EMAGE_SCALER_TYPES 	/**< TODO */
} Emage_Scaler_Type;

EAPI void emage_scale(Emage_Surface *src, Emage_Surface *dst, Emage_Rectangle srect, Emage_Rectangle drect, Emage_Draw_Context *dc);

/** @} */

/**
 * @defgroup Scanlines_Group Scanlines
 * @{
 */
typedef struct _Emage_Span 	Emage_Span; /**< TODO */
typedef struct _Emage_Scanline 	Emage_Scanline; /**< TODO */

/**
 * TODO
 */
struct _Emage_Scanline
{
	int 		x; /**< TODO */
	int 		y; /**< TODO */
	int 		w; /**< TODO */
	unsigned int coverage; /**< TODO */
};

EAPI void emage_scanline_draw(Emage_Scanline *sl, Emage_Surface *dst, Emage_Draw_Context *dc);

/** @} */

/** @} */

#endif
