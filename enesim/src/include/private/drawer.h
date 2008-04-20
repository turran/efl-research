#ifndef DRAWER_H_
#define DRAWER_H_

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


#endif /*DRAWER_H_*/
