/*
 * eon_fe_displace.c
 *
 *  Created on: 07-may-2009
 *      Author: jl
 */
#include "Eon.h"
#include "eon_private.h"
#define EON_FILTER_DEBUG 0
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Eon_Fe_Displace_Private *)((Eon_Fe_Displace *)(d))->private)
struct _Eon_Fe_Displace_Private
{
	Eon_Coord x;
	Eon_Coord y;
	Eon_Coord w;
	Eon_Coord h;
};

static void _ctor(void *instance)
{
	Eon_Filter_Effect *i;
	Eon_Filter_Effect_Private *prv;

	i = (Eon_Filter_Effect*) instance;
	i->private = prv = ekeko_type_instance_private_get(eon_fe_displace_type_get(), instance);
	EON_FILTER_EFFECT_X = TYPE_PROP_SINGLE_ADD(type, "x", EON_PROPERTY_COORD, OFFSET(Eon_Fe_Displace_Private, x));
	EON_FILTER_EFFECT_Y = TYPE_PROP_SINGLE_ADD(type, "y", EON_PROPERTY_COORD, OFFSET(Eon_Fe_Displace_Private, y));
	EON_FILTER_EFFECT_W = TYPE_PROP_SINGLE_ADD(type, "w", EON_PROPERTY_COORD, OFFSET(Eon_Fe_Displace_Private, w));
	EON_FILTER_EFFECT_H = TYPE_PROP_SINGLE_ADD(type, "h", EON_PROPERTY_COORD, OFFSET(Eon_Fe_Displace_Private, h));
}

static void _dtor(void *filter)
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
Ekeko_Property_Id EON_FILTER_EFFECT_X;
Ekeko_Property_Id EON_FILTER_EFFECT_Y;
Ekeko_Property_Id EON_FILTER_EFFECT_W;
Ekeko_Property_Id EON_FILTER_EFFECT_H;

EAPI Ekeko_Type *eon_fe_displace_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(EON_TYPE_FILTER_EFFECT, sizeof(Eon_Fe_Displace),
				sizeof(Eon_Fe_Displace_Private), ekeko_filter_effect_type_get(),
				_ctor, _dtor, _appendable);
	}

	return type;
}

EAPI Eon_Fe_Displace * eon_fe_displace_new(Eon_Filter_Effect *f)
{
	Eon_Fe_Displace *fe;

	fe= ekeko_type_instance_new(eon_fe_displace_type_get());
	ekeko_object_child_append((Ekeko_Object *)f, (Ekeko_Object *)fe);

	return fe;
}

