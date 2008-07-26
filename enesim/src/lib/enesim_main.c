#include "enesim_common.h"
#include "Enesim.h"
#include "enesim_private.h"

static int _init = 0;

/**
 * 
 */
EAPI int enesim_init(void)
{
	if (!_init++)
	{
		/* initialize the drawer */
		enesim_drawer_init();
	}
	return _init;
}
/**
 * 
 */
EAPI void enesim_shutdown(void)
{
	
}
