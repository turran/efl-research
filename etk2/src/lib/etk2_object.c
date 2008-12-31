#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <Eina.h>

#include "etk2_private.h"
#include "etk2_types.h"
#include "etk2_object.h"
#include "etk2_type.h"
/*============================================================================*
 *                                  Local                                     *
 *============================================================================*/
#define PRIVATE(obj) ((Object_Private*)(obj->private))

#define TYPE_NAME "Object"

struct _Object_Private
{
	char *name;
	Type *type;
};

static void object_ctor(Type *ftype, void *instance)
{
	Object *object;

	object = (Object*) instance;
	object->private = type_instance_private_get(ftype, object_type_get(), object);
	object->private->type = ftype;
	printf("[obj] ctor %p %p %p\n", object, object->private, object->private->type);
}

static void object_dtor(void *object)
{
	printf("[obj] dtor %p\n", object);
}
/*============================================================================*
 *                                   API                                      *
 *============================================================================*/
Type *object_type_get(void)
{
	static Type *object_type = NULL;

	if (!object_type)
	{
		object_type = type_new(TYPE_NAME, sizeof(Object), sizeof(Object_Private), NULL, object_ctor, object_dtor, object_property_value_set, object_property_value_get);
		type_property_new(object_type, "name", PROPERTY_VALUE_SINGLE_STATE, PROPERTY_STRING, OFFSET(Object_Private, name), NULL);
	}

	return object_type;
}

Object *object_new(void)
{
	Object *object;

	object = type_instance_new(object_type_get());

	return object;
}

void object_name_set(Object *object, const char *name)
{
	Object_Private *private;

	//private = PRIVATE(object);

	if (name)
		object->private->name = strdup(name);
}

const char *object_name_get(Object *object)
{
	Object_Private *private;

	//private = PRIVATE(object);
	return object->private->name;
}

void object_property_value_set(Object *object, char *prop_name, Type_Property_Value *value)
{
	RETURN_IF(object == NULL || prop_name == NULL);

	printf("[obj] value_set: %s %p %p %p\n", prop_name, object, object->private, object->private->type);
	type_instance_property_value_set(object->private->type, object, prop_name, value);
}

Type_Property_Value *object_property_value_get(Object *object, char *prop_name)
{
	printf("[obj] value_get: %s\n", prop_name);
	return NULL;
}

Type * object_private_type_get(Object *object)
{
	return object->private->type;
}
