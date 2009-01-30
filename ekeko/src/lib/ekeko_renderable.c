/*
 * etk2_renderable.c
 *
 *  Created on: 10-ene-2009
 *      Author: jl
 */
#include "Ekeko.h"
#include "ekeko_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(rend) ((Ekeko_Renderable_Private*)((Ekeko_Renderable *)(rend))->private)

#define TYPE_NAME "Renderable"

struct _Ekeko_Renderable_Private
{
	Ekeko_Canvas *canvas;
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
	/* When the canvas manages the list of renderables it needs to know if
	 * a renderable is appended */
	Eina_Bool appended;
	/* TODO we should have a way to inform the canvas that this
	 * renderable needs the lower object to draw in that case
	 * also render the bottom one
	 */
};

/* called whenever a double state property has changed */
static void _properties_updated(const Ekeko_Object *o, Event *e, void *data)
{
	Event_Mutation *em = (Event_Mutation *)e;
	Ekeko_Renderable_Private *prv = PRIVATE(o);

#ifdef ETK2_DEBUG
	printf("[renderable %s] prop updated %s\n", ekeko_object_type_name_get(o), em->prop);
#endif
	if (em->state != EVENT_MUTATION_STATE_POST)
		return;
	if (!prv->canvas)
		return;
	/* geometry changed */
	if (!strcmp(em->prop, "geometry"))
	{
		/* TODO check that the renderable is appended? */
		ekeko_canvas_damage_add(prv->canvas, &em->curr->value.rect);
		ekeko_canvas_damage_add(prv->canvas, &em->prev->value.rect);
	}
	/* visibility changed */
	else if (!strcmp(em->prop, "visibility"))
	{
		/* TODO check that the renderable is appended? */
		ekeko_canvas_damage_add(prv->canvas, &prv->geometry.curr);
		ekeko_canvas_damage_add(prv->canvas, &prv->geometry.prev);
	}
}

static void _parent_set_cb(const Ekeko_Object *o, Event *e, void *data)
{
	Event_Mutation *em = (Event_Mutation *)e;
	Ekeko_Renderable_Private *prv;
	Ekeko_Object *p = (Ekeko_Object *)em->related;

	/* check that the upper hierarchy is of type canvas */
	while (!ekeko_type_instance_is_of(p, "Canvas"))
	{
		p = ekeko_object_parent_get(p);
	}
	if (!p)
	{
#ifdef ETK2_DEBUG
		printf("[renderable %s] Is not of type canvas\n", ekeko_object_type_name_get(o));
#endif
		return;
	}
#ifdef ETK2_DEBUG
	printf("[renderable %s] Some parent %p (%s) is a canvas? %p!!!\n", ekeko_object_type_name_get(o), p, ekeko_object_type_name_get(p), ekeko_renderable_canvas_get((Ekeko_Renderable *)p));
#endif
	prv = PRIVATE(((Ekeko_Renderable *)o));
#ifdef ETK2_DEBUG
	printf("[renderable] %p has a canvas at %p\n", o, p);
#endif
	prv->canvas = (Ekeko_Canvas *)p;
}

static void _ctor(void *instance)
{
	Ekeko_Renderable *rend;
	Ekeko_Renderable_Private *prv;

	rend = (Ekeko_Renderable*) instance;
	rend->private = prv = ekeko_type_instance_private_get(ekeko_renderable_type_get(), instance);
	/* register to an event where this child is appended to a canvas parent */
	ekeko_event_listener_add((Ekeko_Object *)rend, EVENT_PROP_MODIFY, _properties_updated, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)rend, EVENT_OBJECT_APPEND, _parent_set_cb, EINA_FALSE, NULL);
#ifdef ETK2_DEBUG
	printf("[renderable] ctor canvas = %p\n", prv->canvas);
#endif
}

static void _dtor(void *instance)
{

}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
Eina_Bool renderable_appended_get(Ekeko_Renderable *r)
{
	Ekeko_Renderable_Private *prv;

	prv = PRIVATE(r);
	return prv->appended;
}
void renderable_appended_set(Ekeko_Renderable *r, Eina_Bool appended)
{
	Ekeko_Renderable_Private *prv;

	prv = PRIVATE(r);
	prv->appended = appended;
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
Ekeko_Type *ekeko_renderable_type_get(void)
{
	static Ekeko_Type *renderable_type = NULL;

	if (!renderable_type)
	{
		renderable_type = ekeko_type_new(TYPE_NAME, sizeof(Ekeko_Renderable),
				sizeof(Ekeko_Renderable_Private), ekeko_object_type_get(),
				_ctor, _dtor, NULL);
		/* the properties */
		TYPE_PROP_DOUBLE_ADD(renderable_type, "geometry", PROPERTY_RECTANGLE,
				OFFSET(Ekeko_Renderable_Private, geometry.curr), OFFSET(Ekeko_Renderable_Private, geometry.prev),
				OFFSET(Ekeko_Renderable_Private, geometry.changed));
		TYPE_PROP_DOUBLE_ADD(renderable_type, "visibility", PROPERTY_BOOL,
				OFFSET(Ekeko_Renderable_Private, visibility.curr), OFFSET(Ekeko_Renderable_Private, visibility.prev),
				OFFSET(Ekeko_Renderable_Private, visibility.changed));
	}

	return renderable_type;
}

EAPI void ekeko_renderable_move(Ekeko_Renderable *r, int x, int y)
{
	Ekeko_Renderable_Private *prv;
	Eina_Rectangle move;
	Value value;

	/* TODO avoid this duplicate */
	prv = PRIVATE(r);
	move.x = x;
	move.y = y;
	move.w = prv->geometry.curr.w;
	move.h = prv->geometry.curr.h;
	value_rectangle_from(&value, &move);
	ekeko_object_property_value_set((Ekeko_Object *)r, "geometry", &value);
}

EAPI void ekeko_renderable_resize(Ekeko_Renderable *r, int w, int h)
{
	Ekeko_Renderable_Private *prv;
	Eina_Rectangle resize;
	Value value;

	/* TODO avoid this duplicate */
	prv = PRIVATE(r);
	resize.x = prv->geometry.curr.x;
	resize.y = prv->geometry.curr.y;
	resize.w = w;
	resize.h = h;
	value_rectangle_from(&value, &resize);
	ekeko_object_property_value_set((Ekeko_Object *)r, "geometry", &value);
}

EAPI void ekeko_renderable_geometry_set(Ekeko_Renderable *r, Eina_Rectangle *rect)
{
	Value value;

#ifdef ETK2_DEBUG
	printf("[rend] geometry_set %d %d %d %d\n", rect->x, rect->y, rect->w, rect->h);
#endif
	value_rectangle_from(&value, rect);
	ekeko_object_property_value_set((Ekeko_Object *)r, "geometry", &value);
}

EAPI void ekeko_renderable_geometry_get(Ekeko_Renderable *r, Eina_Rectangle *rect)
{
	Ekeko_Renderable_Private *prv;

	prv = PRIVATE(r);
	*rect = prv->geometry.curr;
}

EAPI void ekeko_renderable_visibility_get(Ekeko_Renderable *r, Eina_Bool *visible)
{
	Ekeko_Renderable_Private *prv;

	prv = PRIVATE(r);
#ifdef ETK2_DEBUG
	printf("[renderable %s] visibility get %d (%d %d)\n", ekeko_object_type_name_get((Ekeko_Object *)r), prv->visibility.curr, EINA_FALSE, EINA_TRUE);
#endif
	*visible = prv->visibility.curr;
}

EAPI void ekeko_renderable_show(Ekeko_Renderable *r)
{
	Ekeko_Renderable_Private *prv;
	Value value;

	prv = PRIVATE(r);
#ifdef ETK2_DEBUG
	printf("[renderable] show\n");
#endif
	if (prv->visibility.curr)
		return;
	value_bool_from(&value, EINA_TRUE);
	ekeko_object_property_value_set((Ekeko_Object *)r, "visibility", &value);
}

EAPI void ekeko_renderable_hide(Ekeko_Renderable *r)
{
	Ekeko_Renderable_Private *prv;
	Value value;

	prv = PRIVATE(r);
#ifdef ETK2_DEBUG
	printf("[renderable] hide\n");
#endif
	if (!prv->visibility.curr)
		return;
	value_bool_from(&value, EINA_FALSE);
	ekeko_object_property_value_set((Ekeko_Object *)r, "visibility", &value);
}

EAPI void ekeko_renderable_visibility_set(Ekeko_Renderable *r, Eina_Bool visible)
{
	Ekeko_Renderable_Private *prv;
	Value value;

	prv = PRIVATE(r);
	if (prv->visibility.curr == visible)
		return;
	value_bool_from(&value, visible);
#ifdef ETK2_DEBUG
	printf("[renderable %s] visibility set %d\n", ekeko_object_type_name_get((Ekeko_Object *)r), value.value.bool_value);
#endif
	ekeko_object_property_value_set((Ekeko_Object *)r, "visibility", &value);
}

EAPI Ekeko_Canvas * ekeko_renderable_canvas_get(Ekeko_Renderable *r)
{
	Ekeko_Renderable_Private *prv;

	prv = PRIVATE(r);
	return prv->canvas;
}
