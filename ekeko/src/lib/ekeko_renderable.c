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
struct _Ekeko_Renderable
{
	EINA_INLIST;
	Ekeko_Element *container; /* the container element */
	Ekeko_Canvas *canvas; /* the nearest parent canvas */
	Ekeko_Renderable_Class oclass;
	Eina_Bool delete_me;
	Eina_Bool changed;
	Eina_Bool valid;
	/* TODO
	 * add a way to pass the event to the renderables behind
	 */
};

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
		//printf("canvas changed!!\n");
		//ekeko_canvas_change(o->canvas);
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
static void _attr_updated_cb(Ekeko_Event *ee)
{
	Ekeko_Event_Mutation *e = (Ekeko_Event_Mutation *)ee;
	Ekeko_Renderable *r;

	r = ekeko_node_user_get(ee->current_target, RENDERABLE_PRIVATE);
	/* if the size has changed we should damage the whole canvas */
	if (!strcmp(e->attr, RENDERABLE_GEOMETRY))
	{
		ekeko_canvas_damage_add_internal(r->canvas, &e->prev->v.r);
		ekeko_canvas_damage_add_internal(r->canvas, &e->curr->v.r);
	}
	else if (!strcmp(e->attr, RENDERABLE_VISIBILITY))
	{
		ekeko_canvas_damage_add_internal(r->canvas, &e->prev->v.r);
		ekeko_canvas_damage_add_internal(r->canvas, &e->curr->v.r);
	}
}
static void _process_cb(Ekeko_Event *ee)
{
	printf("PROCESSSSSSSSSS\n");
}
static void _inserted_cb(Ekeko_Event *ee)
{
	Ekeko_Event_Mutation *e = (Ekeko_Event_Mutation *)ee;
	Ekeko_Node *parent;
	Ekeko_Renderable *r;
	Ekeko_Canvas *canvas;

	/* try to get the parent canvas */
	parent = e->related;
	while (parent && !(canvas = ekeko_node_user_get(parent, CANVAS_PRIVATE)))
	{
		parent = parent->parent;
	}
	if (!canvas)
	{
		printf("NO PARENT CANVAS\n");
		return;
	}
	r = ekeko_node_user_get(ee->current_target, RENDERABLE_PRIVATE);
	r->canvas = canvas;
	ekeko_canvas_renderable_add(canvas, r);
	/* TODO
	 * handle the case where another renderable is appended as child node of this
	 * handle the case where the renderable is removed from the canvas, etc
	 */
	printf("PARENT CANVAS\n");
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void ekeko_renderable_render(Ekeko_Renderable *r, Ekeko_Element *parent, Eina_Rectangle *er)
{
	r->oclass.render(parent, r->container, er);
}
void ekeko_renderable_render_all(Ekeko_Element *c, Eina_Inlist *renderables, Eina_Rectangle *er)
{
	Ekeko_Renderable *r;
	EINA_INLIST_FOREACH(renderables, r)
	{
		Ekeko_Value rr;
		Eina_Rectangle intersect;
		Eina_Bool opaque;

		ekeko_node_attribute_get((Ekeko_Node *)r->container, RENDERABLE_OPAQUE, &rr);
		opaque = rr.v.b;
		ekeko_node_attribute_get((Ekeko_Node *)r->container, RENDERABLE_GEOMETRY, &rr);
		intersect = rr.v.r;
		if (eina_rectangle_intersection(&intersect, er))
		{
			//printf("RENDERING %d\n", opaque);
			if (opaque)
			{
				
				ekeko_renderable_render(r, c, &intersect);
				//break;
			}
			else
			{
				/* TODO we need to render the objects below first */
			}
		}
		
	}
}
Eina_Inlist * ekeko_renderable_add(Eina_Inlist *renderables, Ekeko_Renderable *r)
{
	return eina_inlist_append(renderables, EINA_INLIST_GET(r));
}
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
#endif

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/* TODO this should also receive the parent document */
EAPI void ekeko_renderable_new(Ekeko_Element *e, Ekeko_Renderable_Class *c)
{
	Ekeko_Value def;
	Ekeko_Renderable *r;

	r = ekeko_node_user_get((Ekeko_Node *)e, RENDERABLE_PRIVATE);
	if (r) return;
	/* private data */
	r = calloc(1, sizeof(Ekeko_Renderable));
	r->container = e;
	r->oclass = *c;
	/* setup the attributes for a renderable element */
	ekeko_value_rectangle_coords_from(&def, 0, 0, 0, 0);
	ekeko_element_attribute_set(e, RENDERABLE_GEOMETRY, &def);
	ekeko_value_bool_from(&def, EINA_FALSE);
	ekeko_element_attribute_set(e, RENDERABLE_VISIBILITY, &def);
	ekeko_value_bool_from(&def, EINA_FALSE);
	ekeko_element_attribute_set(e, RENDERABLE_OPAQUE, &def);
	/* TODO check the element's parent, should it implement the canvas interface? */
	/* setup the events */
	ekeko_node_event_listener_add((Ekeko_Node *)e, "ProcessEvents",  _process_cb, 
			EINA_FALSE);
	ekeko_node_event_listener_add((Ekeko_Node *)e, "DOMAttrUpdated",  _attr_updated_cb,
			EINA_FALSE);
	ekeko_node_event_listener_add((Ekeko_Node *)e, "DOMNodeInserted",  _inserted_cb,
				EINA_FALSE);
	/* setup the private class */
	ekeko_node_user_set((Ekeko_Node *)e, RENDERABLE_PRIVATE, r);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ekeko_Element * ekeko_renderable_canvas_get(Ekeko_Element *e)
{
	
}
#if 0
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
#if 0
	c->renderables = eina_inlist_append(c->renderables, EINA_INLIST_GET(o));
#endif
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
		//goto callback;
	}
	o->curr.geometry.x = x;
	o->curr.geometry.y = y;
	/* valid */
#if 0
	if (eina_rectangles_intersect(&o->curr.geometry, &o->canvas->size))
		ekeko_renderable_validate(o);
	else
		ekeko_renderable_invalidate(o);
#endif
	_renderable_change(o);
#if 0
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
#endif
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
#if 0
	if (eina_rectangles_intersect(&o->curr.geometry, &o->canvas->size))
		ekeko_renderable_validate(o);
	else
		ekeko_renderable_invalidate(o);
#endif
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
	assert(cmp);typedef struct _Ekeko_Renderable_Cb Ekeko_Renderable_Cb;

	/* every renderable should receive the area or areas to draw based on their
	 * boundings
	 */

	/* do we need this state fixed with priv data on each or better make
	 * callbacks like _state_has_changed and store both states as void *
	 */
	typedef struct _Ekeko_Renderable_State
	{
		Eina_Rectangle geometry;
		Eina_Bool visible;
		void *data;
	} Ekeko_Renderable_State;

	struct _Ekeko_Renderable_Cb
	{
		EINA_INLIST;
	#if 0
		Ekeko_Event_Cb func;
	#endif
		void *data;


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
#endif
