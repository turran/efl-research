#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Etk2.h"
#include "etk2_private.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
/**
 * @brief
 */
struct _Type
{
	char *name;
	size_t size;
	size_t priv_size;
	Type *parent;

	Type_Constructor ctor;
	Type_Destructor dtor;

	Eina_Hash *properties;

	Object_Property_Value_Get prop_value_get;
	Object_Property_Value_Set prop_value_set;
};

/**
 * @brief
 */
struct _Type_Property
{
	char *name;
	Type_Property_Type prop_type;
	ssize_t offset;
	Type_Property_Value_Type value_type;
	Type_Property_Process *process;
	Type *type;
};

static size_t type_public_size_get(Type *type)
{
	return type->size;
}

static size_t type_private_size_get(Type *type)
{
	size_t parent_size = 0;

	if (!type) return 0;
	if (type->parent) parent_size = type_private_size_get(type->parent);

	return type->priv_size + parent_size;
}

static size_t type_size_get(Type *type)
{
	size_t parent_size = 0;

	if (!type) return 0;
	if (type->parent) parent_size = type_size_get(type->parent);

	return type->size + type->priv_size + parent_size;
}

static void * _instance_property_offset_get(Type *type, Type_Property *prop, void *instance)
{
	int offset = type_public_size_get(type) + type_private_size_get(prop->type->parent);
	return (char *)instance + offset + prop->offset;
}

static void type_destruct_internal(Type *type, void *object)
{
	if (!type)
		return;

	if (type->dtor)
		type->dtor(object);

	if (type->parent)
		type_destruct_internal(type->parent, object);
}

/**
 *
 * @param type
 * @param prop_name
 * @return
 */
static Type_Property *_property_get(Type *type, char *prop_name)
{
	Type_Property *property = NULL;

	//printf("[type] looking for property %s in type: %s\n", prop_name, type->name);
	do
	{
		property = eina_hash_find(type->properties, prop_name);
		if (!property)
			type = type->parent;
	} while (!property || !type);

	/*if (property)
	{
		printf("[type] found property %s on type %s with size %d\n", property->name, type->name, type->size);
	}*/

	return property;
}

/* TODO should we register types per document? */
static Eina_Hash *_types = NULL;
/*============================================================================*
 *                                 Global                                     *
 *============================================================================*/
void type_construct(Type *t, void *instance)
{
	if (!t)
		return;

	if (t->parent)
		type_construct(t->parent, instance);

	if (t->ctor)
		t->ctor(instance);
}

void * type_instance_private_get_internal(Type *final, Type *t, void *instance)
{
	printf("[type] private get %s (PUB=%d) %s (PRIV_OFF=%d) %p\n", final->name, type_public_size_get(final), t->name, type_private_size_get(t->parent), instance);
	return (char *)instance + type_public_size_get(final) + type_private_size_get(t->parent);
}
/**
 *
 * @param type
 * @param instance
 * @param prop_name
 * @param value
 */
void type_instance_property_value_set(Type *type, void *instance, char *prop_name, Type_Property_Value *value)
{
	void *curr;
	Type_Property *property;

	RETURN_IF(type == NULL || instance == NULL || prop_name == NULL || value == NULL);
	property = _property_get(type, prop_name);
	RETURN_IF(property == NULL);
	curr = _instance_property_offset_get(type, property, instance);

	if (property->value_type == PROPERTY_STRING)
	{
		//printf("[type] property->type->name = %s with size = %d\n", property->type->name, property->type->size);
		char **str = (char**)curr;
		*str = strdup(value->value.string_value);
	}
}

void type_instance_property_value_get(Type *type, void *instance, char *prop_name, Type_Property_Value *v)
{
	Type_Property *property;
	void *curr;

	if (!type || !instance || !prop_name)
		return;
	property = _property_get(type, prop_name);
	if (!property)
		return;
	curr = _instance_property_offset_get(type, property, instance);
	value_set(v, property->value_type, curr);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
/**
 * Creates a new Type.
 *
 * @param name the type's unique name.
 * @param size the type's size.
 * @param parent the type's parent, or <code>null</code> if it doesnt have one.
 * @param ctor the type's constuctor.
 * @param dtor the type's destructor.
 * @return the newly created type.
 */
Type *type_new(char *name, size_t size, size_t priv_size, Type *parent, Type_Constructor ctor, Type_Destructor dtor, Object_Property_Value_Set prop_value_set, Object_Property_Value_Get prop_value_get)
{
	Type *type;

	type = malloc(sizeof(Type));
	if (!type) return NULL;

	/* initialize the type */
	type->name = strdup(name);
	type->size = size;
	type->priv_size = priv_size;
	type->parent = parent;
	type->ctor = ctor;
	type->dtor = dtor;
	type->prop_value_get = prop_value_get;
	type->prop_value_set = prop_value_set;
	type->properties = eina_hash_string_superfast_new(NULL);
	/* add the type */
	if (!_types) _types = eina_hash_string_superfast_new(NULL);
	eina_hash_add(_types, name, type);

	return type;
}

/**
 * Creates a new instance of the given type.
 *
 * @param type the Type to instantiate.
 * @return the instantiated type.
 */
void *type_instance_new(Type *type)
{
	void *instance;

	if (!type)
		return NULL;
	instance = malloc(type_size_get(type));
	if (!instance)
		return NULL;
	object_construct(type, instance);
	return instance;
}

void * type_instance_new_name_from(const char *name)
{
	Type *t;

	t = eina_hash_find(_types, name);
	return type_instance_new(t);
}
/**
 *
 * @param instance
 */
void type_instance_delete(void *instance)
{
	Type *type;
	if (!instance)
		return;
	type = object_private_type_get(instance);
	if (!type)
		return;
	type_destruct_internal(type, instance);
}

/**
 *
 * @param type
 * @param prop_name
 * @param prop_type
 * @param value_type
 * @param process_cb
 */
void type_property_new(Type *type, char *prop_name, Type_Property_Type prop_type, Type_Property_Value_Type value_type, size_t field_offset, Type_Property_Process *process_cb)
{
	Type_Property *property;

	RETURN_IF(type == NULL || prop_name == NULL);
	printf("[type] new %s property at offset %d\n", prop_name, field_offset);
	property = malloc(sizeof(Type_Property));
	property->name = strdup(prop_name);
	property->prop_type = prop_type;
	property->value_type = value_type;
	property->process = process_cb;
	property->offset = field_offset;
	property->type = type;
	eina_hash_add(type->properties, prop_name, property);
}

#if 0
void type_property_new(Type *type, char *prop_name,
		Type_Property_Type prop_type, Type_Property_Value_Type value_type,
		ssize_t field_offset, ssize_t prev_offset, ssize_t changed)
{
	/* How to handle the changed thing?
	 * usually you do foo_changed:1
	 * but is impossible to get offset of a bit field (man offsetof)
	 * why dont use an uint32/64_t as a bit mask?
	 *
	 * We dont need the process function, just a generic callback handler
	 * for a change after the curr == prev comparision in case it has changed
	 *
	 */
}
#endif

EAPI void * type_instance_private_get(Type *t, void *instance)
{
	Type *final;

	final = object_private_type_get(instance);
	return type_instance_private_get_internal(final, t, instance);
}
