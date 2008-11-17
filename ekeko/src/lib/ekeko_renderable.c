#include "Ekeko.h"
#include "ekeko_private.h"
/* TODO
 * + move this to locatable or renderable
 * + this should be an abstract class, i.e no namespace or name, only
 * abstract functions so the user has to fill those functions, what about
 * namespace and name then? how a canvas can know when a child can be appended
 * here? check if an renderable inherits from a renderable / locatable renderable?
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static inline Eina_Bool _renderable_is_valid(Ekeko_Renderable *o)
{
	return o->valid;
}
static void _renderable_change(Ekeko_Renderable *o)
{
	if (o->changed == EINA_TRUE)
		return;
	o->changed = EINA_TRUE;
	if (o->valid)
	{
		printf("canvas changed!!\n");
		ekeko_canvas_change(o->canvas);
	}
}
static void _rect_pre_cb(Ekeko_Event *e)
{
	// the input events
	/* FIXME this should happen only on the valid renderables */
#if 0
	for (l = (Eina_Inlist *)o->canvas->inputs; l; l = l->next)
	{
		Ekeko_Input *i = (Ekeko_Input *)l;

		/* if the renderable is inside any input, call the callback */
		if (eina_rectangle_coords_inside(&o->curr.geometry,
			i->pointer.x, i->pointer.y))
		{
			ekeko_input_feed_mouse_move(i, i->pointer.x,
					i->pointer.y, i->last_timestamp);
		}
		/* if the renderable was one of the inside renderables, call the callback */
		else if (o == i->pointer.obj)
		{
			ekeko_input_feed_mouse_move(i, i->pointer.x,
					i->pointer.y, i->last_timestamp);
		}
	}
#endif
}
static void _rect_mutation_cb(Ekeko_Event *e)
{
	/* add both rectangles, previous and new */
	//printf("adding[1] %d %d %d %d\n", o->prev.geometry.x, o->prev.geometry.y, o->prev.geometry.w, o->prev.geometry.h);
	//ekeko_tiler_rect_add(o->canvas->tiler, &o->prev.geometry);
	//printf("adding[2] %d %d %d %d\n", o->curr.geometry.x, o->curr.geometry.y, o->curr.geometry.w, o->curr.geometry.h);
	//ekeko_tiler_rect_add(o->canvas->tiler, &o->curr.geometry);
}
static void _visible_pre_cb(Ekeko_Event *e)
{
	// the input events
}
static void _visible_mutation_cb(Ekeko_Event *e)
{
	//ekeko_tiler_rect_add(o->canvas->tiler, &o->prev.geometry);
	//ekeko_tiler_rect_add(o->canvas->tiler, &o->curr.geometry);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#if 0
void ekeko_renderable_event_callback_call(Ekeko_Renderable *o, Ekeko_Event_Type ect, Ekeko_Event *ev)
{
	Eina_Inlist *l;

	for (l = (Eina_Inlist *)o->callbacks[ect]; l; l = l->next)
	{
		Ekeko_Renderable_Cb *cb;
		cb = (Ekeko_Renderable_Cb *)l;

		//printf("calling callback for event %d\n", ect);
		cb->func(o->canvas, o, ev, cb->data);
	}
}
#endif
/**
 * To be documented
 * FIXME: To be fixed
 */
void ekeko_renderable_event_move_call(Ekeko_Renderable *o)
{
	Ekeko_Event ev;

	//ekeko_renderable_event_callback_call(o, EKEKO_EVENT_MOVE, &ev);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
void ekeko_renderable_event_resize_call(Ekeko_Renderable *o)
{
	Ekeko_Event ev;

	//ekeko_renderable_event_callback_call(o, EKEKO_EVENT_RESIZE, &ev);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
Eina_Bool ekeko_renderable_intersection_get(Ekeko_Renderable *o, Ekeko_Rectangle *r, Eina_Rectangle *drect)
{
	drect->x = o->curr.geometry.x;
	drect->y = o->curr.geometry.y;
	drect->w = o->curr.geometry.w;
	drect->h = o->curr.geometry.h;

	return eina_rectangle_intersection(drect, &r->r);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
void ekeko_renderable_pre_process(Ekeko_Renderable *o)
{
	if (!o->valid)
		printf("error pre processing an invalid renderable\n");
	/* check visibility */
	if (o->curr.visible ^ o->prev.visible)
	{
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
void ekeko_renderable_process(Ekeko_Renderable *o, Eina_Rectangle *r)
{
	/* call class function */
	o->oclass->process(o->cdata, r);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
void ekeko_renderable_post_process(Ekeko_Renderable *o)
{
	/* call class function */
	o->changed = EINA_FALSE;
	o->oclass->post_process(o->cdata);
}
/**
 * Validate an renderable, add the renderable to the list of valid renderables
 * FIXME: To be fixed
 */
void ekeko_renderable_validate(Ekeko_Renderable *o)
{
	printf("validating\n");
	if (o->valid)
		return;
	o->valid = EINA_TRUE;
	o->canvas->valid = eina_list_append(o->canvas->valid, o);
	o->canvas->invalid = eina_list_remove(o->canvas->invalid, o);
}
/**
 * Invalidate an renderable, remove the renderable from the list of valid renderables
 * FIXME: To be fixed
 */
void ekeko_renderable_invalidate(Ekeko_Renderable *o)
{
	printf("invalidating\n");
	if (!o->valid)
		return;
	o->valid = EINA_FALSE;
	o->canvas->invalid = eina_list_append(o->canvas->invalid, o);
	o->canvas->valid = eina_list_remove(o->canvas->valid, o);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/* TODO this should also receive the parent document */
EAPI void ekeko_renderable_new(Ekeko_Element *e, Ekeko_Renderable_Class *c)
{
	Ekeko_Value def;

	/* setup the attributes for a renderable element */
	eina_rectangle_coords_from(&def.v.r, 0, 0, 0, 0);
	ekeko_element_attribute_add(e, "_rect", EKEKO_ATTRIBUTE_RECTANGLE, &def);
	ekeko_event_listener_add((Ekeko_Node *)e, "pre_mutation", _rect_pre_cb, EINA_TRUE);
	ekeko_event_listener_add((Ekeko_Node *)e, "mutation", _rect_mutation_cb, EINA_TRUE);
	def.v.b = EINA_FALSE;
	ekeko_element_attribute_add(e, "_visible", EKEKO_ATTRIBUTE_BOOL, &def);
	ekeko_event_listener_add((Ekeko_Node *)e, "pre_mutation", _visible_pre_cb, EINA_TRUE);
	ekeko_event_listener_add((Ekeko_Node *)e, "mutation", _visible_mutation_cb, EINA_TRUE);
	/* setup the private class */
	ekeko_node_user_set((Ekeko_Node *)e, "_renderable_class", c);
	/* TODO the parent document should implement the canvas interface? */
}

/**
 * To be documented
 * FIXME: To be fixed
 * FIXME this function and the renderable class create aren't too good, refactor them :)
 */
EAPI Ekeko_Renderable * ekeko_renderable_add(Ekeko_Canvas *c, Ekeko_Renderable_Class *oclass, void *cdata)
{
	Ekeko_Renderable *o;

	assert(oclass);

	o = calloc(1, sizeof(Ekeko_Renderable));
	assert(o);
	o->canvas = c;

	/* default renderable properties */
	o->curr.visible = EINA_FALSE;

	/* TODO check the class */
	o->oclass = oclass;
	o->cdata = cdata;

	c->renderables = eina_inlist_append(c->renderables, EINA_INLIST_GET(o));
	return o;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_renderable_delete(Ekeko_Renderable *o)
{
	/* TODO change the valid/invalid lists */
	/* update the renderable's list */
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_renderable_change_notify(Ekeko_Renderable *o)
{
	assert(o);
	_renderable_change(o);
}
/**
 * @brief Moves an renderable to the given coordinate
 * @param o Renderable to move
 * @param x X coordinate
 * @param y Y coordinate
 */
EAPI void ekeko_renderable_move(Ekeko_Renderable *o, int x, int y)
{
	Eina_Inlist *l;

	assert(o);

	if ((o->curr.geometry.x == x) && (o->curr.geometry.y == y))
	{
		goto callback;
	}
	o->curr.geometry.x = x;
	o->curr.geometry.y = y;
	/* valid */
	if (eina_rectangles_intersect(&o->curr.geometry, &o->canvas->size))
		ekeko_renderable_validate(o);
	else
		ekeko_renderable_invalidate(o);
	_renderable_change(o);
	/* FIXME this should happen only on the valid renderables */
	for (l = (Eina_Inlist *)o->canvas->inputs; l; l = l->next)
	{
		Ekeko_Input *i = (Ekeko_Input *)l;

		/* if the renderable is inside any input, call the callback */
		if (eina_rectangle_coords_inside(&o->curr.geometry, i->pointer.x, i->pointer.y))
		{
			ekeko_input_feed_mouse_move(i, i->pointer.x, i->pointer.y, i->last_timestamp);
		}
		/* if the renderable was one of the inside renderables, call the callback */
		else if (o == i->pointer.obj)
		{
			ekeko_input_feed_mouse_move(i, i->pointer.x, i->pointer.y, i->last_timestamp);
		}
	}
callback:
	ekeko_renderable_event_move_call(o);
}
/**
 * @brief Resize an renderable
 * @param w New width of the renderable
 * @param h New height of the renderable
 */
EAPI void ekeko_renderable_resize(Ekeko_Renderable *o, int w, int h)
{
	assert(o);
	if ((o->curr.geometry.w == w) && (o->curr.geometry.h == h))
	{
		goto callback;
	}
	o->curr.geometry.w = w;
	o->curr.geometry.h = h;
	if (eina_rectangles_intersect(&o->curr.geometry, &o->canvas->size))
		ekeko_renderable_validate(o);
	else
		ekeko_renderable_invalidate(o);
	_renderable_change(o);
callback:
	ekeko_renderable_event_resize_call(o);
}
/**
 * @brief Shows the renderable on the canvas
 * @param o Renderable to show
 */
EAPI void ekeko_renderable_show(Ekeko_Renderable *o)
{
	assert(o);
	o->curr.visible = EINA_TRUE;
	_renderable_change(o);
//callback:
}
/**
 * @brief Hides the renderable from the canvas
 * @param o Renderable to hide
 */
EAPI void ekeko_renderable_hide(Ekeko_Renderable *o)
{
	assert(o);
	o->curr.visible = EINA_FALSE;
	_renderable_change(o);
//callback:
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_renderable_stack_above(Ekeko_Renderable *o, Ekeko_Renderable *renderable_rel)
{
	assert(o);
	assert(renderable_rel);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_renderable_stack_below(Ekeko_Renderable *o, Ekeko_Renderable *renderable_rel)
{
	assert(o);
	assert(renderable_rel);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void * ekeko_renderable_class_data_get(Ekeko_Renderable *o)
{
	assert(o);

	return o->cdata;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ekeko_Canvas * ekeko_renderable_canvas_get(Ekeko_Renderable *o)
{
	assert(o);

	return o->canvas;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_renderable_geometry_get(Ekeko_Renderable *o, Eina_Rectangle *r)
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
EAPI Eina_Bool ekeko_renderable_is_visible(Ekeko_Renderable *o)
{
	return o->curr.visible;
}
#if 0
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_renderable_event_callback_add(Ekeko_Renderable *o, Ekeko_Event_Type etype, Ekeko_Event_Cb cb, void *data)
{
	Ekeko_Renderable_Cb *ocb;

	ocb = malloc(sizeof(Ekeko_Renderable_Cb));
	ocb->func = cb;
	ocb->data = data;

	o->callbacks[etype] = eina_inlist_append(o->callbacks[etype], EINA_INLIST_GET(ocb));
}
#endif
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ekeko_Renderable * ekeko_renderable_rel_get_up(Ekeko_Renderable *rel, Ekeko_Renderable_Cmp_Func cmp, void *data)
{
	Eina_Inlist *l;

	assert(rel);
	assert(cmp);

	for (l = (Eina_Inlist *)rel; l; l = l->prev)
	{
		Ekeko_Renderable *o = (Ekeko_Renderable*)l;
		if (cmp(o, data))
			return o;
	}
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ekeko_Renderable * ekeko_renderable_rel_get_down(Ekeko_Renderable *rel, Ekeko_Renderable_Cmp_Func cmp, void *data)
{
	Eina_Inlist *l;

	assert(rel);
	assert(cmp);

	for (l = (Eina_Inlist *)rel; l; l = l->next)
	{
		Ekeko_Renderable *o = (Ekeko_Renderable*)l;
		if (cmp(o, data))
			return o;
	}
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool ekeko_renderable_is_inside(Ekeko_Renderable *o, Eina_Rectangle *r)
{
	if (!(ekeko_renderable_geometry_is_inside(o, r)))
		return EINA_FALSE;
	
	if (!(o->oclass->is_inside))
		return EINA_TRUE;
	return o->oclass->is_inside(&o->curr.geometry, r);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Eina_Bool ekeko_renderable_geometry_is_inside(Ekeko_Renderable *o, Eina_Rectangle *r)
{
	return eina_rectangles_intersect(&o->curr.geometry, r);
}
