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
 * pixel source
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



/**
 * 
 */
EAPI Enesim_Drawer_Point enesim_drawer_point_color_get(Enesim_Rop rop, Enesim_Surface_Format sfmt, unsigned int color)
{
	/* check if the color is opaque */
}

/**
 * 
 */

//EAPI Enesim_Drawer_Color enesim_drawer_pt_color_get(Enesim_Rop rop)
