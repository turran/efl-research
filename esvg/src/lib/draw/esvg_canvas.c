#include "esvg_common.h"
#include "ESVG.h"
#include "esvg_private.h"

/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static int _flush(void *data, Eina_Rectangle *rects)
{
	ESVG *e = data;
	Eina_Inlist *l;

	for (l = (Eina_Inlist *)rects; l; l = l->next)
	{
		Eina_Rectangle *r = (Eina_Rectangle *)l;
		printf("flushing\n");
	}
	return 1; 
}

static Ekeko_Canvas_Class _canvas_class = {
	.flush = _flush,
};
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
void esvg_canvas_shape_add(ESVG *svg, ESVG_Shape *s, Ekeko_Object_Class *class, void *data)
{
	s->canvas = svg;
	s->object = ekeko_object_add(svg->canvas, class, data);
	s->engine.context = esvg_engine_context_new(svg);
	ekeko_object_show(s->object);
	svg->shapes = eina_inlist_append(svg->shapes, s);
}
void esvg_canvas_shape_remove(ESVG_Shape *s)
{
	/* TODO remove the ekeko object */
	/* TODO remove the context */ 
}
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
EAPI ESVG * esvg_new(int w, int h, ESVG_Engine_Type type, void *engine_data)
{
	ESVG *e;
	
	if ((w < 0) || (h < 0)) return NULL;
	
	e = calloc(1, sizeof(ESVG));
	e->canvas = ekeko_canvas_new(&_canvas_class, e, EKEKO_TILER_SPLITTER, w, h);
	e->engine_data = engine_data;
	switch (type)
	{
		case ESVG_ENGINE_CAIRO:
		e->engine = &esvg_engine_cairo;
		break;
		
		default:
		break;
	}
	return e;
}

EAPI void esvg_render(ESVG *e)
{
	ekeko_canvas_process(e->canvas);
}

EAPI void esvg_free(ESVG *e)
{
	
}

/* viewport functions */
