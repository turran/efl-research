#include "Ekeko.h"
#include "ekeko_private.h"

/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
Ekeko_Tiler *ekeko_tiler_new(int type, int w, int h)
{
	Ekeko_Tiler *t;
	Ekeko_Tiler_Class *c;
	
	switch (type)
	{
	case EKEKO_TILER_SPLIT_RECTANGLE:
	default:
		return NULL;
	}
	
	t = calloc(1, sizeof(Ekeko_Tiler));
	t->data = c->new(w, h);
	
	return t;
}