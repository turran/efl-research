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

/* 
 * A drawer should implement functions for every format in case of using
 * pixel source. For color source it should implement the function with
 * opaque value and no opaque.
 */
typedef struct _Enesim_Drawer
{
	/* Scanlines */
	Enesim_Drawer_Span sp_color;
	Enesim_Drawer_Span sp_mask_color;
	Enesim_Drawer_Span sp_pixel[ENESIM_SURFACE_FORMATS];
	Enesim_Drawer_Span sp_pixel_color[ENESIM_SURFACE_FORMATS];	
	Enesim_Drawer_Span sp_pixel_mask[ENESIM_SURFACE_FORMATS];
	/* Points */
	Enesim_Drawer_Point pt_color;
	Enesim_Drawer_Point pt_mask_color;
	Enesim_Drawer_Point pt_pixel[ENESIM_SURFACE_FORMATS];
	Enesim_Drawer_Point pt_pixel_color[ENESIM_SURFACE_FORMATS];
	Enesim_Drawer_Point pt_pixel_mask[ENESIM_SURFACE_FORMATS];
} Enesim_Drawer;

Enesim_Drawer drawer[ENESIM_SURFACE_FORMATS][ENESIM_ROPS];

/**
 * 
 */
EAPI Enesim_Drawer_Point enesim_drawer_point_color_get(Enesim_Rop rop, Enesim_Surface_Format dfmt, unsigned int color)
{
	/* TODO check if the color is opaque */
	return drawer[dfmt][rop].pt_color;
}

/* Span functions */

/**
 * Do we need to pass the length here?
 */
EAPI Enesim_Drawer_Span enesim_drawer_span_color_get(Enesim_Rop rop, Enesim_Surface_Format dfmt, unsigned int color)
{
	/* TODO check if the color is opaque */
	return drawer[dfmt][rop].sp_color;
}

/**
 * 
 */
EAPI Enesim_Drawer_Span enesim_drawer_span_pixel_get(Enesim_Rop rop, Enesim_Surface_Format dfmt, Enesim_Surface_Format sfmt)
{
	/* TODO check if the color is opaque */
	return drawer[sfmt][rop].sp_pixel[dfmt];
}

//EAPI Enesim_Drawer_Color enesim_drawer_pt_color_get(Enesim_Rop rop)
