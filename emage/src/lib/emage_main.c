#include "Emage.h"

static int init = 0;

EAPI int emage_init(void)
{
	if (++init != 1) return init;
	evas_common_cpu_init();
	evas_common_blend_init();
	evas_common_convert_init();

	return init;
}

EAPI void emage_shutdown(void)
{
	if (--init) return;
}
