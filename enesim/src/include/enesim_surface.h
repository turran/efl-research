#ifndef ENESIM_SURFACE_H_
#define ENESIM_SURFACE_H_

/**
 * @defgroup Enesim_Surface_Group Surface
 * @{
 */
typedef struct _Enesim_Surface 	Enesim_Surface; /**< Surface Handler */
/**
 * 
 */
typedef enum
{
	ENESIM_SURFACE_ALPHA =	(1 << 0),
	ENESIM_SURFACE_DIRTY =	(1 << 1),
} Enesim_Surface_Flag;
/**
 * 
 */
typedef enum
{
	ENESIM_SURFACE_ARGB8888,
	ENESIM_SURFACE_ARGB8888_PRE,
	ENESIM_SURFACE_RGB565,
	ENESIM_SURFACE_FORMATS,
} Enesim_Surface_Format;
/**
 * 
 */
typedef struct _Argb8888_Data
{
	DATA32	*plane0; /* a8r8g8b8 plane */
} Argb8888_Data;
/**
 * 
 */
typedef struct _Rgb565_Data
{
	DATA16	*plane0; /* r5g6b5 plane */
	DATA8 	*plane1; /* a5 plane */
} Rgb565_Data;
/**
 * 
 */
typedef union _Enesim_Surface_Data
{
	Rgb565_Data 	rgb565;	
	Argb8888_Data 	argb8888;	
} Enesim_Surface_Data;

EAPI Enesim_Surface * enesim_surface_new(Enesim_Surface_Format f, int w, int h, Enesim_Surface_Flag flags, ...);
EAPI void enesim_surface_size_get(Enesim_Surface *s, int *w, int *h);
EAPI void enesim_surface_size_set(Enesim_Surface *s, int w, int h);
EAPI void enesim_surface_data_get(Enesim_Surface *s, ...);
EAPI Enesim_Surface_Format enesim_surface_format_get(Enesim_Surface *s);
EAPI int enesim_surface_flag_get(Enesim_Surface *s);
EAPI void enesim_surface_flag_set(Enesim_Surface *s, Enesim_Surface_Flag flags);
EAPI void enesim_surface_data_set(Enesim_Surface *s, Enesim_Surface_Format f, ...);
EAPI void enesim_surface_delete(Enesim_Surface *s);
EAPI void enesim_surface_draw(Enesim_Surface *s, Enesim_Rectangle *srect, Enesim_Surface *d, Enesim_Rectangle *drect, int mode, int smooth);
/** @} */ //End of Enesim_Surface_Group


#endif /*ENESIM_SURFACE_H_*/
