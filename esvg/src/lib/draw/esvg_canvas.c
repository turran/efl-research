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

	return 1; 
}

static Ekeko_Canvas_Class _canvas_class = {
	.flush = _flush,
};
/* called when a shape of the canvas has changed size or position */
static void _shape_changed(Ekeko_Canvas *c, Ekeko_Object *o, Ekeko_Event *ev, void *data)
{
	ESVG *svg = data;
	Eina_Rectangle r;
	
	ekeko_object_geometry_get(o, &r);
	eina_rectangle_union(&svg->shape_area, &r);
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
void esvg_canvas_shape_add(ESVG *svg, ESVG_Shape *s, Ekeko_Object_Class *class, void *data)
{
	/* TODO make a shape_init function */
	s->canvas = svg;
	s->object = ekeko_object_add(svg->canvas, class, data);
	s->engine.context = esvg_engine_context_new(svg);
	s->attributes.opacity.stroke_opacity = 1;
	s->attributes.opacity.fill_opacity = 1;
	ekeko_object_show(s->object);
	/* in case of resize/move a shape recalc the final canvas area */
	ekeko_object_event_callback_add(s->object, EKEKO_EVENT_MOVE, _shape_changed, svg);
	ekeko_object_event_callback_add(s->object, EKEKO_EVENT_RESIZE, _shape_changed, svg);
}
void esvg_canvas_shape_remove(ESVG_Shape *s)
{
	/* TODO remove the ekeko object */
	/* TODO remove the context */ 
}
/* TODO
void esvg_canvas_resize_register()
*/
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 * TODO change w and h 
 */
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
	/* initialize the shape area to 0 */
	eina_rectangle_coords_from(&e->shape_area, 0, 0, 0, 0);
	/* if the size of the canvas is relative we should add a callback for every
	 * object's resize or move
	 */
	e->background = esvg_rect_add(e);
	/* TODO define the rectangle
	 * the size should be absolute!! or we end with an endless lopp
	//esvg_rect_geometry_set(e->rect, 0, 0, w, h);
	 */
	/* TODO add callbacks to the background, if it changes
	 * resize the canvas */
	return e;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI esvg_damage_add(ESVG *e, ESVG_Coord_Value x, ESVG_Coord_Value y, ESVG_Length_Value w, ESVG_Length_Value h)
{
	Eina_Rectangle r;
	
	eina_rectangle_coords_from(&r, x, y, w, h);
	ekeko_canvas_damage_add(e->canvas, &r);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_render(ESVG *e)
{
	ekeko_canvas_process(e->canvas);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_free(ESVG *e)
{
	
}

/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void esvg_size_set(ESVG *e, ESVG_Length *w, ESVG_Length *h)
{
	/* */
}

/* viewport functions */
