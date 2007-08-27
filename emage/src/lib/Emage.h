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
 * @mainpage Emage
 * @section intro Introduction
 * Emage is a state less graphics library based on the Evas' software
 * common engine.
 *
 * @section changes Changes from the Evas Common Engine
 * - Geometric objects generate scanlines that are drawn on the scanline
 *   module, this allows to abstract the drawing context options from the
 *   geometric algorithms. Things like drawing a rectangle based on the 
 *   pixels of another surface are now possible with minor code changes.
 * - Everything related to fonts is taken away from the drawing context.
 * - We have removed the RGBA_Image as the main pixel data container and
 *   instead we define an Emage_Surface which name makes more sense.
 * - An Emage_Surface abstracts the pixel data it contains based on the pixel
 *   format, right now we support pixel data in ARGB8888 (8 bits per component)
 *   and in RGB565_A5 (5 bits for red, blue and alpha but in another plane,
 *   6 bits for green) formats.
 * - The compositors are abstracted based on two parameters, the @link 
 *   #Emage_Data_Format format @endlink of the @link #Emage_Surface surface
 *   @endlink and the @link #Emage_Render_Op render operation @endlink (we 
 *   use a functions table).
 * - The scaling algorithm is now a parameter of the Drawing Context.
 * - All the headers have been split into smaller ones, for better maintenance
 *   an easy understanding on what belongs to what.
 *
 * @section features New features
 * - Possibility to draw a scanline taking as a reference another surface,
 *   objects can be filled with another surface repeating on X, Y, both 
 *   or none.
 *
 *
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


/**
 * TODO
 */
typedef struct _Emage_Rectangle
{
	int x, y, w, h;
} Emage_Rectangle;


/* delete the reference for this MACROS */

#define EMAGE_RECT_FROM_COORDS(r, cx, cy, cw, ch) 			\
r.x = cx; 								\
r.y = cy; 								\
r.w = cw; 								\
r.h = ch;

#define EMAGE_RECT_IS_EMPTY(r) 	((r.w < 1) || (r.h < 1))

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

#define EMAGE_COLOR_FROM_COMPONENTS(ec, r, g, b, a) 			\
ec.a = a;								\
ec.r = r;								\
ec.g = g;								\
ec.b = b;								


/** @} */

/**
 * @defgroup Draw_Context_Group Draw Context
 * @{
 */

typedef struct _Emage_Draw_Context 	Emage_Draw_Context; /**< A Draw Context Handler */

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
EAPI void emage_surface_scale(Emage_Surface *src, Emage_Rectangle sr, Emage_Surface *dst, Emage_Rectangle dr, Emage_Draw_Context *dc);


/** @} */


/**
 * @addtogroup Draw_Context_Group 
 * @{
 */
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
 * TODO
 * FIXME what about unscaled? 
 */
typedef enum _Emage_Scaler_Type
{
	EMAGE_SCALER_SMOOTH, 	/**< TODO */
	EMAGE_SCALER_SAMPLED, 	/**< TODO */
	EMAGE_SCALER_TYPES 	/**< TODO */
} Emage_Scaler_Type;


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
 * How to repeat the source surface on the destination surface
 */
typedef enum _Emage_Fill_Surface_Type
{
	EMAGE_FILL_SURFACE_REPEAT_NONE 	= 0, /**< Don't repeat on any axis */
	EMAGE_FILL_SURFACE_REPEAT_X	= (1 << 0), /**< Repeat on X axis */
	EMAGE_FILL_SURFACE_REPEAT_Y 	= (1 << 1), /**< Repeat on Y axis */
	EMAGE_FILL_SURFACE_TYPES
} Emage_Fill_Surface_Type;

EAPI void emage_draw_context_fill_type_set(Emage_Draw_Context *dc, Emage_Fill_Type t);
EAPI void emage_draw_context_fill_surface_type_set(Emage_Draw_Context *dc, Emage_Fill_Surface_Type t);
EAPI void emage_draw_context_fill_surface_set(Emage_Draw_Context *dc, Emage_Surface *s, Emage_Rectangle *srect, Emage_Rectangle *drect);
EAPI void 		emage_draw_context_scaler_type_set(Emage_Draw_Context *dc, Emage_Scaler_Type t);


/** @} */

/**
 * @defgroup Geometry_Group Geometry
 * @{
 */
typedef struct _Emage_Polygon_Point    Emage_Polygon_Point; /**< A polygon point handler */

EAPI void emage_line_draw(Emage_Surface *dst, Emage_Draw_Context *dc, int x0, int y0, int x1, int y1);
EAPI void emage_rectangle_draw(Emage_Rectangle *r, Emage_Surface *dst, Emage_Draw_Context *dc);
EAPI Emage_Polygon_Point *emage_polygon_point_add(Emage_Polygon_Point *points, int x, int y);
EAPI Emage_Polygon_Point *emage_polygon_points_clear(Emage_Polygon_Point *points);

/**
 * TODO
 */
static inline void
emage_rectangle_from_coords(Emage_Rectangle *r, int x, int y, int w, int h)
{
	r->x = x;
	r->y = y;
	r->w = w;
	r->h = h;
}

/**
 * TODO
 */
static inline int
emage_rectangle_is_empty(Emage_Rectangle *r)
{
	return ((r->w < 1) || (r->h < 1));
}

/** @} */



/**
 * @defgroup Scanlines_Group Scanlines
 * @{
 */
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

/**
 * TODO
 * again
 */
EAPI static inline int
emage_scanline_split(Emage_Scanline *sl, Emage_Scanline *rsl, int x)
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

/** @} */

/**
 * @defgroup Converter_Group Converter
 * @{
 */

typedef enum _Emage_Rotation
{
	EMAGE_ROTATION_0 	= 0,
	EMAGE_ROTATION_90 	= 90,
	EMAGE_ROTATION_180 	= 180,
	EMAGE_ROTATION_270 	= 270,
	EMAGE_ROTATIONS
} Emage_Rotation;

/**
 * TODO
 */
typedef enum _Emage_Converter_Pal_Mode
{
	EMAGE_PAL_MODE_NONE,
	EMAGE_PAL_MODE_MONO,
	EMAGE_PAL_MODE_GRAY4,
	EMAGE_PAL_MODE_GRAY16,
	EMAGE_PAL_MODE_GRAY64,
	EMAGE_PAL_MODE_GRAY256,
	EMAGE_PAL_MODE_RGB111,
	EMAGE_PAL_MODE_RGB121,
	EMAGE_PAL_MODE_RGB221,
	EMAGE_PAL_MODE_RGB222,
	EMAGE_PAL_MODE_RGB232,
	EMAGE_PAL_MODE_RGB332,
	EMAGE_PAL_MODE_RGB666,
	EMAGE_PAL_MODES
} Emage_Converter_Pal_Mode;

/**
 * TODO
 */
typedef void (*Emage_Converter_Func) (DATA32 *src, DATA8 *dst, int src_jump, int dst_jump, int w, int h, int dith_x, int dith_y, DATA8 *pal);

EAPI Emage_Converter_Func emage_converter_get(DATA8 *dest, int w, int h, int depth, DATA32 rmask, DATA32 gmask, DATA32 bmask, Emage_Converter_Pal_Mode pal_mode, Emage_Rotation rotation);


/** @} */

/** @} */

#endif
