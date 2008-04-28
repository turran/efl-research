#include "esvg_common.h"
#include "ESVG.h"
#include "esvg_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
struct _ESVG
{
	Ekeko_Canvas *canvas;
};

static int _flush(void *data, Eina_Rectangle *rects)
{
	ESVG *e = data;
	Eina_Inlist *l;

	for (l = (Eina_Inlist *)rects; l; l = l->next)
	{
		Eina_Rectangle *r = (Eina_Rectangle *)l;
		
	}
	return 1; 
}

static Ekeko_Canvas_Class _canvas_class = {
	.flush = _flush,
};

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
EAPI ESVG * esvg_new(int w, int h)
{
	ESVG *e;
	
	if ((w < 0) || (h < 0)) return NULL;
	
	e = calloc(1, sizeof(ESVG));
	/* setup the graphics surface */
	/* setup the layout */
	e->canvas = ekeko_canvas_new(&_canvas_class, e, EKEKO_TILER_SPLITTER, w, h);

	return e;
}

EAPI void esvg_free(ESVG *e)
{
	
}
