#include "Ekeko.h"
#include "ekeko_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static int _value_impls = 1;
Eina_Array *_values;

struct _Ekeko_Value
{
	Ekeko_Value_Type type;
	union {
		int i;
		Eina_Bool b;
		char c;
		float f;
		double d;
		short s;
		long l;
		void *p;
		char *st;
		Eina_Rectangle r;
	} v;
};
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void ekeko_value_init(void)
{
	_values = eina_array_new(10);
}
void ekeko_value_shutdown(void)
{
	/* FIXME */
}

Eina_Bool ekeko_value_int_cmp(int a, int b)
{
	if (a != b)
		return EINA_TRUE;
	return EINA_FALSE;
}
Eina_Bool ekeko_value_rectangle_cmp(Eina_Rectangle *a, Eina_Rectangle *b)
{
	
}

Eina_Bool ekeko_value_string_cmp(const char *a, const char *b)
{
	if (!strcmp(a, b))
		return EINA_FALSE;
	return EINA_TRUE;
}

Ekeko_Value_Class * ekeko_value_class_get(Ekeko_Value_Type t)
{
	return eina_array_data_get(_values, t - EKEKO_ATTRIBUTE_LAST);
}

void ekeko_value_int_set(Ekeko_Value *v, int i)
{
#ifdef DEBUG
	if (v->type != EKEKO_ATTRIBUTE_INT)
	{
		/* TODO eina_error */
		return;
	}
#endif
	v->v.i = i;
}

void ekeko_value_set(Ekeko_Value *v, Ekeko_Value_Type type, void *d)
{
	switch(type)
	{
	case EKEKO_ATTRIBUTE_INT:
		v->v.i = (int)d;
		break;
	default:
		break;
	}
}
Ekeko_Value * ekeko_value_new(Ekeko_Value_Type type, void *d)
{
	Ekeko_Value *v;
	
	v = malloc(sizeof(Ekeko_Value));
	v->type = type;
	ekeko_value_set(v, type, d);
}
void ekeko_value_delete(Ekeko_Value *v)
{
	if (v->type == EKEKO_ATTRIBUTE_STRING)
	{
		if (v->v.st) free(v->v.st);
	}	
	free(v);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * Register a new value type
 * the compare function to determine if it has changed
 */
EAPI int ekeko_value_register(Ekeko_Value_Class *vclass)
{
	Ekeko_Value_Class *impl;
	
	impl = malloc(sizeof(Ekeko_Value_Class));
	memcpy(impl, vclass, sizeof(Ekeko_Value_Class));
	
	eina_array_push(_values, impl);
	return _value_impls + EKEKO_ATTRIBUTE_LAST;
}
/**
 * 
 */
EAPI Ekeko_Value_Type ekeko_value_type_get(Ekeko_Value *v)
{
	return v->type;
}
/**
 * 
 */
EAPI void ekeko_value_bool_from(Ekeko_Value *v, Eina_Bool b)
{
	v->type = EKEKO_ATTRIBUTE_BOOL;
	v->v.b = b;
}
/**
 * 
 */
EAPI void ekeko_value_int_from(Ekeko_Value *v, int i)
{
	v->type = EKEKO_ATTRIBUTE_INT;
	v->v.i = i;
}
/**
 * 
 */
EAPI void ekeko_value_rectangle_from(Ekeko_Value *v, Eina_Rectangle *r)
{
	v->type = EKEKO_ATTRIBUTE_RECTANGLE;
	v->v.r = *r;
}
/**
 * 
 */
EAPI void ekeko_value_rectangle_coords_from(Ekeko_Value *v, int x, int y, int w, int h)
{
	v->type = EKEKO_ATTRIBUTE_RECTANGLE;
	eina_rectangle_coords_from(&v->v.r, x, y, w, h);
}
/**
 * 
 */
void ekeko_value_string_from(Ekeko_Value *v, const char *string)
{
	v->type = EKEKO_ATTRIBUTE_STRING;
	v->v.st = strdup(string);
}
/**
 * 
 * 
 */
void ekeko_value_external_from(Ekeko_Value *v, Ekeko_Value_Type t, void *ext)
{
	Ekeko_Value_Class *vclass;

	vclass = ekeko_value_class_get(t);
	
}
/**
 * 
 * 
 */
EAPI ekeko_value_get(Ekeko_Value *v, void *d)
{
	switch (v->type)
	{
	case EKEKO_ATTRIBUTE_INT:
		*(int *)d = v->v.i;
		break;
	default:
		break;
	}
}
/**
 * 
 */
EAPI int ekeko_value_int_get(Ekeko_Value *v)
{
#ifdef DEBUG
	if (v->type != EKEKO_ATTRIBUTE_INT)
	{
		/* TODO eina_error */
		return 0;
	}
#endif
	return v->v.i;
}

EAPI void * ekeko_value_external_get(Ekeko_Value *v)
{
	return v->v.p;
}

EAPI void ekeko_value_rectangle_get(Ekeko_Value *v, Eina_Rectangle *r)
{
	*r = v->v.r;
}
