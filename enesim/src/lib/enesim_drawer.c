#include "enesim_common.h"
#include "Enesim.h"
#include "enesim_private.h"
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/* 
 * 
 * rop functions 
 * pixel
 * color in ARGB format
 * pixel_color
 * mask_color
 * pixel_mask
 */

/**
 * An ARGB color can be opaque or transparent, depending on the alpha value.
 * To optimize the functions we should define one for each case
 */
enum Color_Type
{
	COLOR_OPAQUE,
	COLOR_TRANSPARENT,
	COLOR_TYPES,
};

/* 
 * A drawer should implement functions for every format in case of using
 * pixel source. For color source it should implement the function with
 * opaque value and no opaque.
 */
typedef struct _Enesim_Drawer
{
	/* Scanlines */
	Enesim_Drawer_Span sp_color[ENESIM_ROPS];
	Enesim_Drawer_Span sp_mask_color[ENESIM_ROPS][COLOR_TYPES];
	Enesim_Drawer_Span sp_pixel[ENESIM_ROPS][ENESIM_SURFACE_FORMATS];
	Enesim_Drawer_Span sp_pixel_color[ENESIM_ROPS][ENESIM_SURFACE_FORMATS][COLOR_TYPES];	
	Enesim_Drawer_Span sp_pixel_mask[ENESIM_ROPS][ENESIM_SURFACE_FORMATS];
	/* Points */
	Enesim_Drawer_Point pt_color[ENESIM_ROPS];
	Enesim_Drawer_Point pt_mask_color[ENESIM_ROPS][COLOR_TYPES];
	Enesim_Drawer_Point pt_pixel[ENESIM_ROPS][ENESIM_SURFACE_FORMATS];
	Enesim_Drawer_Point pt_pixel_color[ENESIM_ROPS][ENESIM_SURFACE_FORMATS][COLOR_TYPES];
	Enesim_Drawer_Point pt_pixel_mask[ENESIM_ROPS][ENESIM_SURFACE_FORMATS];
} Enesim_Drawer;

Enesim_Drawer drawer[ENESIM_SURFACE_FORMATS];

/**
 * 
 */
EAPI Enesim_Drawer_Point enesim_drawer_point_color_get(Enesim_Rop rop, Enesim_Surface_Format dfmt, unsigned int color)
{
	/* TODO check if the color is opaque */
	return drawer[dfmt].pt_color[rop];
}

/* Span functions */

/**
 * Do we need to pass the length here?
 */
EAPI Enesim_Drawer_Span enesim_drawer_span_color_get(Enesim_Rop rop, Enesim_Surface_Format dfmt, unsigned int color)
{
	/* TODO check if the color is opaque */
	return drawer[dfmt].sp_color[rop];
}

/**
 * 
 */
EAPI Enesim_Drawer_Span enesim_drawer_span_pixel_get(Enesim_Rop rop, Enesim_Surface_Format dfmt, Enesim_Surface_Format sfmt)
{
	/* TODO check if the color is opaque */
	return drawer[sfmt].sp_pixel[rop][dfmt];
}

//EAPI Enesim_Drawer_Color enesim_drawer_pt_color_get(Enesim_Rop rop)
