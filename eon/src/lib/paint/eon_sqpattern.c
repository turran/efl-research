#include "Eon.h"
#include "eon_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define EON_IMAGE_DEBUG 0
#define PRIVATE(d) ((Eon_Sqpattern_Private *)((Eon_Sqpattern *)(d))->private)

struct _Eon_Sqpattern_Private
{
	Eon_Coord w, h;
	Eon_Color color1, color2;
};

static void _ctor(void *instance)
{
	Eon_Sqpattern *sq;
	Eon_Sqpattern_Private *prv;

	sq = (Eon_Sqpattern *)instance;
	sq->private = prv = ekeko_type_instance_private_get(eon_sqpattern_type_get(), instance);
	sq->parent.create = eon_engine_sqpattern_create;
	sq->parent.setup = eon_engine_sqpattern_setup;
	sq->parent.delete = eon_engine_sqpattern_delete;
}

static void _dtor(void *image)
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
Ekeko_Property_Id EON_SQPATTERN_SW;
Ekeko_Property_Id EON_SQPATTERN_SH;
Ekeko_Property_Id EON_SQPATTERN_COLOR1;
Ekeko_Property_Id EON_SQPATTERN_COLOR2;

EAPI Ekeko_Type *eon_sqpattern_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(EON_TYPE_SQPATTERN, sizeof(Eon_Sqpattern),
				sizeof(Eon_Sqpattern_Private), eon_paint_type_get(),
				_ctor, _dtor, _appendable);
		EON_SQPATTERN_COLOR1 = TYPE_PROP_SINGLE_ADD(type, "color1", EON_PROPERTY_COLOR, OFFSET(Eon_Sqpattern_Private, color1));
		EON_SQPATTERN_COLOR2 = TYPE_PROP_SINGLE_ADD(type, "color2", EON_PROPERTY_COLOR, OFFSET(Eon_Sqpattern_Private, color2));
		EON_SQPATTERN_SW = TYPE_PROP_SINGLE_ADD(type, "sw", EON_PROPERTY_COORD, OFFSET(Eon_Sqpattern_Private, w));
		EON_SQPATTERN_SH = TYPE_PROP_SINGLE_ADD(type, "sh", EON_PROPERTY_COORD, OFFSET(Eon_Sqpattern_Private, h));
	}

	return type;
}

EAPI Eon_Sqpattern * eon_sqpattern_new(void)
{
	Eon_Sqpattern *sq;

	sq = ekeko_type_instance_new(eon_sqpattern_type_get());

	return sq;
}

EAPI Eon_Color eon_sqpattern_color1_get(Eon_Sqpattern *sq)
{
	Eon_Sqpattern_Private *prv = PRIVATE(sq);

	return prv->color1;
}

EAPI Eon_Color eon_sqpattern_color2_get(Eon_Sqpattern *sq)
{
	Eon_Sqpattern_Private *prv = PRIVATE(sq);

	return prv->color2;
}

EAPI void eon_sqpattern_color1_set(Eon_Sqpattern *sq, Eon_Color color)
{
	Ekeko_Value v;

	eon_value_color_from(&v, color);
	ekeko_object_property_value_set((Ekeko_Object *)sq, "color1", &v);
}

EAPI void eon_sqpattern_color2_set(Eon_Sqpattern *sq, Eon_Color color)
{
	Ekeko_Value v;

	eon_value_color_from(&v, color);
	ekeko_object_property_value_set((Ekeko_Object *)sq, "color2", &v);
}
