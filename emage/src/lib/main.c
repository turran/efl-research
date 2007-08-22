#include "Emage.h"
#include "emage_private.h"

static int init = 0;

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/

/**
 * Initialize the library
 * @return 1 or greater on success, 0 otherwise
 *
 * This function initializes the library, must be the first function to be
 * called. 
 */
EAPI int
emage_init(void)
{
	if (++init != 1) return init;
	emage_compositor_init();
	/* FIXME */
	evas_common_cpu_init();

	return init;
}

/**
 * Shutdown the library
 *
 */
EAPI void
emage_shutdown(void)
{
	if (--init) return;
}
