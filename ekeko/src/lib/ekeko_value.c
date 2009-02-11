/*
 * etk2_value.c
 *
 *  Created on: 05-ene-2009
 *      Author: jl
 */
#include "Ekeko.h"
#include "ekeko_private.h"
/* FIXME rename the callbacks, get/set isnt enough to describe what they do
 * FIXME check the PROPERTY_VALUE
 * pointer_from -> double: dont allocate just copy the vars, pointers or whatever
 *                 single: same
 * create -> double: dont called
 *           single: alloc the pointer malloc(ssize_t impl->size)
 * pointer_to -> strdup for string
 * free -> free the malloced area and free the char * or any other pointer
 */
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
static Eina_Hash *_values = NULL;

typedef struct _Ekeko_Value_Impl
{
	const char *name;
	Ekeko_Value_Create create;
	Ekeko_Value_Compare cmp;
	Ekeko_Value_Free free;
	Ekeko_Value_Pointer_From pointer_from;
	Ekeko_Value_Pointer_From pointer_to;
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
void ekeko_value_create(Ekeko_Value *value, Ekeko_Value_Type type)
{
	switch (type)
	{
		case PROPERTY_UNDEFINED:
		printf("[Ekeko_Value] creating an undefined value?\n");
		//exit(1);
		break;

		/* there's no need to allocate anything for this property types */
		case PROPERTY_INT:
		case PROPERTY_BOOL:
		case PROPERTY_CHAR:
		case PROPERTY_FLOAT:
		case PROPERTY_DOUBLE:
		case PROPERTY_SHORT:
		case PROPERTY_LONG:
		case PROPERTY_RECTANGLE:
		case PROPERTY_STRING:
		break;

		case PROPERTY_VALUE:
#ifndef EKEKO_DEBUG
		printf("[Ekeko_Value] value create %d %d\n", type, value->type);
#endif
		ekeko_value_create(value, value->type);
		break;

		default:
#ifdef EKEKO_DEBUG
		printf("[Ekeko_Value] value create %d\n", type);
#endif
		{
			Ekeko_Value_Impl *impl;

			impl = _implementation_get(type);
			value->value.pointer_value = impl->create();
		}
		break;
	}
}
void ekeko_value_pointer_double_to(Ekeko_Value *value, Ekeko_Value_Type type, void *ptr,
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

		case PROPERTY_VALUE:
#ifndef EKEKO_DEBUG
		printf("[Ekeko_Value] Pointer double property value set %d %d\n", type, value->type);
#endif
		ekeko_value_pointer_double_to(value, value->type, ptr,
				prev, changed);
		break;

		default:
#ifdef EKEKO_DEBUG
		printf("[Ekeko_Value] Pointer double set %d\n", type);
#endif
		{
			Ekeko_Value_Impl *impl;

			impl = _implementation_get(type);
			impl->pointer_to(value, ptr);
			if (impl->cmp(ptr, prev))
				*changed = EINA_TRUE;
		}
		break;
	}
}

/*
 * ptr points to the memory area where the values are stored
 */
void ekeko_value_pointer_to(Ekeko_Value *value, Ekeko_Value_Type vtype, void *ptr)
{
	switch (vtype)
	{
		case PROPERTY_UNDEFINED:
		printf("[Ekeko_Value] pointer to undefined value?\n");
		break;

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

		case PROPERTY_VALUE:
		{
			Ekeko_Value *v = ptr;


			/* FIXME malloc the pointer */
			printf("[Ekeko_Value] value pointer to %p\n", v->value.pointer_value);
			v->type = value->type;
			/* TODO if the pointer already has a property allocated
			 * also free it
			 */
			if (v->value.pointer_value)
				ekeko_value_free(v, v->type);
			ekeko_value_create(v, v->type);
			ekeko_value_pointer_to(value, value->type, v->value.pointer_value);
		}
		break;

		default:
#ifdef EKEKO_DEBUG
		printf("[Ekeko_Value] Pointer to %d\n", vtype);
#endif
		{
			Ekeko_Value_Impl *impl;

			impl = _implementation_get(vtype);
			impl->pointer_to(value, ptr);
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
int ekeko_value_register(const char *name, Ekeko_Value_Create create,
		Ekeko_Value_Free free, Ekeko_Value_Compare cmp,
		Ekeko_Value_Pointer_From pointer_from,
		Ekeko_Value_Pointer_To pointer_to)
{
	static int _curr = PROPERTY_STRING; // the last internal property
	Ekeko_Value_Impl *impl;

	impl = malloc(sizeof(Ekeko_Value_Impl));
	impl->name = strdup(name);
	impl->cmp = cmp;
	impl->create = create;
	impl->free = free;
	impl->pointer_to = pointer_to;
	impl->pointer_from = pointer_from;

	++_curr;
	eina_hash_add(_values, &_curr, impl);
	return _curr;
}

void ekeko_value_pointer_from(Ekeko_Value *v, Ekeko_Value_Type vtype, void *ptr)
{
	switch (vtype)
	{
		case PROPERTY_UNDEFINED:
		v->type = PROPERTY_UNDEFINED;
		printf("[Ekeko_Value] pointer from undefined value?\n");
		break;

		case PROPERTY_STRING:
		v->type = PROPERTY_STRING;
		v->value.string_value = *(char **)ptr;
		break;

		case PROPERTY_INT:
		v->type = PROPERTY_INT;
		v->value.int_value = *(int *)ptr;
		break;

		case PROPERTY_RECTANGLE:
		v->type = PROPERTY_RECTANGLE;
		v->value.rect = *(Eina_Rectangle *)ptr;
		break;

		case PROPERTY_BOOL:
		v->type = PROPERTY_BOOL;
		v->value.bool_value = *(Eina_Bool *)ptr;
		break;

		case PROPERTY_VALUE:
		{
			Ekeko_Value *val = ptr;
#ifndef EKEKO_DEBUG
			printf("[Ekeko_Value] value pointer from %p %p\n", ptr, ((Ekeko_Value *)ptr)->value.pointer_value);
#endif
			ekeko_value_pointer_from(v, val->type, val->value.pointer_value);
		}
		break;

		default:
#ifdef EKEKO_DEBUG
		printf("[Ekeko_Value] Pointer from %p %p\n", v, ptr);
#endif
		{
			Ekeko_Value_Impl *impl;

			v->type = vtype;
			impl = _implementation_get(vtype);
			impl->pointer_from(v, ptr);
		}
		break;
	}
}

void ekeko_value_free(Ekeko_Value *v, Ekeko_Value_Type vtype)
{
	switch (vtype)
	{
		case PROPERTY_UNDEFINED:
		printf("[Ekeko_Value] freeing an undefined value?\n");
		break;

		case PROPERTY_INT:
		case PROPERTY_BOOL:
		case PROPERTY_CHAR:
		case PROPERTY_FLOAT:
		case PROPERTY_DOUBLE:
		case PROPERTY_SHORT:
		case PROPERTY_LONG:
		case PROPERTY_RECTANGLE:
		break;

		case PROPERTY_STRING:
		free(v->value.string_value);
		break;

		case PROPERTY_VALUE:
		ekeko_value_free(v, v->type);
		break;

		default:
		{
			Ekeko_Value_Impl *impl;

			impl = _implementation_get(vtype);
			impl->free(v->value.pointer_value);
			v->value.pointer_value = NULL;
		}
	}
}
