#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <Eina.h>

#include "etk2_types.h"
#include "etk2_private.h"
#include "etk2_type.h"
#include "etk2_object.h"
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
	size_t offset;
	Type_Property_Value_Type value_type;
	Type_Property_Value *prev_value, *curr_value;
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

static void * type_instance_property_offset_get(Type *type, Type_Property *prop, void *instance)
{
	int offset = type_public_size_get(type) + type_private_size_get(prop->type->parent);
	return (char *)instance + offset + prop->offset;
}

static void type_construct_internal(Type *ftype, Type *type, void *instance)
{
	if (!type)
		return;

	if (type->parent)
		type_construct_internal(ftype, type->parent, instance);

	if (type->ctor)
		type->ctor(ftype, instance);
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
/* TODO should we register types per document? */
Eina_Hash *_types = NULL;
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
	type_construct_internal(type, type, instance);
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

/**
 *
 * @param type
 * @param prop_name
 * @return
 */
Type_Property *type_property_get(Type *type, char *prop_name)
{
	Type_Property *property = NULL;

	RETURN_NULL_IF(type == NULL || type->properties == NULL || prop_name == NULL);

	printf("[type] looking for property %s in type: %s\n", prop_name, type->name);
	do
	{
		property = eina_hash_find(type->properties, prop_name);
		if (!property)
			type = type->parent;
	} while (!property || !type);

	if (property)
	{
		printf("[type] found property %s on type %s with size %d\n", property->name, type->name, type->size);
	}

	return property;
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
	Type_Property *property;

	RETURN_IF(type == NULL || instance == NULL || prop_name == NULL || value == NULL);

	//printf("getting property: %s from type: %s (type addr = %p)\n", prop_name, type->name, type);

	property = type_property_get(type, prop_name);

	if (property == NULL)
	{
		fprintf(stderr, "WARNING: Property %s not found in type %s\n", prop_name, type->name);
	}

	// TODO: give warning
	RETURN_IF(property == NULL);

	if (property->value_type == PROPERTY_STRING)
	{
		printf("[type] property->type->name = %s with size = %d\n", property->type->name, property->type->size);

		char **str = (char**)type_instance_property_offset_get(type, property, instance);
		//printf("total offset for %s = %d + %d = %d at addr=%p\n", property->name, offset, property->offset, offset + property->offset, str);
		//printf("setting property to '%s'\n", value->value.string_value);

		//printf("address of property %s is %p\n", property->name, str);
		*str = strdup(value->value.string_value);
		//printf("*str = %s\n", *str);
	}
}

void * type_instance_private_get(Type *instance_type, Type *priv_type, void *instance)
{
	printf("[type] private get %s (PUB=%d) %s (PRIV_OFF=%d) %p\n", instance_type->name, type_public_size_get(instance_type), priv_type->name, type_private_size_get(priv_type->parent), instance);
	return (char *)instance + type_public_size_get(instance_type) + type_private_size_get(priv_type->parent);
}
