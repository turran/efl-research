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
};

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
}

static void _ctor(void *instance)
{
	Etk_Shape *s;
	Etk_Shape_Private *prv;

	s = (Etk_Shape*) instance;
	s->private = prv = ekeko_type_instance_private_get(etk_shape_type_get(), instance);
	ekeko_event_listener_add((Ekeko_Object *)s, EVENT_OBJECT_APPEND, _child_append_cb, EINA_FALSE, NULL);
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
	}

	return type;
}

EAPI void etk_shape_color_set(Etk_Shape *s, int color)
{
	Value v;

	value_int_from(&v, color);
	ekeko_object_property_value_set((Ekeko_Object *)s, "color", &v);
}
