#include "Etk2.h"
#include "etk2_private.h"

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Etk_Animation_Basic_Private *)((Etk_Animation_Basic *)(d))->private)
struct _Etk_Animation_Basic_Private
{

};

static void _animation_coord_callback(Etk_Animation *a, const char *prop,
		Ekeko_Object *o, const Etch_Data *curr, const Etch_Data *prev)
{
	Etk_Coord coord;
	Ekeko_Value v;

	/* we need to fetch it in order to know if it is relative or absolute */
	ekeko_object_property_value_get(o, prop, &v);
#if ETK_ANIMATION_DEBUG
	printf("[Etk_Animation] Animation %p %p called %d -> %d\n", a, v.value.pointer_value, prev->data.i32, curr->data.i32);
#endif
	etk_coord_set(&coord, curr->data.i32, ((Etk_Coord *)v.value.pointer_value)->type);
	etk_value_coord_from(&v, &coord);
	ekeko_object_property_value_set(o, prop, &v);
}

/* object, propname, animation */
static void _animation_color_callback(Etk_Animation *a, const char *prop,
		Ekeko_Object *o, const Etch_Data *curr, const Etch_Data *prev)
{
	Ekeko_Value v;

	etk_value_color_from(&v, curr->data.argb);
	ekeko_object_property_value_set(o, prop, &v);
}

static void _value_set(Ekeko_Value *v, Etch_Animation_Keyframe *k)
{
	if (v->type == ETK_PROPERTY_COORD)
	{
		Etk_Coord *c = v->value.pointer_value;

		etch_animation_keyframe_value_set(k, c->value);
#if !ETK_ANIMATION_DEBUG
		printf("[Etk_Animation] Setting coord to %d\n", c->value);
#endif
	}
	else if (v->type == ETK_PROPERTY_COLOR)
	{
		Etk_Color c = v->value.int_value;

		etch_animation_keyframe_value_set(k, c);
		printf("[Etk_Animation] Setting color to %08x\n", c);
	}
}

static void _callback_set(Ekeko_Value_Type vtype, Etch_Data_Type *dtype, Etk_Animation_Callback *cb)
{
	switch (vtype)
	{
		/* basic types */
		case PROPERTY_INT:
		*dtype = ETCH_INT32;
		break;

		default:
		if (vtype == ETK_PROPERTY_COORD)
		{
			*dtype = ETCH_UINT32;
			*cb = _animation_coord_callback;
		}
		else if (vtype == ETK_PROPERTY_COLOR)
		{

			*dtype = ETCH_ARGB;
			*cb = _animation_color_callback;
		}
		break;
	}
}


static void _ctor(void *instance)
{
	Etk_Animation_Basic *a;
	Etk_Animation_Basic_Private *prv;

	a = (Etk_Animation_Basic *) instance;
	a->private = prv = ekeko_type_instance_private_get(etk_animation_basic_type_get(), instance);
	/* default values */
	a->parent.callback_set = _callback_set;
	a->parent.value_set = _value_set;
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
EAPI Ekeko_Type * etk_animation_basic_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(ETK_TYPE_ANIMATION_BASIC, sizeof(Etk_Animation_Basic),
				sizeof(Etk_Animation_Basic_Private), etk_animation_type_get(),
				_ctor, _dtor, NULL);
	}

	return type;
}

EAPI Etk_Animation_Basic * etk_animation_basic_new(void)
{
	Etk_Animation_Basic *a;

	a = ekeko_type_instance_new(etk_animation_basic_type_get());

	return a;
}


