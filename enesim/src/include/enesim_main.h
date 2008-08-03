#ifndef ENESIM_CORE_H_
#define ENESIM_CORE_H_

/**
 * TODO every function that expects a color must be cnahed to this
 * type
 * The color is premultiplied!!!!!!
 */
typedef uint32_t Enesim_Color;

/*
 * 
 */
typedef enum
{
	ENESIM_BLEND, /* D = S + D(1 - Sa) */
	ENESIM_FILL, /* D = S */
	ENESIM_ROPS
} Enesim_Rop;

EAPI void enesim_color_get(Enesim_Color *color, uint8_t a, uint8_t r, uint8_t g, uint8_t b);

/*
 * TODO rename this file to enesim_main.h
 */
EAPI int enesim_init(void);
EAPI void enesim_shutdown(void);

/* TODO
 * remove this error handling and use eina's approach */
/*
 * TODO normalize this errors
 *
 */
typedef enum
{
	ENESIM_ERROR_HANDLE_INVALID 	= 1,
	ENESIM_ERROR_SCANLINE_NOT_SUPPORTED,
	ENESIM_ERROR_GEOMETRY_INVALID,
	ENESIM_ERROR_FROMAT_NOT_SUPPORTED,
	ENESIM_ERROR_SRCRECT_INVALID,
	ENESIM_ERROR_DSTRECT_INVALID,
	ENESIM_ERROR_TRANSFORMATION_NOT_SUPPORTED,
} Enesim_Error;


/**
 * 
 */
extern unsigned int enesim_err;

EAPI char * enesim_error_to_str(unsigned int err);

#endif /*ENESIM_CORE_H_*/
