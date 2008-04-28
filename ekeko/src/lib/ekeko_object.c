#include "Ekeko.h"
#include "ekeko_private.h"
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
static void _object_change(Ekeko_Object *o)
{
	if (o->changed == EINA_TRUE)
		return;
	o->changed = EINA_TRUE;
	ekeko_canvas_change(o->canvas);
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
void ekeko_object_event_callback_call(Ekeko_Object *o, Ekeko_Event_Type ect, Ekeko_Event *ev)
{
	Eina_Inlist *l;
	
	for (l = (Eina_Inlist *)o->callbacks[ect]; l; l = l->next)
	{
		Ekeko_Object_Cb *cb;
		cb = (Ekeko_Object_Cb *)l;
		
		printf("calling callback for event %d\n", ect);
		cb->func(o->canvas, o, ev, cb->data);
	}
}
/**
 * To be documented
 * FIXME: To be fixed
 */
Ekeko_Object * ekeko_object_get_at_coordinate(Ekeko_Object *o, unsigned int x, unsigned int y)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
Eina_Bool ekeko_object_is_inside(Ekeko_Object *o, Ekeko_Rectangle *r, Eina_Rectangle *drect)
{
	drect->x = o->curr.geometry.x;
	drect->y = o->curr.geometry.y;
	drect->w = o->curr.geometry.w;
	drect->h = o->curr.geometry.h;

	return eina_rectangle_rectangle_intersection_get(drect, &r->r);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
void ekeko_object_pre_process(Ekeko_Object *o)
{
	/* check visibility */
	if (o->curr.visible ^ o->prev.visible)
	{
		ekeko_tiler_rect_add(o->canvas->tiler, &o->prev.geometry);
		ekeko_tiler_rect_add(o->canvas->tiler, &o->curr.geometry);
		/* update the visibility */
		o->prev.visible = o->curr.visible;
		goto ok;
	}
	/* check geometry */
	if ((o->curr.geometry.x != o->prev.geometry.x) ||
			(o->curr.geometry.y != o->prev.geometry.y) ||
			(o->curr.geometry.w != o->prev.geometry.w) ||
			(o->curr.geometry.h != o->prev.geometry.h))
	{
		/* add both rectangles, previous and new */
		//printf("adding[1] %d %d %d %d\n", o->prev.geometry.x, o->prev.geometry.y, o->prev.geometry.w, o->prev.geometry.h);
		ekeko_tiler_rect_add(o->canvas->tiler, &o->prev.geometry);
		//printf("adding[2] %d %d %d %d\n", o->curr.geometry.x, o->curr.geometry.y, o->curr.geometry.w, o->curr.geometry.h);
		ekeko_tiler_rect_add(o->canvas->tiler, &o->curr.geometry);
		/* update the geometry */
		o->prev.geometry.x = o->curr.geometry.x;
		o->prev.geometry.y = o->curr.geometry.y;
		o->prev.geometry.w = o->curr.geometry.w;
		o->prev.geometry.h = o->curr.geometry.h;
		goto ok;
			
	}
	return;
	/* call class function */
ok:
	o->oclass->pre_process(o->cdata);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
void ekeko_object_process(Ekeko_Object *o, Eina_Rectangle *r)
{
	/* call class function */
	o->oclass->process(o->cdata, r);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
void ekeko_object_post_process(Ekeko_Object *o)
{	
	/* call class function */
	o->changed = EINA_FALSE;
	o->oclass->post_process(o->cdata);
}
/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 * FIXME this function and the object class create aren't too good, refactor them :)
 */
EAPI Ekeko_Object * ekeko_object_add(Ekeko_Canvas *c, Ekeko_Object_Class *oclass, void *cdata)
{
	Ekeko_Object *o;

	assert(o);
	assert(oclass);
	
	o = calloc(1, sizeof(Ekeko_Object));
	o->canvas = c;
	
	/* default object properties */
	o->curr.visible = EINA_FALSE; 
	
	/* TODO check the class */
	o->oclass = oclass;
	o->cdata = cdata;
	
	c->objects = eina_inlist_append(c->objects, o);
	return o;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_object_move(Ekeko_Object *o, int x, int y)
{
	Eina_Inlist *l;
	
	assert(o);
	// FIXME check states
	o->curr.geometry.x = x;
	o->curr.geometry.y = y;
	_object_change(o);
	
	for (l = (Eina_Inlist *)o->canvas->inputs; l; l = l->next)
	{
		Ekeko_Input *i = (Ekeko_Input *)l;
		
		/* if the object is inside any input, call the callback */
		if (eina_rectangle_coords_inside(&o->curr.geometry, i->pointer.x, i->pointer.y))
		{
			ekeko_input_feed_mouse_move(i, i->pointer.x, i->pointer.y, i->last_timestamp);
		}
		/* if the object was one of the inside objects, call the callback */
		else if (o == i->pointer.obj)
		{
			ekeko_input_feed_mouse_move(i, i->pointer.x, i->pointer.y, i->last_timestamp);
		}
	}
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_object_show(Ekeko_Object *o)
{
	assert(o);
	o->curr.visible = EINA_TRUE;
	_object_change(o);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_object_hide(Ekeko_Object *o)
{
	assert(o);
	o->curr.visible = EINA_FALSE;
	_object_change(o);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_object_resize(Ekeko_Object *o, int w, int h)
{
	assert(o);
	// FIXME check states
	o->curr.geometry.w = w;
	o->curr.geometry.h = h;
	_object_change(o);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_object_stack_above(Ekeko_Object *o, Ekeko_Object *object_rel)
{
	assert(o);
	assert(object_rel);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_object_stack_below(Ekeko_Object *o, Ekeko_Object *object_rel)
{
	assert(o);
	assert(object_rel);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void * ekeko_object_class_data_get(Ekeko_Object *o)
{
	assert(o);
	
	return o->cdata;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ekeko_Canvas * ekeko_object_canvas_get(Ekeko_Object *o)
{
	assert(o);
	
	return o->canvas;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_object_geometry_get(Ekeko_Object *o, Eina_Rectangle *r)
{
	assert(o);
	assert(r);
	
	r->x = o->curr.geometry.x;
	r->y = o->curr.geometry.y;
	r->w = o->curr.geometry.w;
	r->h = o->curr.geometry.h;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool ekeko_object_is_visible(Ekeko_Object *o)
{
	return o->curr.visible;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_object_event_callback_add(Ekeko_Object *o, Ekeko_Event_Type etype, Ekeko_Event_Cb cb, void *data)
{
	Ekeko_Object_Cb *ocb;
	
	ocb = malloc(sizeof(Ekeko_Object_Cb));
	ocb->func = cb;
	ocb->data = data;
	
	o->callbacks[etype] = eina_inlist_append(o->callbacks[etype], ocb);
}
