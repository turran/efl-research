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
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
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
/**
 * To be documented
 * FIXME: To be fixed
 */
void ekeko_object_pre_process(Ekeko_Object *o)
{
	/* check visibility */
	
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
void ekeko_object_process(Ekeko_Object *o, Enesim_Rectangle *r)
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
	// FIXME check states
	o->curr.geometry.x = x;
	o->curr.geometry.y = y;
	_object_change(o);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_object_show(Ekeko_Object *o)
{
	o->curr.visible = EINA_TRUE;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_object_hide(Ekeko_Object *o)
{
	o->curr.visible = EINA_FALSE;
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
	_object_change(o);
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
EAPI void * ekeko_object_class_data_get(Ekeko_Object *o)
{
	return o->cdata;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ekeko_Canvas * ekeko_object_canvas_get(Ekeko_Object *o)
{
	return o->canvas;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_object_geometry_get(Ekeko_Object *o, Enesim_Rectangle *r)
{
	r->x = o->curr.geometry.x;
	r->y = o->curr.geometry.y;
	r->w = o->curr.geometry.w;
	r->h = o->curr.geometry.h;
}