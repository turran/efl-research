/*
 * etk2_shape.c
 *
 *  Created on: 04-feb-2009
 *      Author: jl
 */
#include "Etk2.h"
#include "etk2_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Etk_Shape_Private *)((Etk_Shape *)(d))->private)
#define TYPE_NAME "Etk_Shape"

struct _Etk_Shape_Private
{
	void *context;
	int color;
	int rop;
};

/* Render wrapper */
static void _render(Ekeko_Renderable *r, Eina_Rectangle *rect)
{
	Etk_Shape *s;
	Etk_Shape_Private *prv;
	Etk_Document *d;
	Etk_Canvas *c;
	Etk_Engine *func;
	Etk_Surface *surface;

	s = (Etk_Shape *)r;
	prv = PRIVATE(s);
	c = (Etk_Canvas *)ekeko_renderable_canvas_get(r);
	d = etk_canvas_document_get(c);
	func = etk_document_engine_get(d);
	surface = etk_canvas_surface_get(c);
	/* TODO context clip set */
	func->canvas->lock(surface);
	/* Call the shape's render function */
	s->render(s, func, surface, prv->context);
	func->canvas->unlock(surface);
	/* TODO context clear */
}

static void _prop_modify_cb(const Ekeko_Object *o, Event *e, void *data)
{
	Ekeko_Object *parent;
	Event_Mutation *em = (Event_Mutation *)e;
	Etk_Shape *s = (Etk_Shape *)o;
	Etk_Document *d;
	Etk_Shape_Private *prv;

	if (!(parent = ekeko_object_parent_get(o)))
		return;
	if (!strcmp(em->prop, "color"))
	{
		Etk_Engine *func;

		d = etk_canvas_document_get((Etk_Canvas *)parent);
		func = etk_document_engine_get(d);
		prv = PRIVATE(s);
		func->context->color_set(prv->context, em->curr->value.int_value);
	}
	else if (!strcmp(em->prop, "rop"))
	{
		Etk_Engine *func;

		d = etk_canvas_document_get((Etk_Canvas *)parent);
		func = etk_document_engine_get(d);
		prv = PRIVATE(s);
		func->context->rop_set(prv->context, em->curr->value.int_value);
	}
}

static void _child_append_cb(const Ekeko_Object *o, Event *e, void *data)
{
	Event_Mutation *em = (Event_Mutation *)e;
	Etk_Shape *s;
	Etk_Shape_Private *prv;
	Etk_Document *d;
	Etk_Engine *func;

	/* when this shape is appended to a canvas, try to setup the context */
	d = etk_canvas_document_get((Etk_Canvas *)em->related);
	/* FIXME in case the canvas doesnt have a document */
	func = etk_document_engine_get(d);
	s = (Etk_Shape *)o;
	prv = PRIVATE(s);
	prv->context = func->context->create();
	/* TODO set the color, the rop, etc */
}

static void _ctor(void *instance)
{
	Etk_Shape *s;
	Etk_Shape_Private *prv;

	s = (Etk_Shape*) instance;
	s->private = prv = ekeko_type_instance_private_get(etk_shape_type_get(), instance);
	s->parent.render = _render;
	ekeko_event_listener_add((Ekeko_Object *)s, EVENT_OBJECT_APPEND, _child_append_cb, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)s, EVENT_PROP_MODIFY, _prop_modify_cb, EINA_FALSE, NULL);
}

static void _dtor(void *shape)
{

}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void * etk_shape_context_get(Etk_Shape *s)
{
	Etk_Shape_Private *prv;

	prv = PRIVATE(s);
	return prv->context;
}
Etk_Canvas * etk_shape_canvas_get(Etk_Shape *s)
{
	return (Etk_Canvas *)ekeko_renderable_canvas_get((Ekeko_Renderable *)s);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Ekeko_Type *etk_shape_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(TYPE_NAME, sizeof(Etk_Shape),
				sizeof(Etk_Shape_Private), ekeko_renderable_type_get(),
				_ctor, _dtor, NULL);
		TYPE_PROP_SINGLE_ADD(type, "color", PROPERTY_INT, OFFSET(Etk_Shape_Private, color));
		TYPE_PROP_SINGLE_ADD(type, "rop", PROPERTY_INT, OFFSET(Etk_Shape_Private, rop));
	}

	return type;
}

EAPI void etk_shape_color_set(Etk_Shape *s, int color)
{
	Ekeko_Value v;

	ekeko_value_int_from(&v, color);
	ekeko_object_property_value_set((Ekeko_Object *)s, "color", &v);
}

EAPI void etk_shape_rop_set(Etk_Shape *s, int rop)
{
	Ekeko_Value v;

	ekeko_value_int_from(&v, rop);
	ekeko_object_property_value_set((Ekeko_Object *)s, "rop", &v);
}
