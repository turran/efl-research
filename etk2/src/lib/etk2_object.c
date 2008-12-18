#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <Eina.h>

#include "etk2_types.h"
#include "etk2_object.h"
#include "etk2_type.h"

#define PRIVATE(obj) ((Object_Private*)(obj))

#define TYPE_NAME "Object"

struct _Object_Private
{
	char *name;
	Type *type;
};

static void object_ctor(void *object);
static void object_dtor(void *object);
static void object_property_value_set(Object *object, char *prop_name, Type_Property_Value *value);
static Type_Property_Value *object_property_value_get(Object *object, char *prop_name);

Type *object_type_get(void)
{
	static Type *object_type = NULL;

	if (!object_type)
	{
		object_type = type_new(TYPE_NAME, sizeof(Object_Private), NULL, object_ctor, object_dtor, object_property_value_set, object_property_value_get);
		type_property_new(object_type, "name", PROPERTY_VALUE_SINGLE_STATE, PROPERTY_STRING, 0, NULL);

	}

	return object_type;
}

Object *object_new(void)
{
	Object *object;

	object = type_instance_new(object_type_get());
	type_construct(object_type_get(), object);
	object->private->type = object_type_get();

	return object;
}

void object_name_set(Object *object, const char *name)
{
	Object_Private *private;

	private = PRIVATE(object);

	if (name)
		private->name = strdup(name);
}

const char *object_name_get(Object *object)
{
	Object_Private *private;

	private = PRIVATE(object);
	return private->name;
}

/** Implementation **/

static void object_ctor(void *object)
{
	Object_Private *private;

	private = PRIVATE(object);

	printf("object_ctor(%p)\n", object);
}

static void object_dtor(void *object)
{
	printf("object_dtor(%p)\n", object);
}

static void object_property_value_set(Object *object, char *prop_name, Type_Property_Value *value)
{
	printf("object_prop_value_set: %s\n", prop_name);
}

static Type_Property_Value *object_property_value_get(Object *object, char *prop_name)
{
	printf("object_prop_value_get: %s\n", prop_name);
	return NULL;
}
