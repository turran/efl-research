/*
 * etk2_value.c
 *
 *  Created on: 05-ene-2009
 *      Author: jl
 */
#include "Ekeko.h"
#include "ekeko_private.h"

/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Eina_Hash *_values = NULL;

typedef struct _Ekeko_Value_Impl
{
	const char *name;
	Ekeko_Value_Set set;
	Ekeko_Value_Pointer_Set pset;
	Ekeko_Value_Compare cmp;
} Ekeko_Value_Impl;

static inline Ekeko_Value_Impl * _implementation_get(int id)
{
	Ekeko_Value_Impl *impl;

	impl = eina_hash_find(_values, &id);
	return impl;
}
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void ekeko_value_pointer_double_set(Value *value, Value_Type type, void *ptr,
		void *prev, char *changed)
{
	switch (type)
	{
		case PROPERTY_INT:
		*((int *)ptr) = value->value.int_value;
		if (*((int *)ptr) != *((int *)prev))
			*changed = EINA_TRUE;
		break;

		case PROPERTY_STRING:
		*((char **)ptr) = strdup(value->value.string_value);
		if (!strcmp(*((char **)ptr), *((char **)prev)))
			*changed = EINA_TRUE;
		break;

		case PROPERTY_RECTANGLE:

		{
			Eina_Rectangle *c = (Eina_Rectangle *)ptr;
			Eina_Rectangle *p;

			*c = value->value.rect;
			p = (Eina_Rectangle *)prev;
			if ((c->x != p->x) || (c->y != p->y) || (c->w != p->w) || (c->h != p->h))
				*changed = EINA_TRUE;
		}
		break;

		case PROPERTY_BOOL:
		*((Eina_Bool *)ptr) = value->value.bool_value;
		if (*((Eina_Bool *)ptr) != *((Eina_Bool *)prev))
			*changed = EINA_TRUE;
		break;

		default:
		printf("POINTER DOUBLE VALUE SET UNDEFINED VALUE\n");
		{
			Ekeko_Value_Impl *impl;

			impl = _implementation_get(type);
			impl->pset(value, ptr);
			if (impl->cmp(ptr, prev))
				*changed = EINA_TRUE;
			break;
		}
	}
}

void ekeko_value_pointer_set(Value *value, Value_Type vtype, void *ptr)
{
	switch (vtype)
	{
		case PROPERTY_INT:
		*((int *)ptr) = value->value.int_value;
		break;

		case PROPERTY_STRING:
		*((char **)ptr) = strdup(value->value.string_value);
		break;

		case PROPERTY_RECTANGLE:
		*((Eina_Rectangle *)ptr) = value->value.rect;
		break;

		case PROPERTY_BOOL:
		*((Eina_Bool *)ptr) = value->value.bool_value;
		break;

		default:
		printf("POINTER DOUBLE VALUE SET UNDEFINED VALUE\n");
		{
			Ekeko_Value_Impl *impl;

			impl = _implementation_get(vtype);
			impl->pset(value, ptr);
		}
		break;
	}
}

void ekeko_value_init(void)
{
	_values = eina_hash_int32_new(NULL);
}

void ekeko_value_shutdown(void)
{
	/* TODO */
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
int ekeko_value_register(const char *name, Ekeko_Value_Set set,
		Ekeko_Value_Pointer_Set pset, Ekeko_Value_Compare cmp)
{
	static int _curr = PROPERTY_STRING; // the last internal property
	Ekeko_Value_Impl *impl;

	impl = malloc(sizeof(Ekeko_Value_Impl));
	impl->name = strdup(name);
	impl->cmp = cmp;
	impl->pset = pset;
	impl->set = set;

	++_curr;
	eina_hash_add(_values, &_curr, impl);
	return _curr;
}

void ekeko_value_set(Value *v, Value_Type vtype, void *val)
{
	switch (vtype)
	{
		case PROPERTY_STRING:
		v->value.string_value = *(char **)val;
		break;

		case PROPERTY_INT:
		v->value.int_value = *(int *)val;
		break;

		case PROPERTY_RECTANGLE:
		v->value.rect = *(Eina_Rectangle *)val;
		break;

		case PROPERTY_BOOL:
		v->value.bool_value = *(Eina_Bool *)val;
		break;

		default:
		printf("VALUE SET UNDEFINED VALUE\n");
		{
			Ekeko_Value_Impl *impl;

			impl = _implementation_get(vtype);
			impl->set(v, val);
		}
		break;
	}
}
