/*
 * etk2_filter.c
 *
 *  Created on: 04-feb-2009
 *      Author: jl
 */
#include "Etk2.h"
#include "etk2_private.h"
#define ETK_FILTER_DEBUG 0
/* TODO
 * + create a temporary filter when the size has changed of size of the filter itself
 * with some pattern in to inform that the filter is loading on the background
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Etk_Filter_Private *)((Etk_Filter *)(d))->private)
struct _Etk_Filter_Private
{
	float radius;
	float x;
	float y;
};

static void _ctor(void *instance)
{
	Etk_Filter *i;
	Etk_Filter_Private *prv;

	i = (Etk_Filter*) instance;
	i->private = prv = ekeko_type_instance_private_get(etk_filter_type_get(), instance);
	i->parent.parent.render = _render;
	ekeko_event_listener_add((Ekeko_Object *)i, ETK_SQUARE_X_CHANGED, _geometry_calc, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)i, ETK_SQUARE_Y_CHANGED, _geometry_calc, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)i, ETK_SQUARE_W_CHANGED, _geometry_calc, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)i, ETK_SQUARE_H_CHANGED, _geometry_calc, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)i, ETK_FILTER_FILE_CHANGED, _file_change, EINA_FALSE, NULL);
	ekeko_event_listener_add((Ekeko_Object *)i, ETK_FILTER_MATRIX_CHANGED, _matrix_change, EINA_FALSE, NULL);
}

static void _dtor(void *filter)
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
Property_Id ETK_FILTER_X;
Property_Id ETK_FILTER_Y;
Property_Id ETK_FILTER_W;
Property_Id ETK_FILTER_H;

EAPI Ekeko_Type *etk_filter_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(ETK_TYPE_FILTER, sizeof(Etk_Filter),
				sizeof(Etk_Filter_Private), etk_square_type_get(),
				_ctor, _dtor, _appendable);
		ETK_FILTER_RADIUS = TYPE_PROP_SINGLE_ADD(type, "radius", EKEKO_VALUE_FLOAT, OFFSET(Etk_Filter_Private, radius));
	}

	return type;
}

EAPI Etk_Filter * etk_filter_new(Etk_Filter *f)
{
	Etk_Filter *r;

	r = ekeko_type_instance_new(etk_filter_type_get());
	ekeko_object_child_append((Ekeko_Object *)c, (Ekeko_Object *)r);

	return r;
}
