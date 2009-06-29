/*
 * eon_polygon.c
 */
#include "Eon.h"
#include "eon_private.h"
#include <limits.h>
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Eon_Polygon_Private *)((Eon_Polygon *)(d))->private)
struct _Eon_Polygon_Private
{
	int xmin, xmax, ymin, ymax; /* bounding box */
	/* our list of points, an engine might want to have its own */
	Eina_List *points;
	void *engine_data;
};

typedef struct _Polygon_Coord
{
	int x, y;
} Polygon_Coord;

/*
 * Called whenever a point has been added/removed/set???
 * Re-calc the bounding box of the polygon
 */
//static void _point_appended_cb(const Ekeko_Object *o, Ekeko_Event *e, void *data)
static void _update_geometry(Eon_Polygon *p)
{
	Eon_Polygon_Private *prv = PRIVATE(p);
	Eina_Rectangle geom;

	eina_rectangle_coords_from(&geom, prv->xmin, prv->xmax - prv->xmin, prv->ymin, prv->ymax - prv->ymin);
	ekeko_renderable_geometry_set((Ekeko_Renderable *)p, &geom);
}


/*
 * When the polygon has been appended to a canvas get the engine
 * and call polygon_new(), if the polygon has points call
 * polygon_point_add()
 */
static void _appended_cb(const Ekeko_Object *o, Ekeko_Event *e, void *data)
{
	Ekeko_Object *parent;
	Eon_Polygon_Private *prv = PRIVATE(o);
#if 0
	if ((parent = ekeko_object_parent_get(o)))
	{
		Eon_Engine *func;
		Eon_Document *d;

		d = eon_canvas_document_get((Eon_Canvas *) parent);
		if (!d)
			return;
		func = eon_document_engine_get(d);
		prv->engine_data = func->shape->polygon_new();
		/* iterate over the list of points and add them to the engine */
		if (prv->points)
		{
			Eina_Iterator *it;
			Polygon_Coord coord;

			it = eina_list_iterator_new(prv->points);
			while (eina_iterator_next(it, (void **)&coord))
			{
				func->shape->polygon_point_add(prv->engine_data, coord.x, coord.y);
			}
			return;
		}
	}
#endif
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
	Eon_Polygon *p;
	Eon_Polygon_Private *prv;

	p = (Eon_Polygon *)s;
	prv = PRIVATE(p);

#ifdef EON_DEBUG
	printf("[Eon_Polygon] Rendering polygon %p into canvas %p\n", r, c);
#endif
#if 0
	func->shape->polygon_render(surface, context, prv->engine_data);
#endif
}

static void _ctor(void *instance)
{
	Eon_Polygon *p;
	Eon_Polygon_Private *prv;

	p = (Eon_Polygon*) instance;
	p->private = prv = ekeko_type_instance_private_get(eon_polygon_type_get(), instance);
	p->parent.render = _render;
	/* setup the limits */
	prv->xmin = prv->ymin = INT_MAX;
	prv->xmax = prv->ymax = INT_MIN;
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

	return p;
}

EAPI void eon_polygon_point_add(Eon_Polygon *p, int x, int y)
{
#if 0
	Eon_Polygon_Private *prv;
	Polygon_Coord *point;
	Ekeko_Object *parent;

	prv = PRIVATE(p);
	point = malloc(sizeof(Polygon_Coord));
	point->x = x;
	point->y = y;

	if (x < prv->xmin)
		prv->xmin = x;
	else if (x > prv->xmax)
		prv->xmax = x;
	if (y < prv->ymin)
		prv->xmin = y;
	else if (y > prv->ymax)
		prv->ymax = y;

	prv->points = eina_list_append(prv->points, point);
	if ((parent = ekeko_object_parent_get(p)))
	{
		Eon_Document *d;
		Eon_Engine *func;

		d = eon_canvas_document_get((Eon_Canvas *) parent);
		if (!d)
			return;
		func = eon_document_engine_get(d);
		func->shape->polygon_point_add(prv->engine_data, x, y);

		_update_geometry(p);
	}
#endif
}

EAPI void eon_polygon_clear(Eon_Polygon *p)
{
	/* TODO */
}
