#include "Etk2.h"
#include "etk2_private.h"

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(d) ((Etk_Animation_Matrix_Private *)((Etk_Animation_Matrix *)(d))->private)
struct _Etk_Animation_Matrix_Private
{
	int type;
};

static void _animation_matrix_callback(Etk_Animation *a, const char *prop,
		Ekeko_Object *o, const Etch_Data *curr, const Etch_Data *prev)
{
	Etk_Animation_Matrix *am = (Etk_Animation_Matrix *)a;
	Etk_Animation_Matrix_Private *prv = PRIVATE(am);
	Enesim_Matrix m;
	Ekeko_Value v;

	if (prv->type == ETK_ROTATE)
	{
			enesim_matrix_rotate(&m, curr->data.u32);
	}
	else if (prv->type == ETK_SCALE)
	{
		enesim_matrix_scale(&m, curr->data.u32, curr->data.u32);
	}
	/* get the matrix type from the animation */
	/* create a new one */
	/* set the matrix on the object */
	printf("CAAAAAAAAAAAAALLLLLLLLLLLLLLLLLLLEEEEEEEEEEEEEEEEEEDDDDDDDDDDDD %d\n", curr->data.u32);
	etk_value_matrix_from(&v, &m);
	ekeko_object_property_value_set(o, prop, &v);
}

static void _value_set(Ekeko_Value *v, Etch_Animation_Keyframe *k)
{
	if (v->type == ETCH_UINT32)
	{
		printf("CAAAAAAAAAAAAALLLLLLLLLLLLLLLLLLLEEEEEEEEEEEEEEEEEEDDDDDDDDDDDD with value %d %d\n", v->type, v->value.int_value);
#if 0
		Enesim_Matrix *m = v->value.pointer_value;
		printf("[Etk_Animation] Setting matrix to:\n");
		printf("[%g %g %g]\n[%g %g %g]\n[%g %g %g]\n", m->xx, m->xy, m->xz, m->yx, m->yy, m->yz, m->zx, m->zy, m->zz);
#endif
		etch_animation_keyframe_value_set(k, v->value.int_value);
	}
}

static void _callback_set(Ekeko_Value_Type vtype, Etch_Data_Type *dtype, Etk_Animation_Callback *cb)
{
	if (vtype == ETK_PROPERTY_MATRIX)
	{
		*dtype = ETCH_UINT32;
		*cb = _animation_matrix_callback;
	}
}

static void _ctor(void *instance)
{
	Etk_Animation_Basic *a;
	Etk_Animation_Basic_Private *prv;

	a = (Etk_Animation_Basic *) instance;
	a->private = prv = ekeko_type_instance_private_get(etk_animation_matrix_type_get(), instance);
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
Property_Id ETK_ANIMATION_MATRIX_TYPE;

EAPI Ekeko_Type *etk_animation_matrix_type_get(void)
{
	static Ekeko_Type *type = NULL;

	if (!type)
	{
		type = ekeko_type_new(ETK_TYPE_ANIMATION_MATRIX, sizeof(Etk_Animation_Matrix),
				sizeof(Etk_Animation_Matrix_Private), etk_animation_type_get(),
				_ctor, _dtor, NULL);
		ETK_ANIMATION_MATRIX_TYPE = TYPE_PROP_SINGLE_ADD(type, "type", PROPERTY_INT, OFFSET(Etk_Animation_Matrix_Private, type));
	}
	return type;
}


EAPI Etk_Animation_Matrix * etk_animation_matrix_new(void)
{
	Etk_Animation_Matrix *a;

	printf("ANIMATION MATRIX NEWWWWWWWWWW\n");
	a = ekeko_type_instance_new(etk_animation_matrix_type_get());

	return a;
}

