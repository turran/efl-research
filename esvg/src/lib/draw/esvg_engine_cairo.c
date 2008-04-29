#include "esvg_common.h"
#include "ESVG.h"
#include "esvg_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void * _context_new(void *engine_data)
{
	cairo_t *c;
	cairo_surface_t *cs = engine_data;
	
	c = cairo_create(cs);
	
	return c;
}

static void _context_free(void *engine_data, void *cdata)
{
	cairo_t *c = cdata;
	cairo_destroy(c);
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
ESVG_Engine esvg_engine_cairo = {
	.context_new = _context_new,
	.context_free = _context_free,
};
