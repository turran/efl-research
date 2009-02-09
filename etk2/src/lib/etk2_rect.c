/*
 * etk2_rect.c
 *
 *  Created on: 04-feb-2009
 *      Author: jl
 */
#include "Etk2.h"
#include "etk2_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Etk_Rect_Private *)((Etk_Rect *)(d))->private)
struct _Etk_Rect_Private
{
	Etk_Coord x, y, w, h;
	int relative; /* relative counter to track down the listeners */
};

static inline void _coord_relative_handle(Etk_Coord *curr, Etk_Coord *prev,
		const Ekeko_Object *o, Etk_Canvas *canvas, Event_Listener cb)
{
	if (curr->type == prev->type)
		return;
	if (curr->type == ETK_COORD_ABSOLUTE)
	{
		/* TODO remove the callback */
	}
	else
	{
		ekeko_event_listener_add((Ekeko_Object *)canvas, EVENT_PROP_MODIFY, cb, EINA_FALSE, (void *)o);
	}
}

static void _x_canvas_x_change(const Ekeko_Object *o, Event *e, void *data)
{
	Event_Mutation *em = (Event_Mutation *)e;

	printf("X_CANVAS_X_CHANGE\n");
	if (em->state == EVENT_MUTATION_STATE_POST)
		return;
	if (!strcmp(em->prop, "x"))
	{
		Etk_Coord *x;
		Etk_Coord w;
		Etk_Rect *rect = data;
		Etk_Rect_Private *prv = PRIVATE(rect);
		Eina_Rectangle r;

		x = em->curr->value.pointer_value;
		ekeko_renderable_geometry_get((Ekeko_Renderable *)rect, &r);
		etk_canvas_w_get((Etk_Canvas *)o, &w);
		r.x = prv->x.final = x->final + ((w.final * prv->x.value) / 100);
		ekeko_renderable_geometry_set((Ekeko_Renderable *)rect, &r);
		printf("NEW X = %d\n", r.x);
	}
}

static void _x_canvas_w_change(const Ekeko_Object *o, Event *e, void *data)
{
	Event_Mutation *em = (Event_Mutation *)e;

	printf("X_CANVAS_W_CHANGE\n");
	if (em->state == EVENT_MUTATION_STATE_POST)
		return;
	if (!strcmp(em->prop, "w"))
	{
		Etk_Coord x;
		Etk_Coord *w;
		Etk_Rect *rect = data;
		Etk_Rect_Private *prv = PRIVATE(rect);
		Eina_Rectangle r;

		w = em->curr->value.pointer_value;
		ekeko_renderable_geometry_get((Ekeko_Renderable *)rect, &r);
		etk_canvas_x_get((Etk_Canvas *)o, &x);
		r.x = prv->x.final = x.final + ((w->final * prv->x.value) / 100);
		ekeko_renderable_geometry_set((Ekeko_Renderable *)rect, &r);
		printf("NEW X = %d\n", r.x);
	}
}

static void _canvas_geometry_change(const Ekeko_Object *o, Event *e, void *data)
{
	Event_Mutation *em = (Event_Mutation *)e;

	if (em->state == EVENT_MUTATION_STATE_POST)
		return;
	if (!strcmp(em->prop, "geometry"))
	{
		Etk_Rect *rect = data;
		Etk_Rect_Private *prv = PRIVATE(rect);
		Eina_Rectangle r;

		ekeko_renderable_geometry_get((Ekeko_Renderable *)rect, &r);
		printf("CANVAS GEOMETRY CHANGE %d %d %d %d!!!!!!!!!!\n", em->curr->value.rect.x, em->curr->value.rect.y, em->curr->value.rect.w, em->curr->value.rect.h);
		if (prv->x.type == ETK_COORD_RELATIVE)
		{
			r.x = prv->x.final = em->curr->value.rect.x + ((em->curr->value.rect.w * prv->x.value) / 100);
		}
		if (prv->y.type == ETK_COORD_RELATIVE)
		{
			r.y = prv->y.final = em->curr->value.rect.y + ((em->curr->value.rect.h * prv->y.value) / 100);
		}
		if (prv->w.type == ETK_COORD_RELATIVE)
		{
			r.w = prv->w.final = (em->curr->value.rect.w * prv->w.value) / 100;
		}
		if (prv->h.type == ETK_COORD_RELATIVE)
		{
			r.w = prv->h.final = (em->curr->value.rect.h * prv->h.value) / 100;
		}
		ekeko_renderable_geometry_set((Ekeko_Renderable *)rect, &r);
	}
}

static void _properties_updated(const Ekeko_Object *o, Event *e, void *data)
{
	Event_Mutation *em = (Event_Mutation *)e;
	Etk_Rect_Private *prv;
	Etk_Canvas *canvas;
	/* check this is POST process */
	/* get the parent canvas */

	/* set the renderable geometry ? what happens on the process function? */

	/* in case of relative coordinate calc the new coordinate
	 * and store it on Etk_Coord.final
	 */
	if (em->state == EVENT_MUTATION_STATE_POST)
		return;
	if (!(canvas = etk_shape_canvas_get((Etk_Shape *)o)))
		return;
	prv = PRIVATE(o);
	/* geometry coords changed */
	if (!strcmp(em->prop, "x"))
	{
		Eina_Rectangle r;
		Etk_Coord *c, *p;

		ekeko_renderable_geometry_get((Ekeko_Renderable *)o, &r);
		c = em->curr->value.pointer_value;
		p = em->prev->value.pointer_value;
		if (c->type == ETK_COORD_RELATIVE)
		{
			Eina_Rectangle cr;

			ekeko_renderable_geometry_get(ekeko_object_parent_get(o), &cr);
			r.x = prv->x.final = cr.x + ((cr.w * prv->x.value) / 100);
			ekeko_renderable_geometry_set((Ekeko_Renderable *)o, &r);
			if (p->type == ETK_COORD_ABSOLUTE)
			{
				prv->relative++;
				goto coord;
			}
		}
		else
		{
			r.x = prv->x.final = prv->x.value;
			ekeko_renderable_geometry_set((Ekeko_Renderable *)o, &r);
			if (p->type == ETK_COORD_RELATIVE)
			{
				prv->relative--;
				goto coord;
			}
		}
	}
	else if (!strcmp(em->prop, "y"))
	{
		Eina_Rectangle r;
		Etk_Coord *c, *p;

		ekeko_renderable_geometry_get((Ekeko_Renderable *) o, &r);
		c = em->curr->value.pointer_value;
		p = em->prev->value.pointer_value;
		if (c->type == ETK_COORD_RELATIVE)
		{
			Eina_Rectangle cr;

			ekeko_renderable_geometry_get(ekeko_object_parent_get(o), &cr);
			r.y = prv->y.final = cr.y + ((cr.h * prv->y.value) / 100);
			ekeko_renderable_geometry_set((Ekeko_Renderable *) o, &r);
			if (p->type == ETK_COORD_ABSOLUTE)
			{
				prv->relative++;
				goto coord;
			}
		}
		else
		{
			r.y = prv->y.final = prv->y.value;
			ekeko_renderable_geometry_set((Ekeko_Renderable *) o, &r);
			if (p->type == ETK_COORD_RELATIVE)
			{
				prv->relative--;
				goto coord;
			}
		}
	}
	else if (!strcmp(em->prop, "w"))
	{
		Eina_Rectangle r;
		Etk_Coord *c, *p;

		ekeko_renderable_geometry_get((Ekeko_Renderable *)o, &r);
		c = em->curr->value.pointer_value;
		p = em->prev->value.pointer_value;
		if (c->type == ETK_COORD_RELATIVE)
		{
			Eina_Rectangle cr;

			ekeko_renderable_geometry_get(ekeko_object_parent_get(o), &cr);
			r.w = prv->w.final = (cr.w * prv->w.value) / 100;
			ekeko_renderable_geometry_set((Ekeko_Renderable *)o, &r);
			if (p->type == ETK_COORD_ABSOLUTE)
			{
				prv->relative++;
				goto coord;
			}
		}
		else
		{
			r.w = prv->w.final = prv->w.value;
			ekeko_renderable_geometry_set((Ekeko_Renderable *)o, &r);
			if (p->type == ETK_COORD_RELATIVE)
			{
				prv->relative--;
				goto coord;
			}
		}
	}
	else if (!strcmp(em->prop, "h"))
	{
		Eina_Rectangle r;
		Etk_Coord *c, *p;

		ekeko_renderable_geometry_get((Ekeko_Renderable *)o, &r);
		c = em->curr->value.pointer_value;
		p = em->prev->value.pointer_value;
		if (c->type == ETK_COORD_RELATIVE)
		{
			Eina_Rectangle cr;

			ekeko_renderable_geometry_get(ekeko_object_parent_get(o), &cr);
			r.h = prv->h.final = (cr.h * prv->h.value) / 100;
			ekeko_renderable_geometry_set((Ekeko_Renderable *)o, &r);
			if (p->type == ETK_COORD_ABSOLUTE)
			{
				prv->relative++;
				goto coord;
			}
		}
		else
		{
			r.h = prv->h.final = prv->h.value;
			ekeko_renderable_geometry_set((Ekeko_Renderable *)o, &r);
			if (p->type == ETK_COORD_RELATIVE)
			{
				prv->relative--;
				goto coord;
			}
		}
	}
	return;
coord:
	if (prv->relative)
	{
		ekeko_event_listener_add(ekeko_object_parent_get(o), EVENT_PROP_MODIFY, _canvas_geometry_change, EINA_FALSE, (void *)o);
	}
	else
	{
		/* TODO remove the callback*/
	}
}

static void _render(Ekeko_Renderable *r, Eina_Rectangle *rect)
{
	Etk_Document *d;
	Etk_Canvas *c;
	Etk_Engine *func;
	Etk_Rect_Private *prv;
	void *context;
	void *surface;

	/* TODO all of this can be abstracted on a function
	 * at shape
	 */
	c = (Etk_Canvas *)ekeko_renderable_canvas_get(r);
	d = etk_canvas_document_get(c);
	func = etk_document_engine_get(d);
	surface = etk_canvas_surface_get(c);
	context = etk_shape_context_get((Etk_Shape *)r);
	prv = PRIVATE(r);
	/* TODO context clip set */
	func->shape->rect(surface, context, prv->x.final, prv->y.final,
			prv->w.final, prv->h.final);
}


static void _ctor(void *instance)
{
	Etk_Rect *r;
	Etk_Rect_Private *prv;

	r = (Etk_Rect*) instance;
	r->private = prv = ekeko_type_instance_private_get(etk_rect_type_get(), instance);
	r->parent.parent.render = _render;
	ekeko_event_listener_add((Ekeko_Object *)r, EVENT_PROP_MODIFY, _properties_updated, EINA_FALSE, NULL);
}

static void _dtor(void *rect)
{

}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Ekeko_Type *etk_rect_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(ETK_TYPE_RECT, sizeof(Etk_Rect),
				sizeof(Etk_Rect_Private), etk_shape_type_get(),
				_ctor, _dtor, NULL);
		TYPE_PROP_SINGLE_ADD(type, "x", ETK_PROPERTY_COORD, OFFSET(Etk_Rect_Private, x));
		TYPE_PROP_SINGLE_ADD(type, "y", ETK_PROPERTY_COORD, OFFSET(Etk_Rect_Private, y));
		TYPE_PROP_SINGLE_ADD(type, "w", ETK_PROPERTY_COORD, OFFSET(Etk_Rect_Private, w));
		TYPE_PROP_SINGLE_ADD(type, "h", ETK_PROPERTY_COORD, OFFSET(Etk_Rect_Private, h));
		//TYPE_PROP_SINGLE_ADD(type, "color", PROPERTY_INT, OFFSET(Etk_Shape_Private, color));
	}

	return type;
}

EAPI Etk_Rect * etk_rect_new(Etk_Canvas *c)
{
	Etk_Rect *r;

	r = ekeko_type_instance_new(etk_rect_type_get());
	ekeko_object_child_append((Ekeko_Object *)c, (Ekeko_Object *)r);

	return r;
}

EAPI void etk_rect_x_rel_set(Etk_Rect *r, int x)
{
	Etk_Coord coord;
	Value v;

	etk_value_coord_from(&v, &coord, x, ETK_COORD_RELATIVE);
	ekeko_object_property_value_set((Ekeko_Object *)r, "x", &v);
}

EAPI void etk_rect_x_set(Etk_Rect *r, int x)
{
	Etk_Coord coord;
	Value v;

	etk_value_coord_from(&v, &coord, x, ETK_COORD_ABSOLUTE);
	ekeko_object_property_value_set((Ekeko_Object *)r, "x", &v);
}

EAPI void etk_rect_y_set(Etk_Rect *r, int y)
{
	Etk_Coord coord;
	Value v;

	etk_value_coord_from(&v, &coord, y, ETK_COORD_ABSOLUTE);
	ekeko_object_property_value_set((Ekeko_Object *)r, "y", &v);
}

EAPI void etk_rect_y_rel_set(Etk_Rect *r, int y)
{
	Etk_Coord coord;
	Value v;

	etk_value_coord_from(&v, &coord, y, ETK_COORD_RELATIVE);
	ekeko_object_property_value_set((Ekeko_Object *)r, "y", &v);
}

EAPI void etk_rect_w_set(Etk_Rect *r, int w)
{
	Etk_Coord coord;
	Value v;

	etk_value_coord_from(&v, &coord, w, ETK_COORD_ABSOLUTE);
	ekeko_object_property_value_set((Ekeko_Object *)r, "w", &v);
}

EAPI void etk_rect_w_rel_set(Etk_Rect *r, int w)
{
	Etk_Coord coord;
	Value v;

	etk_value_coord_from(&v, &coord, w, ETK_COORD_RELATIVE);
	ekeko_object_property_value_set((Ekeko_Object *)r, "w", &v);
}

EAPI void etk_rect_h_set(Etk_Rect *r, int h)
{
	Etk_Coord coord;
	Value v;

	etk_value_coord_from(&v, &coord, h, ETK_COORD_ABSOLUTE);
	ekeko_object_property_value_set((Ekeko_Object *)r, "h", &v);
}

EAPI void etk_rect_h_rel_set(Etk_Rect *r, int h)
{
	Etk_Coord coord;
	Value v;

	etk_value_coord_from(&v, &coord, h, ETK_COORD_RELATIVE);
	ekeko_object_property_value_set((Ekeko_Object *)r, "h", &v);
}
