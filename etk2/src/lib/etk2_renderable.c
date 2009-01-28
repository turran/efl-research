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
#define PRIVATE(rend) ((Renderable_Private*)((Renderable *)(rend))->private)

#define TYPE_NAME "Renderable"

struct _Renderable_Private
{
	Canvas *canvas;
	/* the geometry */
	struct
	{

		Eina_Rectangle curr;
		Eina_Rectangle prev;
		char changed;
	} geometry;
	/* the visibility */
	struct
	{
		Eina_Bool curr;
		Eina_Bool prev;
		char changed;
	} visibility;
	/* this renderable is appended to the list of renderables of the canvas */
	Eina_Bool appended;
	/* TODO we should have a way to inform the canvas that this
	 * renderable needs the lower object to draw in that case
	 * also render the bottom one
	 */
};

/* called whenever a double state property has changed */
static void _properties_updated(const Object *o, Event *e)
{
	Event_Mutation *em = (Event_Mutation *)e;
	Renderable_Private *prv = PRIVATE(o);

	printf("[renderable %s] prop updated %s\n", object_type_name_get(o), em->prop);
	if (em->state != EVENT_MUTATION_STATE_POST)
		return;
	if (!prv->canvas)
		return;
	/* geometry changed */
	if (!strcmp(em->prop, "geometry"))
	{
		/* TODO check that the renderable is appended? */
		canvas_damage_add(prv->canvas, &em->curr->value.rect);
		canvas_damage_add(prv->canvas, &em->prev->value.rect);
	}
	/* visibility changed */
	else if (!strcmp(em->prop, "visibility"))
	{
		/* TODO check that the renderable is appended? */
		{
			Eina_Rectangle *r;

			r = &prv->geometry.curr;
			printf("%d %d %d %d\n", r->x, r->y, r->w, r->h);
		}
		canvas_damage_add(prv->canvas, &prv->geometry.curr);
		canvas_damage_add(prv->canvas, &prv->geometry.prev);
	}
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
		printf("[renderable %s] Is not of type canvas\n", object_type_name_get(o));
		return;
	}
	printf("[renderable %s] Some parent %p (%s) is a canvas? %p!!!\n", object_type_name_get(o), p, object_type_name_get(p), renderable_canvas_get((Renderable *)p));
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
	printf("[renderable] ctor canvas = %p\n", prv->canvas);
}

static void _dtor(void *instance)
{

}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eina_Bool renderable_appended_get(Renderable *r)
{
	Renderable_Private *prv;

	prv = PRIVATE(r);
	return prv->appended;
}
void renderable_appended_set(Renderable *r, Eina_Bool appended)
{
	Renderable_Private *prv;

	prv = PRIVATE(r);
	prv->appended = appended;
}
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
				OFFSET(Renderable_Private, geometry.curr), OFFSET(Renderable_Private, geometry.prev),
				OFFSET(Renderable_Private, geometry.changed));
		TYPE_PROP_DOUBLE_ADD(renderable_type, "visibility", PROPERTY_BOOL,
				OFFSET(Renderable_Private, visibility.curr), OFFSET(Renderable_Private, visibility.prev),
				OFFSET(Renderable_Private, visibility.changed));
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
	move.w = prv->geometry.curr.w;
	move.h = prv->geometry.curr.h;
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
	resize.x = prv->geometry.curr.x;
	resize.y = prv->geometry.curr.y;
	resize.w = w;
	resize.h = h;
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
	*rect = prv->geometry.curr;
}

EAPI void renderable_visibility_get(Renderable *r, Eina_Bool *visible)
{
	Renderable_Private *prv;

	prv = PRIVATE(r);
	printf("[renderable %s] visibility get %d (%d %d)\n", object_type_name_get((Object *)r), prv->visibility.curr, EINA_FALSE, EINA_TRUE);
	*visible = prv->visibility.curr;
}

EAPI void renderable_show(Renderable *r)
{
	Renderable_Private *prv;
	Value value;

	prv = PRIVATE(r);
	printf("[renderable] show\n");
	if (prv->visibility.curr)
		return;
	value_bool_from(&value, EINA_TRUE);
	object_property_value_set((Object *)r, "visibility", &value);
}

EAPI void renderable_hide(Renderable *r)
{
	Renderable_Private *prv;
	Value value;

	prv = PRIVATE(r);
	printf("[renderable] hide\n");
	if (!prv->visibility.curr)
		return;
	value_bool_from(&value, EINA_FALSE);
	object_property_value_set((Object *)r, "visibility", &value);
}

EAPI void renderable_visibility_set(Renderable *r, Eina_Bool visible)
{
	Renderable_Private *prv;
	Value value;

	prv = PRIVATE(r);
	if (prv->visibility.curr == visible)
		return;
	value_bool_from(&value, visible);
	printf("[renderable %s] visibility set %d\n", object_type_name_get((Object *)r), value.value.bool_value);
	object_property_value_set((Object *)r, "visibility", &value);
}

EAPI Canvas * renderable_canvas_get(Renderable *r)
{
	Renderable_Private *prv;

	prv = PRIVATE(r);
	return prv->canvas;
}
