#ifndef ENESIM_CORE_H_
#define ENESIM_CORE_H_

/*
 * 
 */
typedef enum
{
	ENESIM_BLEND,
	ENESIM_FILL,
	ENESIM_ROPS
} Enesim_Rop;
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


typedef unsigned int 		DATA32;
typedef unsigned short int 	DATA16;
typedef unsigned char 		DATA8;

EAPI char * enesim_error_to_str(unsigned int err);

#endif /*ENESIM_CORE_H_*/
