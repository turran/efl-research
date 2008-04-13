#ifndef ENESIM_CORE_H_
#define ENESIM_CORE_H_

/*
 * 
 */
typedef enum
{
	ENESIM_BLEND,
	ENESIM_ROPS
} Enesim_Rop;
/*
 * TODO normalize this errors
 *
 */
enum
{
	ENESIM_ERROR_NONE 	= 1,
	ENESIM_ERROR_INVAL,
	ENESIM_ERROR_IO,
	ENESIM_ERRORS
};

typedef unsigned int 		DATA32;
typedef unsigned short int 	DATA16;
typedef unsigned char 		DATA8;

#endif /*ENESIM_CORE_H_*/
