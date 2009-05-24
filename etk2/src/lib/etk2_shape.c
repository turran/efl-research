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
#define BOUNDING_DEBUG 0
#define PRIVATE(d) ((Etk_Shape_Private *)((Etk_Shape *)(d))->private)
#define TYPE_NAME "Etk_Shape"

struct _Etk_Shape_Private
{
	void *context;
	Etk_Color color; /* FIXME the color should be double state? */
	Etk_Filter *filter;
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

#if BOUNDING_DEBUG
	{
		Etk_Context *ctx;

		ctx = func->context->create();
		func->context->color_set(ctx, 0xffaaaaaa);
		func->context->rop_set(ctx, ENESIM_FILL);
		func->canvas->lock(surface);
		func->shape->rect(surface, ctx, rect->x, rect->y, rect->w, rect->h);
		func->canvas->unlock(surface);
		func->context->delete(ctx);
	}
#endif
	/* TODO context clip set */
	func->context->clip_set(prv->context, rect);
	//printf("Rendering %s\n", ekeko_object_type_name_get((Ekeko_Object *)s));
	func->canvas->lock(surface);
	/* Call the shape's render function */
	s->render(s, func, surface, prv->context);
	func->context->clip_clear(prv->context);
	func->canvas->unlock(surface);
	/* TODO context clear ? */
}

static void _rop_change(const Ekeko_Object *o, Event *e, void *data)
{
	Etk_Engine *func;
	Ekeko_Object *parent;
	Event_Mutation *em = (Event_Mutation *)e;
	Etk_Shape *s = (Etk_Shape *)o;
	Etk_Document *d;
	Etk_Shape_Private *prv;

	if (!(parent = ekeko_object_parent_get(o)))
		return;

	d = etk_canvas_document_get((Etk_Canvas *)parent);
	func = etk_document_engine_get(d);
	prv = PRIVATE(s);
	func->context->rop_set(prv->context, em->curr->value.int_value);
	/* before adding the damage check that the rop has changed */
	etk_shape_change(s);
}

static void _color_change(const Ekeko_Object *o, Event *e, void *data)
{
	Etk_Engine *func;
	Ekeko_Object *parent;
	Event_Mutation *em = (Event_Mutation *)e;
	Etk_Shape *s = (Etk_Shape *)o;
	Etk_Document *d;
	Etk_Shape_Private *prv;

	if (!(parent = ekeko_object_parent_get(o)))
		return;

	d = etk_canvas_document_get((Etk_Canvas *)parent);
	func = etk_document_engine_get(d);
	prv = PRIVATE(s);
	func->context->color_set(prv->context, em->curr->value.int_value);
	etk_shape_change(s);
}

static void _filter_change(const Ekeko_Object *o, Event *e, void *data)
{
	printf("CALLEEEEEEEEEEEEE\n");
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
	func->context->color_set(prv->context, prv->color);
	/* TODO set the color, the rop, etc */
}

static void _ctor(void *instance)
{
	Etk_Shape *s;
	Etk_Shape_Private *prv;

	s = (Etk_Shape*) instance;
	s->private = prv = ekeko_type_instance_private_get(etk_shape_type_get(), instance);
	s->parent.render = _render;
	/* the default color, useful for pixel_color operations */
	prv->color = 0xffffffff;
	printf("color = %08x\n", prv->color);
	ekeko_event_listener_add((Ekeko_Object *)s, EVENT_OBJECT_APPEND, _child_append_cb, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)s, ETK_SHAPE_COLOR_CHANGED, _color_change, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)s, ETK_SHAPE_ROP_CHANGED, _rop_change, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)s, ETK_SHAPE_FILTER_CHANGED, _filter_change, EINA_FALSE, NULL);
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

void etk_shape_change(Etk_Shape *s)
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
Property_Id ETK_SHAPE_COLOR;
Property_Id ETK_SHAPE_ROP;
Property_Id ETK_SHAPE_FILTER;

EAPI Ekeko_Type *etk_shape_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(TYPE_NAME, sizeof(Etk_Shape),
				sizeof(Etk_Shape_Private), ekeko_renderable_type_get(),
				_ctor, _dtor, NULL);
		ETK_SHAPE_COLOR = TYPE_PROP_SINGLE_ADD(type, "color", ETK_PROPERTY_COLOR, OFFSET(Etk_Shape_Private, color));
		ETK_SHAPE_ROP = TYPE_PROP_SINGLE_ADD(type, "rop", PROPERTY_INT, OFFSET(Etk_Shape_Private, rop));
		ETK_SHAPE_FILTER = TYPE_PROP_SINGLE_ADD(type, "filter", PROPERTY_OBJECT, OFFSET(Etk_Shape_Private, filter));
	}

	return type;
}

EAPI void etk_shape_color_set(Etk_Shape *s, Etk_Color color)
{
	Ekeko_Value v;

	etk_value_color_from(&v, color);
	ekeko_object_property_value_set((Ekeko_Object *)s, "color", &v);
}

EAPI void etk_shape_rop_set(Etk_Shape *s, Enesim_Rop rop)
{
	Ekeko_Value v;

	ekeko_value_int_from(&v, rop);
	ekeko_object_property_value_set((Ekeko_Object *)s, "rop", &v);
}

EAPI void etk_shape_filter_set(Etk_Shape *s, Etk_Filter *filter)
{
	Ekeko_Value v;

	ekeko_value_object_from(&v, (Ekeko_Object *)filter);
	ekeko_object_property_value_set((Ekeko_Object *)s, "filter", &v);
}
