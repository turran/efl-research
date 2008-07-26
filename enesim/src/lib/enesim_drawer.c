#include "enesim_common.h"
#include "Enesim.h"
#include "enesim_private.h"
/* 
 * 
 * rop functions 
 * pixel
 * color in ARGB format
 * pixel_color
 * mask_color
 * pixel_mask
 */
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void _unbuilt_drawer_pt(Enesim_Surface_Data *d, Enesim_Surface_Data *s,
		unsigned int color, unsigned char *mask)
{
	
}
static void _unbuilt_drawer_span(Enesim_Surface_Data *d, unsigned int len,
		Enesim_Surface_Data *s, unsigned int color, unsigned char *mask)
{
	
}

Enesim_Drawer _unbuilt;

#ifdef BUILD_SURFACE_ARGB888_PRE
extern Enesim_Drawer argb8888_pre_drawer;
#endif

#ifdef BUILD_SURFACE_ARGB888
extern Enesim_Drawer argb8888_drawer;
#endif

Enesim_Drawer *drawer[ENESIM_SURFACE_FORMATS] = {
#ifdef BUILD_SURFACE_ARGB888
		[ENESIM_SURFACE_ARGB8888] = &argb8888_drawer,
#else
		[ENESIM_SURFACE_ARGB8888] = &_unbuilt,
#endif
#ifdef BUILD_SURFACE_ARGB888_PRE
		[ENESIM_SURFACE_ARGB8888_PRE] = &argb8888_pre_drawer,
#else
		[ENESIM_SURFACE_ARGB8888] = &_unbuilt,
#endif
};
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
void enesim_drawer_init(void)
{
	int i;
	int j;
	
	for (i = 0; i < ENESIM_ROPS; i++)
	{
		int j;
		
		/* sp_color, pt_color */
		_unbuilt.sp_color[i] = &_unbuilt_drawer_span;
		_unbuilt.pt_color[i] = &_unbuilt_drawer_pt;
		/* sp_pixel, sp_pixel_mask, pt_pixel, pt_pixel_mask */
		for (j = 0; j < ENESIM_SURFACE_FORMATS; j++)
		{
			int k;
			
			_unbuilt.sp_pixel[i][j] = &_unbuilt_drawer_span;
			_unbuilt.sp_pixel_mask[i][j] = &_unbuilt_drawer_span;
			_unbuilt.pt_pixel_mask[i][j] = &_unbuilt_drawer_pt;
			_unbuilt.pt_pixel_mask[i][j] = &_unbuilt_drawer_pt;
			/* sp_pixel_color, pt_pixel_color */
			for (k = 0; k < COLOR_TYPES; k++)
			{
				_unbuilt.pt_pixel_color[i][j][k] = _unbuilt_drawer_pt;
				_unbuilt.sp_pixel_color[i][j][k] = _unbuilt_drawer_span;
			}
		}
		/* sp_mask_color, pt_mask_color */
		for (j = 0; j < COLOR_TYPES; j++)
		{
			_unbuilt.pt_mask_color[i][j] = &_unbuilt_drawer_pt;
			_unbuilt.sp_mask_color[i][j] = &_unbuilt_drawer_span;
		}
	}
}

void enesim_drawer_shutdown(void)
{
	/* do nothing */
}
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
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
