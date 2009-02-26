/*
 * etk2_rect.c
 *
 *  Created on: 04-feb-2009
 *      Author: jl
 */
#include "Etk2.h"
#include "etk2_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Etk_Rect_Private *)((Etk_Rect *)(d))->private)
struct _Etk_Rect_Private
{

};

static void _geometry_calc(const Ekeko_Object *o, Event *e, void *data)
{
	Etk_Rect *r = (Etk_Rect *)o;
	Eina_Rectangle geom;
	Etk_Coord x, y, w, h;

	etk_square_coords_get((Etk_Square *)r, &x, &y, &w, &h);
	eina_rectangle_coords_from(&geom, x.final, y.final, w.final,
			h.final);
#ifdef ETK_DEBUG
	printf("[Etk_Rect] Setting geometry of size %d %d %d %d\n",
			x.final, y.final, w.final, h.final);
#endif
	ekeko_renderable_geometry_set((Ekeko_Renderable *)r, &geom);
}

static void _render(Etk_Shape *s, Etk_Engine *func, Etk_Surface *surface, Etk_Context *context)
{
	Etk_Rect *r;
	Etk_Rect_Private *prv;
	Etk_Coord x, y, w, h;

	r = (Etk_Rect *)s;
	prv = PRIVATE(r);

	etk_square_coords_get((Etk_Square *)r, &x, &y, &w, &h);
#ifdef ETK_DEBUG
	printf("[Etk_Rect] Rendering rectangle %p into canvas %p\n", r, c);
#endif
	func->shape->rect(surface, context, x.final, y.final, w.final, h.final);
}

static void _ctor(void *instance)
{
	Etk_Rect *r;
	Etk_Rect_Private *prv;

	r = (Etk_Rect*) instance;
	r->private = prv = ekeko_type_instance_private_get(etk_rect_type_get(), instance);
	r->parent.parent.render = _render;
	ekeko_event_listener_add((Ekeko_Object *)r, ETK_SQUARE_X_CHANGED, _geometry_calc, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)r, ETK_SQUARE_Y_CHANGED, _geometry_calc, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)r, ETK_SQUARE_W_CHANGED, _geometry_calc, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)r, ETK_SQUARE_H_CHANGED, _geometry_calc, EINA_FALSE, NULL);
}

static void _dtor(void *rect)
{

}

static Eina_Bool _appendable(void *instance, void *child)
{
	if (!ekeko_type_instance_is_of(child, ETK_TYPE_ANIMATION))
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
Property_Id ETK_RECT_X;
Property_Id ETK_RECT_Y;
Property_Id ETK_RECT_W;
Property_Id ETK_RECT_H;

EAPI Ekeko_Type *etk_rect_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(ETK_TYPE_RECT, sizeof(Etk_Rect),
				sizeof(Etk_Rect_Private), etk_square_type_get(),
				_ctor, _dtor, _appendable);
	}

	return type;
}

EAPI Etk_Rect * etk_rect_new(Etk_Canvas *c)
{
	Etk_Rect *r;

	r = ekeko_type_instance_new(etk_rect_type_get());
	ekeko_object_child_append((Ekeko_Object *)c, (Ekeko_Object *)r);

	return r;
}
