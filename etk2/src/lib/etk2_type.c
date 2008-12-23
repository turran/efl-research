#include <stdlib.h>
#include <string.h>

#include <Eina.h>

#include "etk2_types.h"
#include "etk2_private.h"
#include "etk2_type.h"
#include "etk2_object.h"

/**
 * @brief
 */
struct _Type
{
	char *name;
	size_t size;
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
	Type_Property_Type type;
	size_t offset;
	Type_Property_Value_Type value_type;
	Type_Property_Value *prev_value, *curr_value;
	Type_Property_Process *process;
};

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
Type *type_new(char *name, size_t size, Type *parent, Type_Constructor ctor, Type_Destructor dtor, Object_Property_Value_Set prop_value_set, Object_Property_Value_Get prop_value_get)
{
	Type *type;

	type = malloc(sizeof(Type));

	type->name = strdup(name);
	type->size = size;
	type->parent = parent;
	type->ctor = ctor;
	type->dtor = dtor;
	type->prop_value_get = prop_value_get;
	type->prop_value_set = prop_value_set;
	type->properties = eina_hash_string_superfast_new(NULL);

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

	instance = malloc(type_size_get(type));
	return instance;
}

/**
 * Constructs a type on the given object.
 *
 * @param type
 * @param object
 */
void type_construct(Type *type, void *object)
{
	if (!type || !object)
		return;

	if (type->parent)
		type_construct(type->parent, object);

	if (type->ctor)
		type->ctor(object);
}

/**
 *
 * @param type
 * @param object
 */
void type_destruct(Type *type, void *object)
{
	if (!type || !object)
		return;

	if (type->dtor)
		type->dtor(object);

	if (type->parent)
		type_destruct(type->parent, object);
}

/**
 *
 * @param type
 * @return
 */
size_t type_size_get(Type *type)
{
	size_t parent_size = 0;

	if (type->parent)
		parent_size = type_size_get(type->parent);

	return type->size + parent_size;
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

	property = malloc(sizeof(Type_Property));
	property->name = strdup(prop_name);
	property->type = prop_type;
	property->value_type = value_type;
	property->process = process_cb;
	property->offset = field_offset;
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

	printf("looking for property in type: %s\n", type->name);

	property = eina_hash_find(type->properties, prop_name);

	printf("property is: %p %s\n", property, property->name);

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

	printf("getting property: %s from type: %s (type addr = %p)\n", prop_name, type->name, type);

	property = type_property_get(type, prop_name);

	// TODO: give warning
	RETURN_IF(property == NULL);

	if (property->value_type == PROPERTY_STRING)
	{
		int parent_size = type->parent != NULL ? type_size_get(type->parent) : 0;

		printf("parent size for %s is %d, property offset is %d\n", type->name, parent_size, property->offset);

		char **str = (char**)((char*)instance + parent_size + property->offset);
		printf("setting property to '%s'\n", value->value.string_value);

		printf("address of property %s is %p\n", property->name, &str);
		*str = strdup(value->value.string_value);
		printf("*str = %s\n", *str);
	}
}
