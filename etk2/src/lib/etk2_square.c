/*
 * etk2_square_shape.c
 *
 *  Created on: 26-feb-2009
 *      Author: jl
 */
#include "Etk2.h"
#include "etk2_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Etk_Square_Private *)((Etk_Square *)(d))->private)
#define ETK_TYPE_SQUARE "Etk_Square"
struct _Etk_Square_Private
{
	Etk_Coord x, y, w, h;
};

/* Just informs that the x.final property has to be recalculated */
static void _x_inform(const Ekeko_Object *o, Event *e, void *data)
{
	Etk_Square_Private *prv = PRIVATE(data);
	Ekeko_Value v;

	printf("[Etk_Square] Informing X change\n");
	etk_value_coord_from(&v, &prv->x);
	ekeko_object_property_value_set((Ekeko_Object *)data, "x", &v);
}

/* Just informs that the y.final property has to be recalculated */
static void _y_inform(const Ekeko_Object *o, Event *e, void *data)
{
	Etk_Square_Private *prv = PRIVATE(data);
	Ekeko_Value v;

	printf("[Etk_Square] Informing Y change\n");
	etk_value_coord_from(&v, &prv->y);
	ekeko_object_property_value_set((Ekeko_Object *)data, "y", &v);
}

/* Just informs that the w.final property has to be recalculated */
static void _w_inform(const Ekeko_Object *o, Event *e, void *data)
{
	Etk_Square_Private *prv = PRIVATE(data);
	Ekeko_Value v;

	printf("[Etk_Square] Informing W change\n");
	etk_value_coord_from(&v, &prv->w);
	ekeko_object_property_value_set((Ekeko_Object *)data, "w", &v);
}

/* Just informs that the h.final property has to be recalculated */
static void _h_inform(const Ekeko_Object *o, Event *e, void *data)
{
	Etk_Square_Private *prv = PRIVATE(data);
	Ekeko_Value v;

	printf("[Etk_Square] Informing H change\n");
	etk_value_coord_from(&v, &prv->h);
	ekeko_object_property_value_set((Ekeko_Object *)data, "h", &v);
}

/* Called whenever the x property changes */
static void _x_change(const Ekeko_Object *o, Event *e, void *data)
{
	Event_Mutation *em = (Event_Mutation *)e;
	Etk_Square *s = (Etk_Square *)o;
	Etk_Square_Private *prv = PRIVATE(o);
	Ekeko_Object *parent;
	Etk_Coord x, w;

	if (em->state == EVENT_MUTATION_STATE_POST)
		return;

	if (!(parent = ekeko_object_parent_get(o)))
		return;

	etk_canvas_x_get((Etk_Canvas *)parent, &x);
	etk_canvas_w_get((Etk_Canvas *)parent, &w);
	etk_coord_change(o, &prv->x, em->curr->value.pointer_value,
			em->prev->value.pointer_value, x.final, w.final, parent,
			ETK_CANVAS_X_CHANGED, ETK_CANVAS_W_CHANGED,
			_x_inform);
}

/* Called whenever the y property changes */
static void _y_change(const Ekeko_Object *o, Event *e, void *data)
{
	Event_Mutation *em = (Event_Mutation *)e;
	Etk_Square *s = (Etk_Square *)o;
	Etk_Square_Private *prv = PRIVATE(o);
	Ekeko_Object *parent;
	Etk_Coord y, h;

	if (em->state == EVENT_MUTATION_STATE_POST)
		return;

	if (!(parent = ekeko_object_parent_get(o)))
		return;

	etk_canvas_y_get((Etk_Canvas *)parent, &y);
	etk_canvas_h_get((Etk_Canvas *)parent, &h);
	etk_coord_change(o, &prv->y, em->curr->value.pointer_value,
			em->prev->value.pointer_value, y.final, h.final, parent,
			ETK_CANVAS_Y_CHANGED, ETK_CANVAS_H_CHANGED,
			_y_inform);
}

/* Called whenever the w property changes */
static void _w_change(const Ekeko_Object *o, Event *e, void *data)
{
	Event_Mutation *em = (Event_Mutation *)e;
	Etk_Square *s = (Etk_Square *)o;
	Etk_Square_Private *prv = PRIVATE(o);
	Ekeko_Object *parent;
	Etk_Coord w;

	if (em->state == EVENT_MUTATION_STATE_POST)
		return;

	if (!(parent = ekeko_object_parent_get(o)))
		return;

	etk_canvas_w_get((Etk_Canvas *)parent, &w);
	etk_coord_length_change(o, &prv->w, em->curr->value.pointer_value,
			em->prev->value.pointer_value, w.final, parent,
			ETK_CANVAS_W_CHANGED, _w_inform);
}

/* Called whenever the w property changes */
static void _h_change(const Ekeko_Object *o, Event *e, void *data)
{
	Event_Mutation *em = (Event_Mutation *)e;
	Etk_Square *s = (Etk_Square *)o;
	Etk_Square_Private *prv = PRIVATE(o);
	Ekeko_Object *parent;
	Etk_Coord h;

	if (em->state == EVENT_MUTATION_STATE_POST)
		return;

	if (!(parent = ekeko_object_parent_get(o)))
		return;

	etk_canvas_h_get((Etk_Canvas *)parent, &h);
	etk_coord_length_change(o, &prv->h, em->curr->value.pointer_value,
			em->prev->value.pointer_value, h.final, parent,
			ETK_CANVAS_H_CHANGED, _h_inform);
}


static void _ctor(void *instance)
{
	Etk_Square *s;
	Etk_Square_Private *prv;

	s = (Etk_Square*) instance;
	s->private = prv = ekeko_type_instance_private_get(etk_square_type_get(), instance);
	ekeko_event_listener_add((Ekeko_Object *)s, ETK_SQUARE_X_CHANGED, _x_change, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)s, ETK_SQUARE_Y_CHANGED, _y_change, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)s, ETK_SQUARE_W_CHANGED, _w_change, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)s, ETK_SQUARE_H_CHANGED, _h_change, EINA_FALSE, NULL);
}

static void _dtor(void *rect)
{

}

/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/

/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
Property_Id ETK_SQUARE_X;
Property_Id ETK_SQUARE_Y;
Property_Id ETK_SQUARE_W;
Property_Id ETK_SQUARE_H;

EAPI Ekeko_Type *etk_square_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(ETK_TYPE_SQUARE, sizeof(Etk_Square),
				sizeof(Etk_Square_Private), etk_shape_type_get(),
				_ctor, _dtor, NULL);
		ETK_SQUARE_X = TYPE_PROP_SINGLE_ADD(type, "x", ETK_PROPERTY_COORD, OFFSET(Etk_Square_Private, x));
		ETK_SQUARE_Y = TYPE_PROP_SINGLE_ADD(type, "y", ETK_PROPERTY_COORD, OFFSET(Etk_Square_Private, y));
		ETK_SQUARE_W = TYPE_PROP_SINGLE_ADD(type, "w", ETK_PROPERTY_COORD, OFFSET(Etk_Square_Private, w));
		ETK_SQUARE_H = TYPE_PROP_SINGLE_ADD(type, "h", ETK_PROPERTY_COORD, OFFSET(Etk_Square_Private, h));
	}

	return type;
}

EAPI void etk_square_x_rel_set(Etk_Square *s, int x)
{
	Etk_Coord coord;
	Ekeko_Value v;

	etk_coord_set(&coord, x, ETK_COORD_RELATIVE);
	etk_value_coord_from(&v, &coord);
	ekeko_object_property_value_set((Ekeko_Object *)s, "x", &v);
}

EAPI void etk_square_x_set(Etk_Square *s, int x)
{
	Etk_Coord coord;
	Ekeko_Value v;

	etk_coord_set(&coord, x, ETK_COORD_ABSOLUTE);
	etk_value_coord_from(&v, &coord);
	ekeko_object_property_value_set((Ekeko_Object *)s, "x", &v);
}

EAPI void etk_square_y_set(Etk_Square *s, int y)
{
	Etk_Coord coord;
	Ekeko_Value v;

	etk_coord_set(&coord, y, ETK_COORD_ABSOLUTE);
	etk_value_coord_from(&v, &coord);
	ekeko_object_property_value_set((Ekeko_Object *)s, "y", &v);
}

EAPI void etk_square_y_rel_set(Etk_Square *s, int y)
{
	Etk_Coord coord;
	Ekeko_Value v;

	etk_coord_set(&coord, y, ETK_COORD_RELATIVE);
	etk_value_coord_from(&v, &coord);
	ekeko_object_property_value_set((Ekeko_Object *)s, "y", &v);
}

EAPI void etk_square_w_set(Etk_Square *s, int w)
{
	Etk_Coord coord;
	Ekeko_Value v;

	etk_coord_set(&coord, w, ETK_COORD_ABSOLUTE);
	etk_value_coord_from(&v, &coord);
	ekeko_object_property_value_set((Ekeko_Object *)s, "w", &v);
}

EAPI void etk_square_w_rel_set(Etk_Square *s, int w)
{
	Etk_Coord coord;
	Ekeko_Value v;

	etk_coord_set(&coord, w, ETK_COORD_RELATIVE);
	etk_value_coord_from(&v, &coord);
	ekeko_object_property_value_set((Ekeko_Object *)s, "w", &v);
}

EAPI void etk_square_h_set(Etk_Square *s, int h)
{
	Etk_Coord coord;
	Ekeko_Value v;

	etk_coord_set(&coord, h, ETK_COORD_ABSOLUTE);
	etk_value_coord_from(&v, &coord);
	ekeko_object_property_value_set((Ekeko_Object *)s, "h", &v);
}

EAPI void etk_square_h_rel_set(Etk_Square *s, int h)
{
	Etk_Coord coord;
	Ekeko_Value v;

	etk_coord_set(&coord, h, ETK_COORD_RELATIVE);
	etk_value_coord_from(&v, &coord);
	ekeko_object_property_value_set((Ekeko_Object *)s, "h", &v);
}

EAPI void etk_square_coords_get(Etk_Square *s, Etk_Coord *x, Etk_Coord *y, Etk_Coord *w, Etk_Coord *h)
{
	Etk_Square_Private *prv = PRIVATE(s);

	*x = prv->x;
	*y = prv->y;
	*w = prv->w;
	*h = prv->h;
}
