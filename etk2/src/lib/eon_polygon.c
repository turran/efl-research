/*
 * eon_polygon.c
 */
#include "Eon.h"
#include "eon_private.h"
#if 0
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Eon_Polygon_Private *)((Eon_Polygon *)(d))->private)
struct _Eon_Polygon_Private
{
	void *engine_data;
};

/* 
 * When the polygon has been appended to a canvas get the engine
 * and call polygon_new(), if the polygon has points call
 * polygon_point_add()
 */
static void _appended_cb(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{

}

/*
 * Called whenever a point has been added/removed/set???
 * Re-calc the bounding box of the polygon
 */
static void _point_appended_cb(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{

}

/*
 * When the quality of the shape has changed, check if we have a polygon
 * if so, delete it, create a new one and re-append the points
 */
static void _quality_cb(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{

}

static void _render(Eon_Shape *s, Eon_Engine *func, Eon_Surface *surface, Eon_Context *context)
{
	Eon_Polygon *r;
	Eon_Polygon_Private *prv;
	Eon_Coord x, y, w, h;

	r = (Eon_Polygon *)s;
	prv = PRIVATE(r);

	eon_square_coords_get((Eon_Square *)r, &x, &y, &w, &h);
#ifdef EON_DEBUG
	printf("[Eon_Polygon] Rendering polygonangle %p into canvas %p\n", r, c);
#endif
	func->shape->polygon(surface, context, x.final, y.final, w.final, h.final);
}

static void _ctor(void *instance)
{
	Eon_Polygon *r;
	Eon_Polygon_Private *prv;

	r = (Eon_Polygon*) instance;
	r->private = prv = ekeko_type_instance_private_get(eon_polygon_type_get(), instance);
	r->parent.parent.render = _render;
	ekeko_event_listener_add((Ekeko_Object *)r, EON_SQUARE_X_CHANGED, _geometry_calc, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)r, EON_SQUARE_Y_CHANGED, _geometry_calc, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)r, EON_SQUARE_W_CHANGED, _geometry_calc, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)r, EON_SQUARE_H_CHANGED, _geometry_calc, EINA_FALSE, NULL);
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
EAPI Ekeko_Type *eon_polygon_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(EON_TYPE_POLYGON, sizeof(Eon_Polygon),
				sizeof(Eon_Polygon_Private), eon_shape_type_get(),
				_ctor, _dtor, _appendable);
	}

	return type;
}

EAPI Eon_Polygon * eon_polygon_new(Eon_Canvas *c)
{
	Eon_Polygon *p;

	p = ekeko_type_instance_new(eon_polygon_type_get());
	ekeko_object_child_append((Ekeko_Object *)c, (Ekeko_Object *)p);

	return r;
}

EAPI void eon_polygon_point_add(Eon_Polygon *p, Eon_Coord c)
{
	Eon_Polygon_Private *pp;

	pp = PRIVATE(p);
	/* call the engine polygon_new */
	/* call the engine polygon_point_add */
}

#endif
