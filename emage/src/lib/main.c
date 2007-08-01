#include "Emage.h"
#include "emage_private.h"

static int init = 0;

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/

EAPI int emage_init(void)
{
	if (++init != 1) return init;
	emage_compositor_init();
	/* FIXME */
	evas_common_cpu_init();
	//evas_common_convert_init();

	return init;
}

EAPI void emage_shutdown(void)
{
	if (--init) return;
}
