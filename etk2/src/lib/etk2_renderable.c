/*
 * etk2_renderable.c
 *
 *  Created on: 10-ene-2009
 *      Author: jl
 */
#include "Etk2.h"
#include "etk2_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(rend) ((Renderable_Private*)(rend->private))

#define TYPE_NAME "Renderable"

struct _Renderable_Private
{
	Canvas *canvas;
	Eina_Rectangle geom;
	Eina_Rectangle geom_prev;
	char geom_changed;
};

static void _parent_set_cb(const Object *o, Event *e)
{
	Renderable_Private *prv;
	Object *p = (Object *)e->target;

	/* check that the upper hierarchy is of type canvas */
	while (!type_instance_is_of(p, "Canvas"))
	{
		p = object_parent_get(p);
	}
	if (!p)
	{
		printf("Is not of type canvas\n");
		return;
	}
	printf("Some parent is a canvas!!!\n");
	prv = PRIVATE(((Renderable *)o));
	prv->canvas = (Canvas *)p;

}

static void _ctor(void *instance)
{
	Renderable *rend;
	Renderable_Private *prv;

	rend = (Renderable*) instance;
	rend->private = prv = type_instance_private_get(renderable_type_get(), instance);
	/* register to an event where this child is appended to a canvas parent */
	event_listener_add((Object *)rend, EVENT_OBJECT_APPEND, _parent_set_cb, EINA_FALSE);
}

static void _dtor(void *instance)
{

}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
Type *renderable_type_get(void)
{
	static Type *renderable_type = NULL;

	if (!renderable_type)
	{
		renderable_type = type_new(TYPE_NAME, sizeof(Renderable),
				sizeof(Renderable_Private), object_type_get(), _ctor, _dtor);
		/* the properties */
		TYPE_PROP_DOUBLE_ADD(renderable_type, "geometry", PROPERTY_RECTANGLE,
				OFFSET(Renderable_Private, geom), OFFSET(Renderable_Private, geom_prev),
				OFFSET(Renderable_Private, geom_changed));
	}

	return renderable_type;
}

EAPI void renderable_move(Renderable *r, int x, int y)
{

}

EAPI void renderable_resize(Renderable *r, int w, int h)
{

}
