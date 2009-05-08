/*
 * etk2_fe_displace.c
 *
 *  Created on: 07-may-2009
 *      Author: jl
 */
#include "Etk2.h"
#include "etk2_private.h"
#define ETK_FILTER_DEBUG 0
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Etk_Fe_Displace_Private *)((Etk_Fe_Displace *)(d))->private)
struct _Etk_Fe_Displace_Private
{
	Etk_Coord x;
	Etk_Coord y;
	Etk_Coord w;
	Etk_Coord h;
};

static void _ctor(void *instance)
{
	Etk_Filter_Effect *i;
	Etk_Filter_Effect_Private *prv;

	i = (Etk_Filter_Effect*) instance;
	i->private = prv = ekeko_type_instance_private_get(etk_fe_displace_type_get(), instance);
	ETK_FILTER_EFFECT_X = TYPE_PROP_SINGLE_ADD(type, "x", ETK_PROPERTY_COORD, OFFSET(Etk_Fe_Displace_Private, x));
	ETK_FILTER_EFFECT_Y = TYPE_PROP_SINGLE_ADD(type, "y", ETK_PROPERTY_COORD, OFFSET(Etk_Fe_Displace_Private, y));
	ETK_FILTER_EFFECT_W = TYPE_PROP_SINGLE_ADD(type, "w", ETK_PROPERTY_COORD, OFFSET(Etk_Fe_Displace_Private, w));
	ETK_FILTER_EFFECT_H = TYPE_PROP_SINGLE_ADD(type, "h", ETK_PROPERTY_COORD, OFFSET(Etk_Fe_Displace_Private, h));
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
Property_Id ETK_FILTER_EFFECT_X;
Property_Id ETK_FILTER_EFFECT_Y;
Property_Id ETK_FILTER_EFFECT_W;
Property_Id ETK_FILTER_EFFECT_H;

EAPI Ekeko_Type *etk_fe_displace_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(ETK_TYPE_FILTER_EFFECT, sizeof(Etk_Fe_Displace),
				sizeof(Etk_Fe_Displace_Private), ekeko_filter_effect_type_get(),
				_ctor, _dtor, _appendable);
	}

	return type;
}

EAPI Etk_Fe_Displace * etk_fe_displace_new(Etk_Filter_Effect *f)
{
	Etk_Fe_Displace *fe;

	fe= ekeko_type_instance_new(etk_fe_displace_type_get());
	ekeko_object_child_append((Ekeko_Object *)f, (Ekeko_Object *)fe);

	return fe;
}

