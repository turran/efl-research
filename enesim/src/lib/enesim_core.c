#include "enesim_common.h"
#include "Enesim.h"
#include "enesim_private.h"
/**
 * TODO remove this error handling, use eina's 
 */
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
void enesim_error_set(unsigned int err)
{
	enesim_err = err;
}
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
unsigned int enesim_err;
/**
 * 
 */
EAPI char * enesim_error_to_str(unsigned int err)
{
	switch (err)
	{
	
	case ENESIM_ERROR_HANDLE_INVALID:
		return "ERROR_INVALID_HANDLE";
	
	case ENESIM_ERROR_SCANLINE_NOT_SUPPORTED:
		return "ENESIM_ERROR_SCANLINE_NOT_SUPPORTED";
	
	case ENESIM_ERROR_GEOMETRY_INVALID:
		return "ENESIM_ERROR_GEOMETRY_INVALID";
		
	case ENESIM_ERROR_FROMAT_NOT_SUPPORTED:
		return "ENESIM_ERROR_FROMAT_NOT_SUPPORTED";
	}
	return NULL;
}
