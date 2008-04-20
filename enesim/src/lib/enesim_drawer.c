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

extern Enesim_Drawer argb8888_pre_drawer;
Enesim_Drawer *drawer[ENESIM_SURFACE_FORMATS] = {
		[ENESIM_SURFACE_ARGB8888_PRE] = &argb8888_pre_drawer,
};
/**
 * 
 */
EAPI Enesim_Drawer_Point enesim_drawer_point_color_get(Enesim_Rop rop, Enesim_Surface_Format dfmt, unsigned int color)
{
	/* TODO check if the color is opaque */
	return drawer[dfmt]->pt_color[rop];
}

/* Span functions */

/**
 * Do we need to pass the length here?
 */
EAPI Enesim_Drawer_Span enesim_drawer_span_color_get(Enesim_Rop rop, Enesim_Surface_Format dfmt, unsigned int color)
{
	/* TODO check if the color is opaque */
	return drawer[dfmt]->sp_color[rop];
}

/**
 * 
 */
EAPI Enesim_Drawer_Span enesim_drawer_span_pixel_get(Enesim_Rop rop, Enesim_Surface_Format dfmt, Enesim_Surface_Format sfmt)
{
	/* TODO check if the color is opaque */
	return drawer[sfmt]->sp_pixel[rop][dfmt];
}

//EAPI Enesim_Drawer_Color enesim_drawer_pt_color_get(Enesim_Rop rop)
