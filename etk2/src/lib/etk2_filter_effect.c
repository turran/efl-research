#include "Etk2.h"
#include "etk2_private.h"
#define ETK_FILTER_DEBUG 0
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Etk_Filter_Effect_Private *)((Etk_Filter_Effect *)(d))->private)
struct _Etk_Filter_Effect_Private
{
	Etk_Coord x;
	Etk_Coord y;
	Etk_Coord w;
	Etk_Coord h;
};

static void _ctor(void *instance)
{
	Etk_Filter *i;
	Etk_Filter_Private *prv;

	i = (Etk_Filter*) instance;
	i->private = prv = ekeko_type_instance_private_get(etk_filter_effect_type_get(), instance);
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
Property_Id ETK_FILTER_EFFECT_X;
Property_Id ETK_FILTER_EFFECT_Y;
Property_Id ETK_FILTER_EFFECT_W;
Property_Id ETK_FILTER_EFFECT_H;

EAPI Ekeko_Type *etk_filter_effect_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(ETK_TYPE_FILTER_EFFECT, sizeof(Etk_Filter_Effect),
				sizeof(Etk_Filter_Effect_Private), ekeko_object_type_get(),
				_ctor, _dtor, NULL);
		ETK_FILTER_EFFECT_X = TYPE_PROP_SINGLE_ADD(type, "x", ETK_PROPERTY_COORD, OFFSET(Etk_Filter_Effect_Private, x));
		ETK_FILTER_EFFECT_Y = TYPE_PROP_SINGLE_ADD(type, "y", ETK_PROPERTY_COORD, OFFSET(Etk_Filter_Effect_Private, y));
		ETK_FILTER_EFFECT_W = TYPE_PROP_SINGLE_ADD(type, "w", ETK_PROPERTY_COORD, OFFSET(Etk_Filter_Effect_Private, w));
		ETK_FILTER_EFFECT_H = TYPE_PROP_SINGLE_ADD(type, "h", ETK_PROPERTY_COORD, OFFSET(Etk_Filter_Effect_Private, h));
	}

	return type;
}

EAPI Etk_Filter_Effect * etk_filter_effect_new(Etk_Filter *f)
{
	Etk_Filter_Effect *fe;

	fe= ekeko_type_instance_new(etk_filter_effect_type_get());
	ekeko_object_child_append((Ekeko_Object *)f, (Ekeko_Object *)fe);

	return fe;
}
