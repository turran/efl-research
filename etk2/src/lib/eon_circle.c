/*
 * eon_circle.c
 *
 *  Created on: 28-jun-2009
 *      Author: jl
 */

#include "Eon.h"
#include "eon_private.h"
#include <limits.h>
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Eon_Circle_Private *)((Eon_Circle *)(d))->private)
struct _Eon_Circle_Private
{
	Eon_Coord x;
	Eon_Coord y;
	Eon_Coord radius;
	void *engine_data;
};


/*
 * Called whenever a point has been added/removed/set???
 * Re-calc the bounding box of the polygon
 */
//static void _point_appended_cb(const Ekeko_Object *o, Ekeko_Event *e, void *data)
static void _update_geometry(Eon_Circle *p)
{
	Eon_Circle_Private *prv = PRIVATE(p);
	Eina_Rectangle geom;

	ekeko_renderable_geometry_set((Ekeko_Renderable *)p, &geom);
}

/*
 * When the polygon has been appended to a canvas get the engine
 * and call polygon_new(), if the polygon has points call
 * polygon_point_add()
 */

static void _render(Eon_Shape *s, Eon_Engine *func, Eon_Surface *surface, Eon_Context *context)
{
	Eon_Circle *p;
	Eon_Circle_Private *prv;

	p = (Eon_Circle *)s;
	prv = PRIVATE(p);

#ifdef EON_DEBUG
	printf("[Eon_Circle] Rendering polygon %p into canvas %p\n", r, c);
#endif
}

static void _ctor(void *instance)
{
	Eon_Circle *p;
	Eon_Circle_Private *prv;

	p = (Eon_Circle*) instance;
	p->private = prv = ekeko_type_instance_private_get(eon_polygon_type_get(), instance);
	p->parent.render = _render;
	/* setup the limits */
	ekeko_event_listener_add((Ekeko_Object *)p, EVENT_OBJECT_APPEND, _appended_cb, EINA_FALSE, NULL);
}

static void _dtor(void *polygon)
{

}

static Eina_Bool _appendable(void *instance, void *child)
{
	if (!ekeko_type_instance_is_of(child, EON_TYPE_ANIMATION))
	{
		return EINA_FALSE;
	}
	return EINA_TRUE;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
EAPI Ekeko_Type *eon_circle_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(EON_TYPE_CIRCLE, sizeof(Eon_Circle),
				sizeof(Eon_Circle_Private), eon_shape_type_get(),
				_ctor, _dtor, _appendable);
	}

	return type;
}

EAPI Eon_Circle * eon_circle_new(Eon_Canvas *c)
{
	Eon_Circle *p;

	p = ekeko_type_instance_new(eon_polygon_type_get());
	ekeko_object_child_append((Ekeko_Object *)c, (Ekeko_Object *)p);

	return p;
}

EAPI void eon_circle_x_rel_set(Eon_Square *r, int x);
{

}
EAPI void eon_circle_x_set(Eon_Square *r, int x)
{

}
EAPI void eon_circle_y_set(Eon_Square *r, int y)
{

}
EAPI void eon_circle_y_rel_set(Eon_Square *r, int y)
{

}
EAPI void eon_circle_radius_set(Eon_Square *r, int r)
{

}
EAPI void eon_circle_radius_rel_set(Eon_Square *r, int r)
{

}
