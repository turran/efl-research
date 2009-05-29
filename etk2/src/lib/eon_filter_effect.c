#include "Eon.h"
#include "eon_private.h"
#define EON_FILTER_DEBUG 0
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Eon_Filter_Effect_Private *)((Eon_Filter_Effect *)(d))->private)
struct _Eon_Filter_Effect_Private
{
	Eon_Coord x;
	Eon_Coord y;
	Eon_Coord w;
	Eon_Coord h;
};

static void _ctor(void *instance)
{
	Eon_Filter *i;
	Eon_Filter_Private *prv;

	i = (Eon_Filter*) instance;
	i->private = prv = ekeko_type_instance_private_get(eon_filter_effect_type_get(), instance);
}

static void _dtor(void *filter)
{

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

EAPI Ekeko_Type *eon_filter_effect_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(EON_TYPE_FILTER_EFFECT, sizeof(Eon_Filter_Effect),
				sizeof(Eon_Filter_Effect_Private), ekeko_object_type_get(),
				_ctor, _dtor, NULL);
		EON_FILTER_EFFECT_X = TYPE_PROP_SINGLE_ADD(type, "x", EON_PROPERTY_COORD, OFFSET(Eon_Filter_Effect_Private, x));
		EON_FILTER_EFFECT_Y = TYPE_PROP_SINGLE_ADD(type, "y", EON_PROPERTY_COORD, OFFSET(Eon_Filter_Effect_Private, y));
		EON_FILTER_EFFECT_W = TYPE_PROP_SINGLE_ADD(type, "w", EON_PROPERTY_COORD, OFFSET(Eon_Filter_Effect_Private, w));
		EON_FILTER_EFFECT_H = TYPE_PROP_SINGLE_ADD(type, "h", EON_PROPERTY_COORD, OFFSET(Eon_Filter_Effect_Private, h));
	}

	return type;
}

EAPI Eon_Filter_Effect * eon_filter_effect_new(Eon_Filter *f)
{
	Eon_Filter_Effect *fe;

	fe= ekeko_type_instance_new(eon_filter_effect_type_get());
	ekeko_object_child_append((Ekeko_Object *)f, (Ekeko_Object *)fe);

	return fe;
}
