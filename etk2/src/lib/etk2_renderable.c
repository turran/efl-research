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
	/* the geometry */
	Eina_Rectangle geom;
	Eina_Rectangle geom_prev;
	char geom_changed;
	/* TODO the visibility */
};

/* called whenever a double state property has changed */
static void _properties_updated(const Object *o, Event *e)
{
	Event_Mutation *em = (Event_Mutation *)e;
	printf("CALLEDDDDDDDDDDDDDDDD %d\n", em->state);
	/* TODO geometry changed */
	/* TODO visibility changed */
}

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
	event_listener_add((Object *)rend, EVENT_PROP_MODIFY, _properties_updated, EINA_FALSE);
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
	Renderable_Private *prv;
	Eina_Rectangle move;
	Value value;

	/* TODO avoid this duplicate */
	prv = PRIVATE(r);
	move.x = x;
	move.y = y;
	move.w = prv->geom.w;
	move.h = prv->geom.h;
	value_rectangle_from(&value, &move);
	object_property_value_set((Object *)r, "geometry", &value);
}

EAPI void renderable_resize(Renderable *r, int w, int h)
{
	Renderable_Private *prv;
	Eina_Rectangle resize;
	Value value;

	/* TODO avoid this duplicate */
	prv = PRIVATE(r);
	resize.w = w;
	resize.h = h;
	resize.w = prv->geom.x;
	resize.h = prv->geom.y;
	value_rectangle_from(&value, &resize);
	object_property_value_set((Object *)r, "geometry", &value);
}

EAPI void renderable_geometry_set(Renderable *r, Eina_Rectangle *rect)
{
	Value value;

	printf("[rend] geometry_set %d %d %d %d\n", rect->x, rect->y, rect->w, rect->h);
	value_rectangle_from(&value, rect);
	object_property_value_set((Object *)r, "geometry", &value);
}

EAPI void renderable_geometry_get(Renderable *r, Eina_Rectangle *rect)
{
	Renderable_Private *prv;

	prv = PRIVATE(r);
	*rect = prv->geom;
}
