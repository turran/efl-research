/*
 * eon_shape.c
 *
 *  Created on: 04-feb-2009
 *      Author: jl
 */
#include "Eon.h"
#include "eon_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define BOUNDING_DEBUG 0
#define PRIVATE(d) ((Eon_Shape_Private *)((Eon_Shape *)(d))->private)

struct _Eon_Shape_Private
{
	void *context;
	Eon_Color color; /* FIXME the color should be double state? */
	Eon_Filter *filter;
	int rop;
 	/* TODO we'll only support clipping to a rect */
	Eina_Rectangle *clip;
	/* TODO add the engine data here instead of on every shape subclass */
	void *engine_data;
};

static void _child_append_cb(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{
	Ekeko_Event_Mutation *em = (Ekeko_Event_Mutation *)e;
	Eon_Shape *s;
	Eon_Shape_Private *prv;
	Eon_Document *d;
	Eon_Engine *eng;

	/* when this shape is appended to a canvas, try to setup the context */
	d = eon_canvas_document_get((Eon_Canvas *)em->related);
	/* FIXME in case the canvas doesnt have a document */
	eng = eon_document_engine_get(d);
	s = (Eon_Shape *)o;
	prv = PRIVATE(s);
	prv->engine_data = s->create(eng, s); 
}


/* Render wrapper */
static void _render(Ekeko_Renderable *r, Eina_Rectangle *rect)
{
	Eon_Shape *s;
	Eon_Shape_Private *prv;
	Eon_Document *d;
	Eon_Canvas *c;
	Eon_Engine *eng;
	Eon_Surface *surface;

	s = (Eon_Shape *)r;
	prv = PRIVATE(s);
	c = (Eon_Canvas *)ekeko_renderable_canvas_get(r);
	d = eon_canvas_document_get(c);
	eng = eon_document_engine_get(d);
	surface = eon_canvas_surface_get(c);
#if BOUNDING_DEBUG
	{
#if 0
		Eon_Context *ctx;

		ctx = func->context->create();
		func->context->color_set(ctx, 0xffaaaaaa);
		func->context->rop_set(ctx, ENESIM_FILL);
		func->canvas->lock(surface);
		func->shape->rect(surface, ctx, rect->x, rect->y, rect->w, rect->h);
		func->canvas->unlock(surface);
		func->context->delete(ctx);
#endif
	}
#endif
	/* Call the shape's render function */
	s->render(s, eng, prv->engine_data, surface, rect);
}

static void _rop_change(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{
	/* before adding the damage check that the rop has changed */
	eon_shape_change((Eon_Shape *)o);
}

static void _color_change(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{
	/* before adding the damage check that the clor has changed */
	eon_shape_change((Eon_Shape *)o);
}

static void _filter_change(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{
	printf("CALLEEEEEEEEEEEEE\n");
}

static void _ctor(void *instance)
{
	Eon_Shape *s;
	Eon_Shape_Private *prv;

	s = (Eon_Shape*) instance;
	s->private = prv = ekeko_type_instance_private_get(eon_shape_type_get(), instance);
	s->parent.render = _render;
	/* the default color, useful for pixel_color operations */
	prv->color = 0xffffffff;
	ekeko_event_listener_add((Ekeko_Object *)s, EVENT_OBJECT_APPEND, _child_append_cb, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)s, EON_SHAPE_COLOR_CHANGED, _color_change, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)s, EON_SHAPE_ROP_CHANGED, _rop_change, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)s, EON_SHAPE_FILTER_CHANGED, _filter_change, EINA_FALSE, NULL);
}

static void _dtor(void *shape)
{

}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void * eon_shape_engine_data_get(Eon_Shape *s)
{
	Eon_Shape_Private *prv;

	prv = PRIVATE(s);
	return prv->engine_data;
}

void eon_shape_engine_data_set(Eon_Shape *s, void *engine_data)
{
	Eon_Shape_Private *prv;

	prv = PRIVATE(s);
	prv->engine_data = engine_data;
}

Eon_Canvas * eon_shape_canvas_get(Eon_Shape *s)
{
	return (Eon_Canvas *)ekeko_renderable_canvas_get((Ekeko_Renderable *)s);
}

void eon_shape_change(Eon_Shape *s)
{
	Ekeko_Object *parent;
	Eina_Rectangle geom;

	/* add a damage here */
	if (!(parent = ekeko_object_parent_get((Ekeko_Object *)s)))
		return;

	ekeko_renderable_geometry_get((Ekeko_Renderable *)s, &geom);
	ekeko_canvas_damage_add((Ekeko_Canvas *)parent, &geom);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
Ekeko_Property_Id EON_SHAPE_COLOR;
Ekeko_Property_Id EON_SHAPE_ROP;
Ekeko_Property_Id EON_SHAPE_FILTER;

EAPI Ekeko_Type *eon_shape_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(EON_TYPE_SHAPE, sizeof(Eon_Shape),
				sizeof(Eon_Shape_Private), ekeko_renderable_type_get(),
				_ctor, _dtor, NULL);
		EON_SHAPE_COLOR = TYPE_PROP_SINGLE_ADD(type, "color", EON_PROPERTY_COLOR, OFFSET(Eon_Shape_Private, color));
		EON_SHAPE_ROP = TYPE_PROP_SINGLE_ADD(type, "rop", PROPERTY_INT, OFFSET(Eon_Shape_Private, rop));
		EON_SHAPE_FILTER = TYPE_PROP_SINGLE_ADD(type, "filter", PROPERTY_OBJECT, OFFSET(Eon_Shape_Private, filter));
	}

	return type;
}

EAPI void eon_shape_color_set(Eon_Shape *s, Eon_Color color)
{
	Ekeko_Value v;

	eon_value_color_from(&v, color);
	ekeko_object_property_value_set((Ekeko_Object *)s, "color", &v);
}

EAPI Eon_Color eon_shape_color_get(Eon_Shape *s)
{
	Eon_Shape_Private *prv;

	prv = PRIVATE(s);
	return prv->color;
}

EAPI void eon_shape_rop_set(Eon_Shape *s, Enesim_Rop rop)
{
	Ekeko_Value v;

	ekeko_value_int_from(&v, rop);
	ekeko_object_property_value_set((Ekeko_Object *)s, "rop", &v);
}

EAPI Enesim_Rop eon_shape_rop_get(Eon_Shape *s)
{
	Eon_Shape_Private *prv;

	prv = PRIVATE(s);
	return prv->rop;
}

EAPI void eon_shape_filter_set(Eon_Shape *s, Eon_Filter *filter)
{
	Ekeko_Value v;

	ekeko_value_object_from(&v, (Ekeko_Object *)filter);
	ekeko_object_property_value_set((Ekeko_Object *)s, "filter", &v);
}

EAPI Eon_Filter * eon_shape_filter_get(Eon_Shape *s)
{
	Eon_Shape_Private *prv;

	prv = PRIVATE(s);
	return prv->filter;
}
