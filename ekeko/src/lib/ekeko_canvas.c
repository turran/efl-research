#include "Ekeko.h"
#include "ekeko_private.h"
/* 
 * TODO
 * + Remove every reference to renderables because we should use the list of
 * valid renderables only
 * + Make the canvas be an object approach, similar to what has to be done
 * with the renderables / locatable
 * + Make the canvas class actually abstract functions
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
typedef struct _Ekeko_Canvas
{
	/* possible callbacks */
	// rectangle_push: in case of double buffer, this will inform what has changed (the last rendered rectangle)
	// what about post processing of the canvas??
	Ekeko_Tiler *tiler;
	int tiler_type; /* FIXME fix this to an enum */
	Eina_Inlist *renderables;
	Eina_Inlist *inputs;
	Eina_Inlist *damages;
	// obscures
	struct
	{
		Ekeko_Canvas_Flush flush;
	} cb;
	Eina_List *valid; /* objects that need to be draw */
	Eina_List *invalid; /* objects that dont need to be draw */
} Ekeko_Canvas;


/* TODO move this to object.c? */
static void _object_changed(Ekeko_Canvas *c, Ekeko_Renderable *o)
{
	ekeko_renderable_pre_process(o);
}
#if 0
/* refactor this to use the iterator */
static void _renderables_changed(Ekeko_Canvas *c)
{
	Ekeko_Renderable *o;

	EINA_INLIST_FOREACH(c->renderables, o)
	{
		if (o->changed == EINA_TRUE)
			_object_changed(c, o);
	}
}
#endif
static void _damage_add(Ekeko_Canvas *c, Eina_Rectangle *r)
{
	/* TODO we use an inlist, this should be optimized */
	Ekeko_Rectangle *er = calloc(1, sizeof(Ekeko_Rectangle));
	
	er->r = *r;
	c->damages = eina_inlist_append(c->damages, EINA_INLIST_GET(er));	
}
static void _damages_add(Ekeko_Canvas *c)
{
	Ekeko_Rectangle *r;

	/* add the rectangles to the tiler */
	EINA_INLIST_FOREACH(c->damages, r)
	{
		ekeko_tiler_rect_add(c->tiler, &r->r);
		c->damages = eina_inlist_remove(c->damages, EINA_INLIST_GET(r));
	}
}
static void _obscures_remove(Ekeko_Canvas *c)
{
	/* remove the rectangles from the tiler */
}

static void _attr_updated_cb(Ekeko_Event *ee)
{
	Ekeko_Event_Mutation *e = (Ekeko_Event_Mutation *)ee;
	int w, h;
	Eina_Bool r = EINA_FALSE;
	
	/* if the size has changed we should damage the whole canvas */
	if (!strcmp(e->attr, "_width"))
	{
		Ekeko_Value height;

		r = EINA_TRUE;
		ekeko_element_attribute_get((Ekeko_Element *)e->related, "_height", &height);
		h = height.v.i;
		w = e->curr->v.i;
	}
	else if (!strcmp(e->attr, "_height"))
	{
		Ekeko_Value width;

		r = EINA_TRUE;
		ekeko_element_attribute_get((Ekeko_Element *)e->related, "_width", &width);
		w = width.v.i;
		h = e->curr->v.i;
	}
	if (r)
	{
		Eina_Rectangle r;
		Ekeko_Canvas *c;

		c = ekeko_node_user_get(e->related, "canvas");
		eina_rectangle_coords_from(&r, 0, 0, w, h);
		if (c->tiler)
			ekeko_tiler_free(c->tiler);
		c->tiler = ekeko_tiler_new(EKEKO_TILER_SPLITTER, w, h);
		/* TODO add a way to notify that the canvas' surface should resize */
		_damage_add(c, &r);
	}
}

static void _process_cb(Ekeko_Event *ee)
{
	Ekeko_Canvas *c;
	Ekeko_Rectangle *redraw;
	Eina_Iterator *rit;
	Ekeko_Event_Process *e = (Ekeko_Event_Process *)ee;

	c = ekeko_node_user_get(e->related, "canvas");
	/* add every damage to the tiler */
	/* FIXME this should be done at damage add, no need to do that here */
	_damages_add(c);
	/* delete all obscures */
	_obscures_remove(c);
	/* get all rectangles to redraw */
	redraw = ekeko_tiler_rects_get(c->tiler);
	/* iterate over all rectangles to redraw */
	rit = eina_inlist_iterator_new(EINA_INLIST_GET(redraw));
	while (eina_iterator_next(rit, (void **)&redraw))
	{
		Ekeko_Renderable *r;

		/* iterate over all renderables from top to bottom and render them */
		printf("Rectangle %d %d %d %d\n", redraw->r.x, redraw->r.y, redraw->r.w, redraw->r.h);
		/* ekeko_renderable_render(element); */
	}
	eina_iterator_free(rit);
	/* flush the rectangles on the canvas */
	rit = eina_inlist_iterator_new(EINA_INLIST_GET(redraw));
	while (eina_iterator_next(rit, (void **)&redraw))
	{
		if (!c->cb.flush((Ekeko_Element *)e->related, &redraw->r))
			break;
	}
	eina_iterator_free(rit);
	/* clear the rectangles */
	ekeko_tiler_clear(c->tiler);
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
#if 0
/**
 * To be documented
 * FIXME: To be fixed
 */
void ekeko_canvas_change(Ekeko_Canvas *c)
{
	c->changed = EINA_TRUE;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
void ekeko_canvas_input_add(Ekeko_Canvas *c, Ekeko_Input *i)
{
	c->inputs = eina_inlist_append(c->inputs, EINA_INLIST_GET(i));
}
/**
 * To be documented
 * FIXME: To be fixed
 */
Ekeko_Renderable * ekeko_canvas_object_get_at_coordinate(Ekeko_Canvas *c, unsigned int x, unsigned int y)
{
	Eina_Inlist *l;
	Eina_Rectangle r;

	r.x = x;
	r.y = y;
	r.w = 1;
	r.h = 1;
	/* TODO
	 * refactor this to call the iterator of renderables
	 */
	/* iterate from the last to find the topmost object */
	for (l = ((Eina_Inlist *)c->renderables)->last; l; l = l->prev)
	{
		Ekeko_Renderable *o;
		
		o = (Ekeko_Renderable *)l;
		/* TODO, the visibility check should be defined
		 * (receive or not events when hidden)
		 */
		/* check visibility */
		if (!ekeko_renderable_is_visible(o))
			continue;
		/* check geometry */
		if (ekeko_renderable_is_inside(o, &r))
			return o;
	}
	return NULL;
}
/* FIXME use this function
 */
static Eina_Bool _at_coordinate(Ekeko_Renderable *o, void *data)
{
	Eina_Rectangle *r = data;
	
	if (!ekeko_renderable_is_visible(o))
		return EINA_FALSE;
	/* check geometry */
	if (ekeko_renderable_is_inside(o, r))
		return EINA_TRUE;
	
	return EINA_FALSE;
}
#endif
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_canvas_new(Ekeko_Element *e, Ekeko_Canvas_Flush flush)
{
	Ekeko_Canvas *c;
	Ekeko_Value def;

	c = ekeko_node_user_get((Ekeko_Node *)e, "canvas");
	if (c != NULL)
		return;
	/* private data */
	c = calloc(1, sizeof(Ekeko_Canvas));
	c->tiler = ekeko_tiler_new(EKEKO_TILER_SPLITTER, 0, 0);
	//c->tiler_type = type;
	c->cb.flush = flush;
	ekeko_node_user_set((Ekeko_Node *)e, "canvas", c);
	/* setup the attributes for a renderable element */
	ekeko_value_int_from(&def, 0);
	ekeko_element_attribute_set(e, "_width", &def);
	ekeko_element_attribute_set(e, "_height", &def);
	/* setup the events */
	ekeko_node_event_listener_add(e, "ProcessEvents",  _process_cb, 
			EINA_FALSE);
	ekeko_node_event_listener_add(e, "DOMAttrUpdated",  _attr_updated_cb,
			EINA_FALSE);
}
/**
 * @brief Marks a rectangle on the canvas as damaged, this area will be
 * processed again. When the canvas process that area it will no longer be
 * a damaged area
 * @param r Rectangle that defines the area damaged 
 */
EAPI void ekeko_canvas_damage_add(Ekeko_Element *e, Eina_Rectangle *r)
{
	Ekeko_Canvas *c;
	
	c = ekeko_node_user_get((Ekeko_Node *)e, "canvas");
	if (!c)
		return;
	_damage_add(c, r);
}
/**
 * @brief Marks a rectangle area on the canvas that will never be processed.
 * The area is kept on the canvas until it is cleared with
 * ekeko_canvas_obscure_del()
 * @param r Rectangle that defines the obscure area 
 */
EAPI void ekeko_canvas_obscure_add(Ekeko_Element *c, Eina_Rectangle *r)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_canvas_obscure_del(Ekeko_Element *c, Eina_Rectangle *r)
{
	
}
/**
 * @brief Gets the width and height of the canvas
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_canvas_size_get(Ekeko_Element *e, unsigned int *w, unsigned int *h)
{
	Ekeko_Canvas *c;

	c = ekeko_node_user_get((Ekeko_Node *)e, "canvas");
	if (!c) return;
	
	if (w)
	{
		Ekeko_Value v;

		ekeko_node_attribute_get((Ekeko_Node *)e, "_width", &v);
		*h = v.v.i;
	}
	if (h)
	{
		Ekeko_Value v;

		ekeko_node_attribute_get((Ekeko_Node *)e, "_height", &v);
		*h = v.v.i;
	}
}
/**
 * @brief Gets the width and height of the canvas
 * @param w Width of the canvas
 * @param h Height of the canvas
 */
EAPI void ekeko_canvas_size_set(Ekeko_Element *e, unsigned int w, unsigned int h)
{
	Ekeko_Canvas *c;
	Ekeko_Value v;
		
	c = ekeko_node_user_get((Ekeko_Node *)e, "canvas");
	if (!c) return;

	ekeko_value_int_from(&v, w);
	ekeko_node_attribute_set((Ekeko_Node *)e, "_width", &v);
	ekeko_value_int_from(&v, h);
	ekeko_node_attribute_set((Ekeko_Node *)e, "_height", &v);
}
/**
 * @brief Process the canvas. Every object that needs to be processed again
 * will get it's class called. Every damaged area is cleared after this call
 * @param c Canvas to process
 */
#if 0
EAPI void ekeko_canvas_process(Ekeko_Element *c)
{
	Ekeko_Rectangle *redraws;
	Eina_Inlist *lr, *lo;
	int i = 0;

	assert(c);
	/* if the canvas hasnt changed, do nothing */
	if (!c->changed)
		return;
	
	/* 1. check changed renderables */
	_renderables_changed(c);
	/* 2. add damages */
	_damages_add(c);
	/* 3. if the size has changed add the whole canvas and skip the damages */
	if (c->size_changed)
	{
		Eina_Rectangle r;
		
		ekeko_tiler_free(c->tiler);
		c->tiler = ekeko_tiler_new(c->tiler_type, c->size.w, c->size.h);
		eina_rectangle_coords_from(&r, 0, 0, c->size.w, c->size.h);
		ekeko_tiler_rect_add(c->tiler, &r);
		c->size_changed = EINA_FALSE;
		/* TODO regenerate the list of valid and invalid renderables */
	}
	/* 4. remove obscures */
	_obscures_remove(c);
	/* 5. get all rectangles to redraw */
	redraws = ekeko_tiler_rects_get(c->tiler);
	/* 6. process al renderables that intersect with the rects */
	for (lr = (Eina_Inlist *)redraws; lr; lr = lr->next)
	{
		Ekeko_Rectangle *r = (Ekeko_Rectangle *)lr;
		
		//printf("[%d] %d %d %d %d\n", i, r->r.x, r->r.y, r->r.w, r->r.h);
		for (lo = (Eina_Inlist *)c->renderables; lo; lo = lo->next)
		{
			Ekeko_Renderable *o;
			Eina_Rectangle orect;

			o = (Ekeko_Renderable *)lo;
			if (ekeko_renderable_intersection_get(o, r, &orect) &&
				ekeko_renderable_is_visible(o))
			{
				/* clip the rect to the bounding box of the object */
				ekeko_renderable_process(o, &orect);
				/* move this to other place */
				ekeko_renderable_post_process(o);
			}
		}
		i++;
	}
	/* 6. flush the rectangles on the canvas */
	for (lr = (Eina_Inlist *)redraws; lr; lr = lr->next)
	{
		Ekeko_Rectangle *r = (Ekeko_Rectangle *)lr;
	
		if (!c->cclass->flush(c->cdata, &r->r))
			break;
	}
	/* 7. clear the rectangles */
	ekeko_tiler_clear(c->tiler);
	/* 8. delete all renderables that should be deleted */
	c->changed = EINA_FALSE;
}
#endif

#if 0
/**
 * To be documented
 * FIXME: To be fixed
 */
/* FIXME This function can be integrated into eina */
EAPI Ekeko_Renderable * ekeko_canvas_object_from_last_get(Ekeko_Canvas *c, Ekeko_Renderable_Cmp_Func cmp, void *data)
{
	Ekeko_Renderable *o;
	
	o = ekeko_renderable_rel_get_up((Ekeko_Renderable *)(((Eina_Inlist *)c->renderables)->last), cmp, data);
	return o;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
/* FIXME This function can be integrated into eina */
EAPI Ekeko_Renderable * ekeko_canvas_object_from_first_get(Ekeko_Canvas *c, Ekeko_Renderable_Cmp_Func cmp, void *data)
{
	Ekeko_Renderable *o;
	
	o = ekeko_renderable_rel_get_up(c->renderables, cmp, data);
	return o;
}
#endif
/* TODO
 * get renderables that are in a rect
 */
