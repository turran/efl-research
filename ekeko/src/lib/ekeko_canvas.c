#include "Ekeko.h"
#include "ekeko_private.h"
/*============================================================================*
 *                                  Local                                     * 
 *============================================================================*/
/* TODO move this to object.c? */
static void _object_changed(Ekeko_Canvas *c, Ekeko_Object *o)
{
	ekeko_object_pre_process(o);
}
/* refactor this to use the iterator */
static void _objects_changed(Ekeko_Canvas *c)
{
	Eina_Inlist *l;

	for (l = (Eina_Inlist *)c->objects; l; l = l->next)
	{
		Ekeko_Object *o;

		o = (Ekeko_Object *)l;
		if (o->changed == EINA_TRUE)
			_object_changed(c, o); 
	}
}
static void _damages_add(Ekeko_Canvas *c)
{
	Eina_Inlist *l;
	/* add the rectangles to the tiler */
	for (l = (Eina_Inlist*)c->damages; l; l = l->next)
	{
		Ekeko_Rectangle *r = (Ekeko_Rectangle *)l;
		ekeko_tiler_rect_add(c->tiler, &r->r);
		c->damages = eina_inlist_remove(c->damages, l);
	}
}
static void _obscures_remove(Ekeko_Canvas *c)
{
	/* remove the rectangles from the tiler */
}
/*============================================================================*
 *                                 Global                                     * 
 *============================================================================*/
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
	c->inputs = eina_inlist_append(c->inputs, i);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
Ekeko_Object * ekeko_canvas_object_get_at_coordinate(Ekeko_Canvas *c, unsigned int x, unsigned int y)
{
	Eina_Inlist *l;
	Eina_Rectangle r;
	
	r.x = x;
	r.y = y;
	r.w = 1;
	r.h = 1;
	/* TODO
	 * refactor this to call the iterator of objects
	 */
	/* iterate from the last to find the topmost object */
	for (l = ((Eina_Inlist *)c->objects)->last; l; l = l->prev)
	{
		Ekeko_Object *o;
		
		o = (Ekeko_Object *)l;
		/* TODO, the visibility check should be defined
		 * (receive or not events when hidden)
		 */
		/* check visibility */
		if (!ekeko_object_is_visible(o))
			continue;
		/* check geometry */
		if (ekeko_object_is_inside(o, &r))
			return o;
	}
	return NULL;
}
/* FIXME use this function
 */
static Eina_Bool _at_coordinate(Ekeko_Object *o, void *data)
{
	Eina_Rectangle *r = data;
	
	if (!ekeko_object_is_visible(o))
		return EINA_FALSE;
	/* check geometry */
	if (ekeko_object_is_inside(o, r))
		return EINA_TRUE;
	
	return EINA_FALSE;
}

/*============================================================================*
 *                                   API                                      * 
 *============================================================================*/
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI Ekeko_Canvas * ekeko_canvas_new(Ekeko_Canvas_Class *cclass, void *cdata,
		int type, int w, int h)
{
	Ekeko_Canvas *c;

	c = calloc(1, sizeof(Ekeko_Canvas));
	c->tiler = ekeko_tiler_new(type, w, h);
	c->tiler_type = type;
	c->cclass = cclass;
	c->cdata = cdata;

	return c;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_canvas_damage_add(Ekeko_Canvas *c, Eina_Rectangle *r)
{
	/* TODO we use an inlist, this should be optimized */
	Ekeko_Rectangle *er = calloc(1, sizeof(Ekeko_Rectangle));
	
	er->r = *r;
	c->damages = eina_inlist_append(c->damages, er);
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_canvas_obscure_add(Ekeko_Canvas *c, Eina_Rectangle *r)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_canvas_obscure_del(Ekeko_Canvas *c, Eina_Rectangle *r)
{
	
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_canvas_process(Ekeko_Canvas *c)
{
	Ekeko_Rectangle *redraws;
	Eina_Inlist *lr, *lo;
	int i = 0;

	assert(c);
	/* if the canvas hasnt changed, do nothing */
	if (!c->changed)
		return;
	
	/* 1. check changed objects */
	_objects_changed(c);
	/* 2. add damages */
	_damages_add(c);
	/* 3*. if the size has changed add the whole canvas and skip the damages */
	if (c->size_changed)
	{
		Eina_Rectangle r;
		
		ekeko_tiler_free(c->tiler);
		c->tiler = ekeko_tiler_new(c->tiler_type, c->w, c->h);
		eina_rectangle_coords_from(&r, 0, 0, c->w, c->h);
		ekeko_tiler_rect_add(c->tiler, &r);
		c->size_changed = EINA_FALSE;
	}
	/* 3. remove obscures */
	_obscures_remove(c);
	/* 4. get all rectangles to redraw */
	redraws = ekeko_tiler_rects_get(c->tiler);
	/* 5. process al objects that intersect with the rects */
	for (lr = (Eina_Inlist *)redraws; lr; lr = lr->next)
	{
		Ekeko_Rectangle *r = (Ekeko_Rectangle *)lr;
		
		//printf("[%d] %d %d %d %d\n", i, r->r.x, r->r.y, r->r.w, r->r.h);
		for (lo = (Eina_Inlist *)c->objects; lo; lo = lo->next)
		{
			Ekeko_Object *o;
			Eina_Rectangle orect;

			o = (Ekeko_Object *)lo;
			if (ekeko_object_intersection_get(o, r, &orect) &&
				ekeko_object_is_visible(o))
			{
				/* clip the rect to the bounding box of the object */
				ekeko_object_process(o, &orect);
				/* move this to other place */
				ekeko_object_post_process(o);
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
	/* 8. delete all objects that should be deleted */
	c->changed = EINA_FALSE;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_canvas_geometry_get(Ekeko_Canvas *c, Eina_Rectangle *r)
{
	assert(c);
	assert(r);
	
	r->x = 0;
	r->y = 0;
	r->w = c->w;
	r->h = c->h;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void ekeko_canvas_geometry_set(Ekeko_Canvas *c, unsigned int w, unsigned int h)
{
	assert(c);
	
	c->w = w;
	c->h = h;
	c->changed = EINA_TRUE;
	c->size_changed = EINA_TRUE;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
EAPI void * ekeko_canvas_class_data_get(Ekeko_Canvas *c)
{
	assert(c);
	return c->cdata;
}
/**
 * To be documented
 * FIXME: To be fixed
 */
/* FIXME This function can be integrated into eina */
EAPI Ekeko_Object * ekeko_canvas_object_from_last_get(Ekeko_Canvas *c, Ekeko_Object_Cmp_Func cmp, void *data)
{
	Ekeko_Object *o;
	
	o = ekeko_object_rel_get_up((Ekeko_Object *)(((Eina_Inlist *)c->objects)->last), cmp, data);
	return o;
}

/**
 * To be documented
 * FIXME: To be fixed
 */
/* FIXME This function can be integrated into eina */
EAPI Ekeko_Object * ekeko_canvas_object_from_first_get(Ekeko_Canvas *c, Ekeko_Object_Cmp_Func cmp, void *data)
{
	Ekeko_Object *o;
	
	o = ekeko_object_rel_get_up(c->objects, cmp, data);
	return o;
}

/* TODO
 * get objects that are in a rect
 */
