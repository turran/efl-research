#include <stdlib.h>

#include "Edata.h"
#include "Enesim.h"
#include "enesim_private.h"
#include "scanline.h"

/**
 * Aliased scanline
 *
 */

typedef struct _Alias_Sl
{
	int y;
	int x;
	int w;
} Alias_Sl;

typedef struct _Alias
{
	Edata_Array 	*a;
	Alias_Sl	*sls;
	int 		num_sls;
} Alias;

static void _a_alloc(Alias *n, int num)
{
	n->sls = realloc(n->sls, num * sizeof(Alias_Sl));
}

static void _a_free(Alias *n)
{
	free(n->sls);
}

static void _sl_free(Alias *n)
{
	edata_array_free(n->a);
	free(n);
}

static void _sl_add(Alias *n, int x0, int x1, int y, int coverage)
{
	edata_array_element_new(n->a);
	n->sls[n->num_sls].y = y;
	n->sls[n->num_sls].x = x0;
	n->sls[n->num_sls].w = x1 - x0 + 1;
	n->num_sls++;
}

Enesim_Scanline_Func naa = {
	.free	= _sl_free,
	.add 	= _sl_add
};

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Enesim_Scanline * enesim_scanline_alias_new(void)
{
	Enesim_Scanline *sl;
	Alias *n;

	n = calloc(1, sizeof(Alias));
	n->a = edata_array_new(n, EDATA_ARRAY_ALLOC(_a_alloc),
		EDATA_ARRAY_FREE(_a_free));

	sl = enesim_scanline_new();
	sl->funcs = &naa;
	sl->anti_alias = 0;
	sl->data = n;
	return sl;
}
