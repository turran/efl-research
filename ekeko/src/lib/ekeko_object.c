#include "Ekeko.h"
#include "ekeko_private.h"
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
Eina_Bool ekeko_object_changed(Ekeko_Object *o)
{

}
/**
 * To be documented
 * FIXME: To be fixed
 */
Eina_Bool ekeko_object_is_inside(Ekeko_Object *o, Ekeko_Rectangle *r, Enesim_Rectangle *drect)
{
	drect->x = o->curr.geometry.x;
	drect->y = o->curr.geometry.y;
	drect->w = o->curr.geometry.w;
	drect->h = o->curr.geometry.h;

	return enesim_rectangle_rectangle_intersection_get(drect, &r->r);
}


/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ekeko_Object * ekeko_object_add(Ekeko_Canvas *c, Ekeko_Object_Class *oclass)
{
	Ekeko_Object *o;

	o = calloc(1, sizeof(Ekeko_Object));
	o->canvas = c;
	
	/* default object properties */
	o->curr.visible = EINA_FALSE; 
	
	/* TODO check the class */
	o->oclass = oclass;
	o->oclass->create(o);
	c->objects = eina_inlist_append(c->objects, o);
	return o;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_object_move(Ekeko_Object *o, int x, int y)
{
	// FIXME check states
	o->curr.geometry.x = x;
	o->curr.geometry.y = y;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_object_show(Ekeko_Object *o)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_object_hide(Ekeko_Object *o)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_object_resize(Ekeko_Object *o, int w, int h)
{
	// FIXME check states
	o->curr.geometry.w = w;
	o->curr.geometry.h = h;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_object_stack_above(Ekeko_Object *o, Ekeko_Object *object_rel)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_object_stack_below(Ekeko_Object *o, Ekeko_Object *object_rel)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_object_data_set(Ekeko_Object *o, void *data)
{
	o->data = data;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void * ekeko_object_data_get(Ekeko_Object *o)
{
	return o->data;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ekeko_Canvas * ekeko_object_canvas_get(Ekeko_Object *o)
{
	return o->canvas;
}

