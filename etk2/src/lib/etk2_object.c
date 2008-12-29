#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <Eina.h>

#include "etk2_private.h"
#include "etk2_types.h"
#include "etk2_object.h"
#include "etk2_type.h"

//#define PRIVATE_OFFSET(obj) ((Object_Private*)((char*)(obj) + sizeof(Object) + sizeof(Object_Private*)))
#define PRIVATE(obj) ((Object_Private*)(obj->private))

#define TYPE_NAME "Object"

struct _Object_Private
{
	char *name;
	Type *type;
};

static void object_ctor(void *object);
static void object_dtor(void *object);

Type *object_type_get(void)
{
	static Type *object_type = NULL;

	if (!object_type)
	{
		object_type = type_new(TYPE_NAME, sizeof(Object), sizeof(Object_Private), NULL, object_ctor, object_dtor, object_property_value_set, object_property_value_get);
		type_property_new(object_type, "name", PROPERTY_VALUE_SINGLE_STATE, PROPERTY_STRING, OFFSET(Object_Private, name) + sizeof(Object_Private*), NULL);
	}

	return object_type;
}

Object *object_new(void)
{
	Object *object;

	object = type_instance_new(object_type_get());
	type_construct(object_type_get(), object);
	object_type_set(object, object_type_get());

	return object;
}

void object_type_set(Object *object, Type *type)
{
	Object_Private *private;

	RETURN_IF(object == NULL || type == NULL);

	//private = PRIVATE(object);

	RETURN_IF(object->private == NULL);

	//printf("setting %p 's type to %p\n", object->private, type);
	printf("%p %p %p\n", object, type, object->private->type);
	object->private->type = type;
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
	//printf("setting value for %s\n", prop_name);

	RETURN_IF(object == NULL || prop_name == NULL);

	//printf("obj_prop_value_set: %p %p %p\n", object, object->private, object->private->type);

	type_instance_property_value_set(object->private->type, object, prop_name, value);
}

Type_Property_Value *object_property_value_get(Object *object, char *prop_name)
{
	printf("object_prop_value_get: %s\n", prop_name);
	return NULL;
}

/** Implementation **/

static void object_ctor(void *object)
{
	Object *obj;

	obj = (Object*) object;

	//printf("obj private offset = %d at addr %p\n", OFFSET(Object, private), (&obj + OFFSET(Object, private)));

	obj->private = type_instance_private_get(object_type_get(), object);

	//printf("object_ctor(%p)\n", obj);
}

static void object_dtor(void *object)
{
	printf("object_dtor(%p)\n", object);
}
