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
#define BOUNDING_DEBUG
#define PRIVATE(d) ((Etk_Canvas_Private *)((Etk_Canvas *)(d))->private)
struct _Etk_Canvas_Private
{
	Etk_Coord x, y, w, h;
	/* how to handle the geometry of an object based on the matrix and the coords
	 * looks that child objects with relative coordinates use the geometry
	 */
	Enesim_Matrix matrix; /* user matrix S -> D */
	Enesim_Matrix inverse; /* inverse matrix D -> S*/

	/* track parent canvas, if parent canvas == NULL then this is the
	 * main canvas and we should treat it differently
	 */
	Etk_Surface *s;
	Etk_Context *context;
	Eina_Bool root;
	/* TODO overflow property */
	/* FIXME change this later */
	Etk_Document *doc;
};

/* in case the subcanvas has another canvas as parent it will blt to the
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
#ifndef ETK_DEBUG
	printf("[Etk_Canvas] Subcanvas render %d %d %d %d (%d %d %d %d)\n", srect.x, srect.y, srect.w, srect.h, rect->x, rect->y, rect->w, rect->h);
#endif
	func = etk_document_engine_get(sprv->doc);
#ifdef BOUNDING_DEBUG
	{
		Etk_Context *ctx;

		ctx = func->context->create();
		func->context->color_set(ctx, 0xffaaaaaa);
		func->context->rop_set(ctx, ENESIM_FILL);
		func->canvas->lock(cprv->s);
		func->shape->rect(cprv->s, ctx, rect->x, rect->y, rect->w, rect->h);
		func->canvas->unlock(cprv->s);
		func->context->delete(ctx);
	}
#endif
	/* blt there */
	{
		Enesim_Quad q;

		/* FIXME what to do in case the canvas has some transformation applied? */
		/* transform the rectangle too? */
		/* get the largest rectangle that fits on the matrix */
		enesim_matrix_rect_transform(&sprv->inverse, &sgeom, &q);
		enesim_quad_rectangle_to(&q, &srect);
		//ekeko_renderable_geometry_get(r, &sgeom);
		//eina_rectangle_rescale_in(&sgeom, &srect, &srect);
		//enesim_matrix_rect_transform(&sprv->matrix, rect, &q);
		//enesim_quad_rectangle_to(&q, rect);
	}

#ifndef ETK_DEBUG
	/* easy check to see if the transformation is working */
	rect->x = 0;
	rect->y = 72; // 0, 72
	rect->w = 105;
	rect->h = 72; // 143, 72
	printf("[ETK_Canvas] Blitting subcanvas %d,%d %dx%d %d,%d %dx%d\n",
			srect.x, srect.y, srect.w, srect.h,
			//0, 0, sprv->w.final, sprv->h.final,
			rect->x, rect->y, rect->w, rect->h);
#endif
	func->canvas->blit(sprv->s, &srect, sprv->context, cprv->s, rect);
}

static Eina_Bool _subcanvas_is_inside(Ekeko_Canvas *c, int x, int y)
{
	printf("CAAAAAAAAAAALED\n");
	return EINA_TRUE;
}
static inline Eina_Bool _subcanvas_flush(Ekeko_Canvas *c, Eina_Rectangle *r)
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
	printf("[Etk_Canvas] subcanvas adding a new damage %d %d %d %d (%d %d %d %d)\n",
			rscaled.x, rscaled.y, rscaled.w, rscaled.h,
			r->x, r->y, r->w, r->h);
	printf("[Etk_Canvas] subcanvas = %p, canvas = %p\n", c, dc);
	ekeko_canvas_damage_add(dc, &rscaled);
	return EINA_FALSE;
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
		return _subcanvas_flush(c, r);
	}
}

static void _document_surface_create(const Ekeko_Object *o, Event *e, void *data)
{
	Etk_Engine *func;
	Etk_Canvas *c;
	Etk_Canvas_Private *prv;
	Event_Mutation *em = (Event_Mutation *)e;

	func = etk_document_engine_get((Etk_Document *)o);
	c = data;
	prv = PRIVATE(c);
	prv->s = func->canvas->create(prv->root, em->curr->value.rect.w, em->curr->value.rect.h);
}

static void _geometry_change(const Ekeko_Object *o, Event *e, void *data)
{
	Event_Mutation *em = (Event_Mutation *)e;
	Etk_Canvas_Private *prv = PRIVATE(o);
	Etk_Engine *func;
	int w, h;

	/* check if the change is the rectangle */
	if (em->state != EVENT_MUTATION_STATE_POST)
		return;

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
	 * and w, h for non root (not the transformed size)
	 */
	w = prv->w.final;
	h = prv->h.final;
	func = etk_document_engine_get(prv->doc);
	if (!prv->context)
	{
		prv->context = func->context->create();
		func->context->matrix_set(prv->context, &prv->inverse);
	}
	prv->s = func->canvas->create(prv->root, w, h);
}

/* Once the matrix or the coordinates have changed, update the renderable
 * geometry */
static inline void _geometry_calc(Etk_Canvas *c)
{
	Etk_Canvas_Private *prv = PRIVATE(c);
	Eina_Rectangle r;

	eina_rectangle_coords_from(&r, prv->x.final, prv->y.final, prv->w.final,
			prv->h.final);
	/* in case of a subcanvas use the matrix */
	if (!prv->root)
	{
		/* compute the final geometry multiplying by the context matrix */
		Enesim_Quad qm, qi;
		float x1, y1, x2, y2, x3, y3, x4, y4;

		/* get the largest rectangle that fits on the matrix */
		printf("[Etk_Canvas] Geometry is %d %d %d %d\n", r.x, r.y, r.w, r.h);
		printf("[Etk_Canvas] Transforming with matrix = \n");
		{
			Enesim_Matrix *m = &prv->matrix;
			printf("[Etk_Canvas] Subcanvas matrix =\n            %f %f %f\n            %f %f %f\n            %f %f %f\n", m->xx, m->xy, m->xz, m->yx, m->yy, m->yz, m->zx, m->zy, m->zz);
		}
		enesim_matrix_rect_transform(&prv->matrix, &r, &qm);
		enesim_quad_coords_get(&qm, &x1, &y1, &x2, &y2, &x3, &y3, &x4, &y4);
		enesim_quad_rectangle_to(&qm, &r);
		printf("[Etk_Canvas] Destination quad =\n            %fx%f\n            %fx%f\n            %fx%f\n            %fx%f\n", x1, y1, x2, y2, x3, y3, x4, y4);
		enesim_matrix_rect_transform(&prv->inverse, &r, &qi);
		enesim_quad_coords_get(&qi, &x1, &y1, &x2, &y2, &x3, &y3, &x4, &y4);
		printf("[Etk_Canvas] Source quad =\n            %fx%f\n            %fx%f\n            %fx%f\n            %fx%f\n", x1, y1, x2, y2, x3, y3, x4, y4);
	}
	printf("[Etk_Canvas] Setting geometry of size %d %d %d %d\n",
			r.x, r.y, r.w, r.h);
	ekeko_renderable_geometry_set((Ekeko_Renderable *)c, &r);
}

/* Just informs that the x.final property has to be recalculated */
static void _x_inform(const Ekeko_Object *o, Event *e, void *data)
{
	Etk_Canvas_Private *prv = PRIVATE(data);
	Ekeko_Value v;

	printf("[Etk_Canvas] Informing X change\n");
	etk_value_coord_from(&v, &prv->x);
	ekeko_object_property_value_set((Ekeko_Object *)data, "x", &v);
}

/* Just informs that the y.final property has to be recalculated */
static void _y_inform(const Ekeko_Object *o, Event *e, void *data)
{
	Etk_Canvas_Private *prv = PRIVATE(data);
	Ekeko_Value v;

	printf("[Etk_Canvas] Informing Y change\n");
	etk_value_coord_from(&v, &prv->y);
	ekeko_object_property_value_set((Ekeko_Object *)data, "y", &v);
}

/* Just informs that the w.final property has to be recalculated */
static void _w_inform(const Ekeko_Object *o, Event *e, void *data)
{
	Etk_Canvas_Private *prv = PRIVATE(data);
	Ekeko_Value v;

	printf("[Etk_Canvas] Informing W change\n");
	etk_value_coord_from(&v, &prv->w);
	ekeko_object_property_value_set((Ekeko_Object *)data, "w", &v);
}

/* Just informs that the h.final property has to be recalculated */
static void _h_inform(const Ekeko_Object *o, Event *e, void *data)
{
	Etk_Canvas_Private *prv = PRIVATE(data);
	Ekeko_Value v;

	printf("[Etk_Canvas] Informing H change\n");
	etk_value_coord_from(&v, &prv->h);
	ekeko_object_property_value_set((Ekeko_Object *)data, "h", &v);
}

/* Called whenever the x property changes */
static void _x_change(const Ekeko_Object *o, Event *e, void *data)
{
	Event_Mutation *em = (Event_Mutation *)e;
	Etk_Canvas *c = (Etk_Canvas *)o;
	Etk_Canvas_Private *prv = PRIVATE(o);
	Ekeko_Object *parent;

	if (em->state == EVENT_MUTATION_STATE_POST)
		return;

	parent = ekeko_object_parent_get(o);
	if (prv->root)
	{
		int w;

		etk_document_size_get((Etk_Document *)parent, &w, NULL);
		etk_coord_change(o, &prv->x, em->curr->value.pointer_value,
				em->prev->value.pointer_value, 0, w, parent,
				NULL, ETK_DOCUMENT_SIZE_CHANGED, _x_inform);
		{
			Etk_Coord *c = em->curr->value.pointer_value;
			printf("X Change = %d\n", c->final);
		}
	}
	else
	{
		Etk_Canvas *cp = (Etk_Canvas *)parent;
		Etk_Canvas_Private *cprv = PRIVATE(cp);

		etk_coord_change(o, &prv->x, em->curr->value.pointer_value,
				em->prev->value.pointer_value, cprv->x.final, cprv->w.final, parent,
				ETK_CANVAS_X_CHANGED, ETK_CANVAS_W_CHANGED,
				_x_inform);
	}
	_geometry_calc(c);
}

/* Called whenever the y property changes */
static void _y_change(const Ekeko_Object *o, Event *e, void *data)
{
	Event_Mutation *em = (Event_Mutation *)e;
	Etk_Canvas *c = (Etk_Canvas *)o;
	Etk_Canvas_Private *prv = PRIVATE(o);
	Ekeko_Object *parent;

	if (em->state == EVENT_MUTATION_STATE_POST)
		return;

	parent = ekeko_object_parent_get(o);
	if (prv->root)
	{
		int h;

		etk_document_size_get((Etk_Document *)parent, NULL, &h);
		etk_coord_change(o, &prv->y, em->curr->value.pointer_value,
				em->prev->value.pointer_value, 0, h, parent,
				NULL, ETK_DOCUMENT_SIZE_CHANGED, _y_inform);
	}
	else
	{
		Etk_Canvas *cp = (Etk_Canvas *)parent;
		Etk_Canvas_Private *cprv = PRIVATE(cp);

		etk_coord_change(o, &prv->y, em->curr->value.pointer_value,
				em->prev->value.pointer_value, cprv->y.final, cprv->h.final, parent,
				ETK_CANVAS_Y_CHANGED, ETK_CANVAS_H_CHANGED, _y_inform);
	}
	_geometry_calc(c);
}

/* Called whenever the w property changes */
static void _w_change(const Ekeko_Object *o, Event *e, void *data)
{
	Event_Mutation *em = (Event_Mutation *)e;
	Etk_Canvas *c = (Etk_Canvas *)o;
	Etk_Canvas_Private *prv = PRIVATE(o);
	Ekeko_Object *parent;

	if (em->state == EVENT_MUTATION_STATE_POST)
		return;

	parent = ekeko_object_parent_get(o);
	if (prv->root)
	{
		int w;

		etk_document_size_get((Etk_Document *)parent, &w, NULL);
		etk_coord_length_change(o, &prv->w, em->curr->value.pointer_value,
				em->prev->value.pointer_value, w, parent,
				ETK_DOCUMENT_SIZE_CHANGED, _w_inform);
	}
	else
	{
		Etk_Canvas *cp = (Etk_Canvas *)parent;
		Etk_Canvas_Private *cprv = PRIVATE(cp);

		etk_coord_length_change(o, &prv->w, em->curr->value.pointer_value,
				em->prev->value.pointer_value, cprv->w.final, parent,
				ETK_CANVAS_W_CHANGED, _w_inform);
	}
	{
		printf("[Etk_Canvas] W Change = %d\n", prv->w.final);
	}
	_geometry_calc(c);
}

/* Called whenever the h property changes */
static void _h_change(const Ekeko_Object *o, Event *e, void *data)
{
	Event_Mutation *em = (Event_Mutation *)e;
	Etk_Canvas *c = (Etk_Canvas *)o;
	Etk_Canvas_Private *prv = PRIVATE(o);
	Ekeko_Object *parent;

	if (em->state == EVENT_MUTATION_STATE_POST)
		return;

	parent = ekeko_object_parent_get(o);
	if (prv->root)
	{
		int h;

		etk_document_size_get((Etk_Document *)parent, NULL, &h);
		etk_coord_length_change(o, &prv->h, em->curr->value.pointer_value,
				em->prev->value.pointer_value, h, parent,
				ETK_DOCUMENT_SIZE_CHANGED, _h_inform);
	}
	else
	{
		Etk_Canvas *cp = (Etk_Canvas *)parent;
		Etk_Canvas_Private *cprv = PRIVATE(cp);

		etk_coord_length_change(o, &prv->h, em->curr->value.pointer_value,
				em->prev->value.pointer_value, cprv->h.final, parent,
				ETK_CANVAS_H_CHANGED, _h_inform);
	}
	{
		printf("[Etk_Canvas] H Change = %d\n", prv->h.final);
	}
	_geometry_calc(c);
}

static void _matrix_change(const Ekeko_Object *o, Event *e, void *data)
{
	Event_Mutation *em = (Event_Mutation *)e;
	Etk_Canvas *c = (Etk_Canvas *)o;
	Etk_Canvas_Private *prv = PRIVATE(o);
	Enesim_Matrix *m;

	m = em->curr->value.pointer_value;
	enesim_matrix_inverse(m, &prv->inverse);
	/* update the geometry */
	_geometry_calc(c);
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
				ETK_DOCUMENT_SIZE_CHANGED, _document_surface_create,
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
	Etk_Canvas *c;
	Etk_Canvas_Private *prv;

	c = (Etk_Canvas*) instance;
	c->private = prv = ekeko_type_instance_private_get(etk_canvas_type_get(), instance);
	c->parent.flush = _flush;
	c->parent.parent.render = _subcanvas_render;
	c->parent.parent.is_inside= _subcanvas_is_inside;
	enesim_matrix_identity(&prv->matrix);
	enesim_matrix_inverse(&prv->matrix, &prv->inverse);
	ekeko_event_listener_add((Ekeko_Object *)c, ETK_CANVAS_X_CHANGED, _x_change, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)c, ETK_CANVAS_Y_CHANGED, _y_change, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)c, ETK_CANVAS_W_CHANGED, _w_change, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)c, ETK_CANVAS_H_CHANGED, _h_change, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)c, ETK_CANVAS_MATRIX_CHANGED, _matrix_change, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)c, EKEKO_RENDERABLE_GEOMETRY_CHANGED, _geometry_change, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)c, EVENT_OBJECT_APPEND, _child_append_cb, EINA_FALSE, NULL);
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
Property_Id ETK_CANVAS_X;
Property_Id ETK_CANVAS_Y;
Property_Id ETK_CANVAS_W;
Property_Id ETK_CANVAS_H;
Property_Id ETK_CANVAS_MATRIX;

EAPI Ekeko_Type *etk_canvas_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(ETK_TYPE_CANVAS, sizeof(Etk_Canvas),
				sizeof(Etk_Canvas_Private), ekeko_canvas_type_get(),
				_ctor, _dtor, _appendable);
		ETK_CANVAS_X = TYPE_PROP_SINGLE_ADD(type, "x", ETK_PROPERTY_COORD, OFFSET(Etk_Canvas_Private, x));
		ETK_CANVAS_Y = TYPE_PROP_SINGLE_ADD(type, "y", ETK_PROPERTY_COORD, OFFSET(Etk_Canvas_Private, y));
		ETK_CANVAS_W = TYPE_PROP_SINGLE_ADD(type, "w", ETK_PROPERTY_COORD, OFFSET(Etk_Canvas_Private, w));
		ETK_CANVAS_H = TYPE_PROP_SINGLE_ADD(type, "h", ETK_PROPERTY_COORD, OFFSET(Etk_Canvas_Private, h));
		ETK_CANVAS_MATRIX = TYPE_PROP_SINGLE_ADD(type, "matrix", ETK_PROPERTY_MATRIX, OFFSET(Etk_Canvas_Private, matrix));
	}

	return type;
}

EAPI Etk_Canvas * etk_canvas_new(Etk_Canvas *c)
{
	Etk_Canvas *sc;

	sc = ekeko_type_instance_new(etk_canvas_type_get());
	ekeko_object_child_append((Ekeko_Object *)c, (Ekeko_Object *)sc);

	return sc;
}


EAPI void etk_canvas_x_rel_set(Etk_Canvas *c, int x)
{
	Etk_Coord coord;
	Ekeko_Value v;

	etk_coord_set(&coord, x, ETK_COORD_RELATIVE);
	etk_value_coord_from(&v, &coord);
	ekeko_object_property_value_set((Ekeko_Object *)c, "x", &v);
}

EAPI void etk_canvas_x_set(Etk_Canvas *c, int x)
{
	Etk_Coord coord;
	Ekeko_Value v;

	etk_coord_set(&coord, x, ETK_COORD_ABSOLUTE);
	etk_value_coord_from(&v, &coord);
	ekeko_object_property_value_set((Ekeko_Object *)c, "x", &v);
}

EAPI void etk_canvas_y_set(Etk_Canvas *c, int y)
{
	Etk_Coord coord;
	Ekeko_Value v;

	etk_coord_set(&coord, y, ETK_COORD_ABSOLUTE);
	etk_value_coord_from(&v, &coord);
	ekeko_object_property_value_set((Ekeko_Object *)c, "y", &v);
}

EAPI void etk_canvas_y_rel_set(Etk_Canvas *c, int y)
{
	Etk_Coord coord;
	Ekeko_Value v;

	etk_coord_set(&coord, y, ETK_COORD_RELATIVE);
	etk_value_coord_from(&v, &coord);
	ekeko_object_property_value_set((Ekeko_Object *)c, "y", &v);
}

EAPI void etk_canvas_w_set(Etk_Canvas *c, int w)
{
	Etk_Coord coord;
	Ekeko_Value v;

	etk_coord_set(&coord, w, ETK_COORD_ABSOLUTE);
	etk_value_coord_from(&v, &coord);
	ekeko_object_property_value_set((Ekeko_Object *)c, "w", &v);
}

EAPI void etk_canvas_w_rel_set(Etk_Canvas *c, int w)
{
	Etk_Coord coord;
	Ekeko_Value v;

	etk_coord_set(&coord, w, ETK_COORD_RELATIVE);
	etk_value_coord_from(&v, &coord);
	ekeko_object_property_value_set((Ekeko_Object *)c, "w", &v);
}

EAPI void etk_canvas_h_set(Etk_Canvas *c, int h)
{
	Etk_Coord coord;
	Ekeko_Value v;

	etk_coord_set(&coord, h, ETK_COORD_ABSOLUTE);
	etk_value_coord_from(&v, &coord);
	ekeko_object_property_value_set((Ekeko_Object *)c, "h", &v);
}

EAPI void etk_canvas_h_rel_set(Etk_Canvas *c, int h)
{
	Etk_Coord coord;
	Ekeko_Value v;

	etk_coord_set(&coord, h, ETK_COORD_RELATIVE);
	etk_value_coord_from(&v, &coord);
	ekeko_object_property_value_set((Ekeko_Object *)c, "h", &v);
}

EAPI void etk_canvas_x_get(Etk_Canvas *c, Etk_Coord *x)
{
	Etk_Canvas_Private *prv;

	prv = PRIVATE(c);
	*x = prv->x;
}

EAPI void etk_canvas_y_get(Etk_Canvas *c, Etk_Coord *y)
{
	Etk_Canvas_Private *prv;

	prv = PRIVATE(c);
	*y = prv->y;
}

EAPI void etk_canvas_w_get(Etk_Canvas *c, Etk_Coord *w)
{
	Etk_Canvas_Private *prv;

	prv = PRIVATE(c);
	*w = prv->w;
}

EAPI void etk_canvas_h_get(Etk_Canvas *c, Etk_Coord *h)
{
	Etk_Canvas_Private *prv;

	prv = PRIVATE(c);
	*h = prv->h;
}

EAPI void * etk_canvas_surface_get(Etk_Canvas *c)
{
	Etk_Canvas_Private *prv;

	prv = PRIVATE(c);
	return prv->s;
}

EAPI void etk_canvas_matrix_set(Etk_Canvas *c, Enesim_Matrix *m)
{
	Ekeko_Value v;

	etk_value_matrix_from(&v, m);
	ekeko_object_property_value_set((Ekeko_Object *)c, "matrix", &v);
}
