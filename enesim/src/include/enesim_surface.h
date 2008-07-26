#ifndef ENESIM_SURFACE_H_
#define ENESIM_SURFACE_H_

/**
 * @defgroup Enesim_Surface_Group Surface
 * @{
 * 
 * TODO
 * remove RGB565 and rename it to RGB565_A8, RGB565_bA3, etc
 * same thing for RGB888, etc. I mean make the alpha relative
 * not always A8
 */
typedef struct _Enesim_Surface 	Enesim_Surface; /**< Surface Handler */
/**
 * 
 */
typedef enum
{
	ENESIM_SURFACE_ARGB8888, /**< */
	ENESIM_SURFACE_ARGB8888_PRE, /**< */
	ENESIM_SURFACE_RGB565, /**< */
	ENESIM_SURFACE_RGB888, /**< */
	ENESIM_SURFACE_A8, /**< */
	ENESIM_SURFACE_b1A3, /**< */
	ENESIM_SURFACE_FORMATS,
} Enesim_Surface_Format;
/**
 * 
 */
typedef struct _Argb8888_Data
{
	uint32_t	*plane0; /* a8r8g8b8 plane */
} Argb8888_Data, Argb8888_Pre_Data;
/**
 * 
 */
typedef struct _Rgb565_Data
{
	uint16_t	*plane0; /* r5g6b5 plane */
	uint8_t 	*plane1; /* a5 plane */
} Rgb565_Data;
/**
 * 
 */
typedef struct _Rgb888_Data
{
	uint8_t 	*plane0; /* r8g8b8 plane */
	uint8_t 	*plane1; /* a8 plane */
} Rgb888_Data;
/**
 * TODO how to handle this??
 */
typedef struct _A8_Data
{
	uint8_t 	*plane0; /* a8 plane */
} A8_Data;
/**
 * 
 */
typedef struct _b1A3_Data
{
	uint8_t *plane0; /* b1A3 plane */
} bA3_Data;
/**
 * 
 */
typedef union _Enesim_Surface_Data
{
	Rgb565_Data rgb565;
	Rgb888_Data rgb888;
	A8_Data a8;
	Argb8888_Data argb8888;
	Argb8888_Pre_Data argb8888_pre;
} Enesim_Surface_Data;

EAPI Enesim_Surface * enesim_surface_new(Enesim_Surface_Format f, int w, int h, Enesim_Surface_Data *sdata);
EAPI void enesim_surface_size_get(const Enesim_Surface *s, int *w, int *h);
EAPI void enesim_surface_size_set(Enesim_Surface *s, int w, int h);
EAPI Enesim_Surface_Format enesim_surface_format_get(const Enesim_Surface *s);
EAPI void enesim_surface_data_get(const Enesim_Surface *s, Enesim_Surface_Data *sdata);
EAPI void enesim_surface_data_set(Enesim_Surface *s, const Enesim_Surface_Data *sdata);
EAPI void enesim_surface_data_increment(Enesim_Surface_Data *sdata, Enesim_Surface_Format sfmt, unsigned int len);
EAPI void enesim_surface_delete(Enesim_Surface *s);
/** @} */ //End of Enesim_Surface_Group


#endif /*ENESIM_SURFACE_H_*/
