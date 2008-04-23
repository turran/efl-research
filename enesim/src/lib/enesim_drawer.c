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
extern Enesim_Drawer argb8888_drawer;

Enesim_Drawer *drawer[ENESIM_SURFACE_FORMATS] = {
		[ENESIM_SURFACE_ARGB8888] = &argb8888_drawer,
		[ENESIM_SURFACE_ARGB8888_PRE] = &argb8888_pre_drawer,
};

/**
 * 
 */
EAPI Enesim_Drawer_Point enesim_drawer_point_color_get(Enesim_Rop rop, Enesim_Surface_Format dfmt, unsigned int color)
{
	/* TODO check if the color is opaque */
#if 0
	if ((rop == ENESIM_BLEND) && (alpha(color) == 0xff))
	{
		rop = RENESIM_FILL;
	}
#endif
	return drawer[dfmt]->pt_color[rop];
}
/**
 * 
 */
EAPI Enesim_Drawer_Point enesim_drawer_point_pixel_get(Enesim_Rop rop, Enesim_Surface_Format dfmt, Enesim_Surface_Format sfmt)
{
	return drawer[dfmt]->pt_pixel[rop][sfmt];
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
	return drawer[dfmt]->sp_pixel[rop][sfmt];
}

//EAPI Enesim_Drawer_Color enesim_drawer_pt_color_get(Enesim_Rop rop)
