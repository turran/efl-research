#include <stdlib.h>

#include "Edata.h"
#include "Enesim.h"
#include "enesim_private.h"
#include "scanline.h"

/**
 * Naa, No Anti-Alias
 *
 */

typedef struct _Naa_Sl
{
	int y;
	int x;
	int w;
} Naa_Sl;

typedef struct _Naa
{
	Edata_Array 	*a;
	Naa_Sl 		*sls;
	int 		num_sls;
} Naa;

static void _a_alloc(Naa *n, int num)
{
	n->sls = realloc(n->sls, num * sizeof(Naa_Sl));
}

static void _a_free(Naa *n)
{
	free(n->sls);
}

static void _sl_free(Naa *n)
{
	edata_array_free(n->a);
	free(n);
}

static void *_sl_create(void)
{
	Naa *n;

	n = malloc(sizeof(Naa));
	n->a = edata_array_new(n, EDATA_ARRAY_ALLOC(_a_alloc),
		EDATA_ARRAY_FREE(_a_free));
	return n;
}

static void _sl_add(Naa *n, int x0, int x1, int y, int coverage)
{
	edata_array_element_new(n->a);
	n->sls[n->num_sls].y = y;
	n->sls[n->num_sls].x = x0;
	n->sls[n->num_sls].w = x1 - x0 + 1;
	n->num_sls++;
}

Enesim_Scanline_Func naa = {
	.create = _sl_create,
	.free	= _sl_free,
	.add 	= _sl_add
};
