/*
 * etk2_canvas.c
 *
 *  Created on: 03-feb-2009
 *      Author: jl
 */
#include "Etk2.h"
#include "etk2_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Etk_Canvas_Private *)((Etk_Canvas *)(d))->private)
struct _Etk_Canvas_Private
{
	Etk_Coord x, y, w, h;
	/* track parent canvas, if parent canvas == NULL then this is the
	 * main canvas and we should treat it differently
	 */
	void *s;
	Eina_Bool root;
	/* TODO overflow property */
	/* FIXME change this later */
	Etk_Document *doc;
};

/* in case the subcanvas has another canva as parent it will blt to the
 * parent canvas
 */
static void _subcanvas_render(Ekeko_Renderable *r, Eina_Rectangle *rect)
{
	Eina_Rectangle sgeom, srect;
	Etk_Canvas *c;
	Etk_Canvas_Private *sprv, *cprv;
	Etk_Engine *func;

	sprv = PRIVATE(r);
	c = (Etk_Canvas *)ekeko_renderable_canvas_get(r);
	cprv = PRIVATE(c);

	ekeko_renderable_geometry_get(r, &sgeom);
	eina_rectangle_rescale_in(&sgeom, rect, &srect);
	/* blt there */
	func = etk_document_engine_get(sprv->doc);
	func->canvas->blit(sprv->s, &srect, NULL, cprv->s, rect);
}

static Eina_Bool _flush(Ekeko_Canvas *c, Eina_Rectangle *r)
{
	Etk_Canvas_Private *prv;

	prv = PRIVATE(c);
	if (!prv->s)
	{
#ifdef ETK2_DEBUG
		printf("[Etk_Canvas] the canvas doesnt have a surface\n");
#endif
		return EINA_TRUE;
	}
	/* if root flip */
	if (prv->root)
	{
		Etk_Engine *func;

#ifdef ETK2_DEBUG
		printf("[Etk_Canvas] flipping root surface\n");
#endif
		/* TODO flip */
		func = etk_document_engine_get(prv->doc);
		return func->canvas->flush(prv->s, r);
	}
	/* otherwise blt */
	else
	{
		Eina_Rectangle rscaled;
		Eina_Rectangle cgeom;
		Ekeko_Canvas *dc; /* the canvas this subcanvas has */

		/* this canvas doesnt have a parent canvas? */
		dc = ekeko_renderable_canvas_get((Ekeko_Renderable *)c);
		if (!dc)
			return EINA_TRUE;
		ekeko_renderable_geometry_get((Ekeko_Renderable *)c, &cgeom);
		/* transform the rectangle relative to the upper canvas */
		eina_rectangle_rescale_out(&cgeom, r, &rscaled);
		printf("[EtkCanvas] subcanvas adding a new damage %d %d %d %d (%d %d %d %d)\n",
				rscaled.x, rscaled.y, rscaled.w, rscaled.h,
				r->x, r->y, r->w, r->h);
		printf("[EtkCanvas] subcanvas = %p, canvas = %p\n", c, dc);
		ekeko_canvas_damage_add(dc, &rscaled);
	}
	return EINA_FALSE;
}

static void _x_changed_canvas(Etk_Coord *curr, Etk_Coord *prev, Etk_Canvas *c)
{
	printf("PARENT IS A CANVAS!!!\n");
}

static void _y_changed_canvas(Etk_Coord *curr, Etk_Coord *prev, Etk_Canvas *c)
{
	printf("PARENT IS A CANVAS!!!\n");
}

static void _h_changed_canvas(Etk_Coord *curr, Etk_Coord *prev, Etk_Canvas *c)
{
	printf("PARENT IS A CANVAS!!!\n");
}

static void _w_changed_canvas(Etk_Coord *curr, Etk_Coord *prev, Etk_Canvas *c)
{
	printf("PARENT IS A CANVAS!!!\n");
}

static void _document_w_change(const Ekeko_Object *o, Event *e, void *data)
{
	Event_Mutation *em = (Event_Mutation *)e;

#ifdef ETK2_DEBUG
	printf("Document W changed\n");
#endif
	/* w change then change x or w as needed */
	if (!strcmp(em->prop, "size"))
	{
		Etk_Canvas *c = data;
		Etk_Canvas_Private *prv = PRIVATE(c);
		Eina_Rectangle r;

		ekeko_renderable_geometry_get((Ekeko_Renderable *)c, &r);
		if (prv->x.type == ETK_COORD_RELATIVE)
		{
			r.x = prv->x.final = (em->curr->value.rect.w * prv->x.value) / 100;

		}
		if (prv->w.type == ETK_COORD_RELATIVE)
		{
			r.w = prv->w.final = (em->curr->value.rect.w * prv->w.value) / 100;
		}
		ekeko_renderable_geometry_set((Ekeko_Renderable *)c, &r);
	}
}

static void _document_h_change(const Ekeko_Object *o, Event *e, void *data)
{
	Event_Mutation *em = (Event_Mutation *)e;

	printf("Document H changed\n");
	/* h change then change y or h as needed */
	if (!strcmp(em->prop, "size"))
	{
		Etk_Canvas *c = data;
		Etk_Canvas_Private *prv = PRIVATE(c);
		Eina_Rectangle r;

		ekeko_renderable_geometry_get((Ekeko_Renderable *)c, &r);
		if (prv->y.type == ETK_COORD_RELATIVE)
		{
			r.y = prv->y.final = (em->curr->value.rect.h * prv->y.value) / 100;
		}
		if (prv->h.type == ETK_COORD_RELATIVE)
		{
			r.h = prv->h.final = (em->curr->value.rect.h * prv->h.value) / 100;
		}
		ekeko_renderable_geometry_set((Ekeko_Renderable *)c, &r);
	}
}
/* FIXME The below functions dont need to register a callback themselves
 * only one callback for geometry change, like the what Etk_Rect does
 */
static void _xw_changed_document(Etk_Coord *curr, Etk_Coord *prev, Etk_Canvas *c)
{
	if (curr->type == ETK_COORD_RELATIVE)
	{
		int w;
		Ekeko_Object *doc;

		doc = ekeko_object_parent_get((Ekeko_Object *)c);
		etk_document_size_get((Etk_Document *)doc, &w, NULL);
		curr->final = (w * curr->value) / 100;
		if (prev->type == ETK_COORD_ABSOLUTE)
		{
			/* add the callback */
			ekeko_event_listener_add(doc, EVENT_PROP_MODIFY, _document_w_change, EINA_FALSE, (void *)c);
		}
	}
	else
	{
		curr->final = curr->value;
		if (prev->type == ETK_COORD_RELATIVE)
		{
			Ekeko_Object *doc;

			doc = ekeko_object_parent_get((Ekeko_Object *)c);
			/* TODO remove the callback */
		}
	}
}

static void _yh_changed_document(Etk_Coord *curr, Etk_Coord *prev, Etk_Canvas *c)
{
	if (curr->type == ETK_COORD_RELATIVE)
	{
		int h;
		Ekeko_Object *doc;

		doc = ekeko_object_parent_get((Ekeko_Object *)c);
		etk_document_size_get((Etk_Document *)doc, NULL, &h);
		curr->final = (h * curr->value) / 100;;
		if (prev->type == ETK_COORD_ABSOLUTE)
		{
			/* add the callback */
			ekeko_event_listener_add(doc, EVENT_PROP_MODIFY, _document_h_change, EINA_FALSE, (void *)c);
		}
	}
	else
	{
		curr->final = curr->value;
		if (prev->type == ETK_COORD_RELATIVE)
		{
			Ekeko_Object *doc;

			doc = ekeko_object_parent_get((Ekeko_Object *)c);
			/* TODO remove the callback */
		}
	}
}

static void _document_geometry_cb(const Ekeko_Object *o, Event *e, void *data)
{
	Etk_Engine *func;
	Etk_Canvas *c;
	Etk_Canvas_Private *prv;
	Event_Mutation *em = (Event_Mutation *)e;

	if (!strcmp(em->prop, "size"))
	{
		func = etk_document_engine_get((Etk_Document *)o);
		c = data;
		prv = PRIVATE(c);

		prv->s = func->canvas->create(prv->root, em->curr->value.rect.w, em->curr->value.rect.h);
	}
}

static void _prop_modify_cb(const Ekeko_Object *o, Event *e, void *data)
{
	Event_Mutation *em = (Event_Mutation *)e;
	Etk_Canvas_Private *prv = PRIVATE(o);

	/* check if the change is the rectangle */
	if (em->state == EVENT_MUTATION_STATE_POST)
	{
		if (!strcmp(em->prop, "geometry"))
		{
			Etk_Engine *func;
			int w, h;

			if (!prv->doc)
			{
				printf("EEEEEEEEEEEEERRRRRRRRRRRRRRROOOOOOOOOOR\n");
				exit(1);
			}
			if (prv->root)
			{
				return;
			}
			/* the canvas surface size should be:
			 * document.w, document.h for root
			 * and geom.w, geom.h for non root
			 */
			w = prv->w.final;
			h = prv->h.final;
			func = etk_document_engine_get(prv->doc);
			prv->s = func->canvas->create(prv->root, w, h);
		}
	}
	/* geometry coords changed */
	else
	{
		/* if x, y, w or h have changed and are relative to the parent
		 * add listeners for it
		 */
		if (!strcmp(em->prop, "x"))
		{
			Eina_Rectangle r;
			Etk_Coord *c, *p;

			ekeko_renderable_geometry_get((Ekeko_Renderable *)o, &r);
			c = em->curr->value.pointer_value;
			p = em->prev->value.pointer_value;
			/* Set the final value */
			if (prv->root)
				_xw_changed_document(c, p, (Etk_Canvas *)o);
			else
				_x_changed_canvas(c, p, (Etk_Canvas *)o);
			r.x = c->final;
			ekeko_renderable_geometry_set((Ekeko_Renderable *)o, &r);
		}
		else if (!strcmp(em->prop, "y"))
		{
			Eina_Rectangle r;
			Etk_Coord *c, *p;

			ekeko_renderable_geometry_get((Ekeko_Renderable *)o, &r);
			c = em->curr->value.pointer_value;
			p = em->prev->value.pointer_value;
			/* Set the final value */
			if (prv->root)
				_yh_changed_document(c, p, (Etk_Canvas *)o);
			else
				_y_changed_canvas(c, p, (Etk_Canvas *)o);
			r.y = c->value;
			ekeko_renderable_geometry_set((Ekeko_Renderable *)o, &r);
		}
		else if (!strcmp(em->prop, "w"))
		{
			Eina_Rectangle r;
			Etk_Coord *c, *p;

			ekeko_renderable_geometry_get((Ekeko_Renderable *)o, &r);
			c = em->curr->value.pointer_value;
			p = em->prev->value.pointer_value;
			/* Set the final value */
			if (prv->root)

				_xw_changed_document(c, p, (Etk_Canvas *)o);
			else
				_w_changed_canvas(c, p, (Etk_Canvas *)o);
			/* Set the final value */
			r.w = c->final;
			ekeko_renderable_geometry_set((Ekeko_Renderable *)o, &r);
		}
		else if (!strcmp(em->prop, "h"))
		{
			Eina_Rectangle r;
			Etk_Coord *c, *p;

			ekeko_renderable_geometry_get((Ekeko_Renderable *)o, &r);
			c = em->curr->value.pointer_value;
			p = em->prev->value.pointer_value;
			/* Set the final value */
			if (prv->root)
				_yh_changed_document(c, p, (Etk_Canvas *)o);
			else
				_h_changed_canvas(c, p, (Etk_Canvas *)o);
			r.h = c->final;
			ekeko_renderable_geometry_set((Ekeko_Renderable *)o, &r);
		}
	}
}

static void _child_append_cb(const Ekeko_Object *o, Event *e, void *data)
{
	Etk_Canvas *c;
	Etk_Canvas_Private *prv;
	Event_Mutation *em = (Event_Mutation *)e;

	/* FIXME the canvas can only have one parent, either a document
	 * or a canvas
	 */
	c = (Etk_Canvas *)o;
	prv = PRIVATE(c);

	/* in case the parent is a document retrieve the engine
	 * in case the parent is a canvas retrieve the engine from the relative
	 * document
	 */
	/* parent is a canvas */
	if (ekeko_type_instance_is_of(em->related, ETK_TYPE_CANVAS))
	{
		/* FIXME what happens if the canvas is appended to another canvas
		 * and the parent doesnt have a document?
		 */
		prv->doc = etk_canvas_document_get((Etk_Canvas *)em->related);
	}
	/* parent is a document */
	else
	{
		prv->doc = (Etk_Document *)em->related;
		prv->root = EINA_TRUE;
		/* whenever the document changes geometry we should set up
		 * the surface again
		 */
		ekeko_event_listener_add((Ekeko_Object *)em->related,
				EVENT_PROP_MODIFY, _document_geometry_cb,
				EINA_FALSE, c);
	}
}

static Eina_Bool _appendable(void *instance, void *child)
{
#if 0
	if (!strcmp(name, "EtkRect"))
		return EINA_TRUE;
	else if (!strcmp(name, "EtkCanvas"))
		return EINA_TRUE;
	else
		return EINA_FALSE;
#endif
	return EINA_TRUE;
}

static void _ctor(void *instance)
{
	Etk_Canvas *dc;
	Etk_Canvas_Private *prv;

	dc = (Etk_Canvas*) instance;
	dc->private = prv = ekeko_type_instance_private_get(etk_canvas_type_get(), instance);
	dc->parent.flush = _flush;
	dc->parent.parent.render = _subcanvas_render;
	ekeko_event_listener_add((Ekeko_Object *)dc, EVENT_PROP_MODIFY, _prop_modify_cb, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)dc, EVENT_OBJECT_APPEND, _child_append_cb, EINA_FALSE, NULL);
}

static void _dtor(void *canvas)
{

}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Etk_Document * etk_canvas_document_get(Etk_Canvas *c)
{
	Etk_Canvas_Private *prv;

	prv = PRIVATE(c);
	return prv->doc;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Ekeko_Type *etk_canvas_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(ETK_TYPE_CANVAS, sizeof(Etk_Canvas),
				sizeof(Etk_Canvas_Private), ekeko_canvas_type_get(),
				_ctor, _dtor, _appendable);
		TYPE_PROP_SINGLE_ADD(type, "x", ETK_PROPERTY_COORD, OFFSET(Etk_Canvas_Private, x));
		TYPE_PROP_SINGLE_ADD(type, "y", ETK_PROPERTY_COORD, OFFSET(Etk_Canvas_Private, y));
		TYPE_PROP_SINGLE_ADD(type, "w", ETK_PROPERTY_COORD, OFFSET(Etk_Canvas_Private, w));
		TYPE_PROP_SINGLE_ADD(type, "h", ETK_PROPERTY_COORD, OFFSET(Etk_Canvas_Private, h));

	}

	return type;
}

EAPI Etk_Canvas * etk_canvas_new(void)
{
	return ekeko_type_instance_new(etk_canvas_type_get());
}

EAPI void etk_canvas_root_set(Etk_Canvas *c)
{
	Etk_Canvas_Private *prv;

	prv = PRIVATE(c);
	prv->root = EINA_TRUE;
}

EAPI void etk_canvas_x_rel_set(Etk_Canvas *c, int x)
{
	Etk_Coord coord;
	Ekeko_Value v;

	etk_value_coord_from(&v, &coord, x, ETK_COORD_RELATIVE);
	ekeko_object_property_value_set((Ekeko_Object *)c, "x", &v);
}

EAPI void etk_canvas_x_set(Etk_Canvas *c, int x)
{
	Etk_Coord coord;
	Ekeko_Value v;

	etk_value_coord_from(&v, &coord, x, ETK_COORD_ABSOLUTE);
	ekeko_object_property_value_set((Ekeko_Object *)c, "x", &v);
}

EAPI void etk_canvas_y_set(Etk_Canvas *c, int y)
{
	Etk_Coord coord;
	Ekeko_Value v;

	etk_value_coord_from(&v, &coord, y, ETK_COORD_ABSOLUTE);
	ekeko_object_property_value_set((Ekeko_Object *)c, "y", &v);
}

EAPI void etk_canvas_y_rel_set(Etk_Canvas *c, int y)
{
	Etk_Coord coord;
	Ekeko_Value v;

	etk_value_coord_from(&v, &coord, y, ETK_COORD_RELATIVE);
	ekeko_object_property_value_set((Ekeko_Object *)c, "y", &v);
}

EAPI void etk_canvas_w_set(Etk_Canvas *c, int w)
{
	Etk_Coord coord;
	Ekeko_Value v;

	etk_value_coord_from(&v, &coord, w, ETK_COORD_ABSOLUTE);
	ekeko_object_property_value_set((Ekeko_Object *)c, "w", &v);
}

EAPI void etk_canvas_w_rel_set(Etk_Canvas *c, int w)
{
	Etk_Coord coord;
	Ekeko_Value v;

	etk_value_coord_from(&v, &coord, w, ETK_COORD_RELATIVE);
	ekeko_object_property_value_set((Ekeko_Object *)c, "w", &v);
}

EAPI void etk_canvas_h_set(Etk_Canvas *c, int h)
{
	Etk_Coord coord;
	Ekeko_Value v;

	etk_value_coord_from(&v, &coord, h, ETK_COORD_ABSOLUTE);
	ekeko_object_property_value_set((Ekeko_Object *)c, "h", &v);
}

EAPI void etk_canvas_h_rel_set(Etk_Canvas *c, int h)
{
	Etk_Coord coord;
	Ekeko_Value v;

	etk_value_coord_from(&v, &coord, h, ETK_COORD_RELATIVE);
	ekeko_object_property_value_set((Ekeko_Object *)c, "h", &v);
}

EAPI void etk_canvas_w_get(Etk_Canvas *c, Etk_Coord *w)
{
	Etk_Canvas_Private *prv;

	prv = PRIVATE(c);
	*w = prv->w;
}

EAPI void etk_canvas_x_get(Etk_Canvas *c, Etk_Coord *x)
{
	Etk_Canvas_Private *prv;

	prv = PRIVATE(c);
	*x = prv->x;
}

EAPI void * etk_canvas_surface_get(Etk_Canvas *c)
{
	Etk_Canvas_Private *prv;

	prv = PRIVATE(c);
	return prv->s;
}
